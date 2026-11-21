#!/usr/bin/env python3
"""Gangster Outbreak: turn a trainer battle picture into a 64x64, 16-colour GBA front pic.
The background is removed by flood fill from the edges (so it may also appear inside the figure),
the figure is scaled down to fit 64x64 if needed and stands on the bottom edge.

usage: import_trainer_pic.py PIC.png OUT.png [x0 y0 x1 y1]   (optional crop box when the file holds several pictures)
"""
import os, sys
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from import_sheet import remove_background, TRANSPARENT  # noqa: E402

SIZE = 64


def main(src, dst, box=None):
    im = Image.open(src).convert('RGBA')
    if box:
        im = im.crop(box)
    remove_background(im)
    im = im.crop(im.getbbox())
    if im.width > SIZE or im.height > SIZE:
        f = min(SIZE / im.width, SIZE / im.height)
        im = im.resize((max(1, round(im.width * f)), max(1, round(im.height * f))), Image.BOX)
    canvas = Image.new('RGBA', (SIZE, SIZE), (0, 0, 0, 0))
    canvas.paste(im, ((SIZE - im.width) // 2, SIZE - im.height), im)
    rgb = Image.new('RGB', canvas.size, (0, 0, 0))
    mask = canvas.getchannel('A').point(lambda a: 255 if a >= 128 else 0)
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
    main(sys.argv[1], sys.argv[2], tuple(int(v) for v in sys.argv[3:7]) if len(sys.argv) >= 7 else None)
