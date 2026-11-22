#!/usr/bin/env python3
"""Gangster Outbreak: turn a trainer battle picture into a 64x64, 16-colour GBA front pic.
The background is removed by flood fill from the edges (so it may also appear inside the figure),
the figure is scaled down to fit 64x64 if needed and stands on the bottom edge.

usage: import_trainer_pic.py [--outline] PIC.png OUT.png [x0 y0 x1 y1]
  x0 y0 x1 y1: optional crop box when the file holds several pictures
  --outline:   draw a dark 1-pixel outline around the figure (for scaled-down drawings without one)
  --max=WxH:   largest figure size (default 64x64; the FRLG trainers are about 54x60)
  --tolerance=N: colours this close to the background also count as background (default 120 with --outline, else 0)
"""
import os, sys
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from import_sheet import remove_background, TRANSPARENT  # noqa: E402

SIZE = 64


def main(src, dst, box=None, outline=False, tolerance=None, max_w=SIZE, max_h=SIZE):
    im = Image.open(src).convert('RGBA')
    if box:
        im = im.crop(box)
    # drawings have soft edges: with --outline, colours close to the background count as background too
    if tolerance is None:
        tolerance = 120 if outline else 0
    remove_background(im, tolerance)
    im = im.crop(im.getbbox())
    if im.width > max_w or im.height > max_h:
        f = min(max_w / im.width, max_h / im.height)
        im = im.resize((max(1, round(im.width * f)), max(1, round(im.height * f))), Image.BOX)
    canvas = Image.new('RGBA', (SIZE, SIZE), (0, 0, 0, 0))
    canvas.paste(im, ((SIZE - im.width) // 2, SIZE - im.height), im)
    rgb = Image.new('RGB', canvas.size, (0, 0, 0))
    mask = canvas.getchannel('A').point(lambda a: 255 if a >= 128 else 0)
    if outline:
        mp0 = mask.load()
        edge = [(x, y) for y in range(SIZE) for x in range(SIZE) if not mp0[x, y]
                and any(0 <= x + dx < SIZE and 0 <= y + dy < SIZE and mp0[x + dx, y + dy]
                        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))]
        for x, y in edge:
            mp0[x, y] = 255
            rgb.putpixel((x, y), (32, 24, 32))
    rgb.paste(canvas.convert('RGB'), (0, 0), mask)
    # 15 colours for the figure (octree keeps small accents like skin and glasses), index 0 is transparent
    mp = mask.load()
    px = rgb.load()
    opaque = [px[x, y] for y in range(SIZE) for x in range(SIZE) if mp[x, y]]
    sample = Image.new('RGB', (len(opaque), 1))
    sample.putdata(opaque)
    pal = sample.quantize(15, method=Image.Quantize.FASTOCTREE, dither=Image.Dither.NONE).getpalette()[:45]
    colours = [tuple(pal[i:i + 3]) for i in range(0, len(pal), 3)]
    res = Image.new('P', canvas.size, 0)
    res.putpalette(list(TRANSPARENT) + pal + [0] * 3 * (15 - len(colours)))
    rp, cache = res.load(), {}
    for y in range(SIZE):
        for x in range(SIZE):
            if mp[x, y]:
                c = px[x, y]
                if c not in cache:
                    cache[c] = 1 + min(range(len(colours)), key=lambda i: sum((a - b) ** 2 for a, b in zip(c, colours[i])))
                rp[x, y] = cache[c]
    res.save(dst)
    print(dst)


if __name__ == '__main__':
    args = sys.argv[1:]
    outline = '--outline' in args
    tolerance = next((int(a.split('=')[1]) for a in args if a.startswith('--tolerance=')), None)
    mw, mh = next((tuple(int(v) for v in a.split('=')[1].split('x')) for a in args if a.startswith('--max=')), (SIZE, SIZE))
    args = [a for a in args if not a.startswith('--')]
    main(args[0], args[1], tuple(int(v) for v in args[2:6]) if len(args) >= 6 else None, outline, tolerance, mw, mh)
