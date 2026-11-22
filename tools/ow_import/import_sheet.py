#!/usr/bin/env python3
"""Gangster Outbreak: turn a 4x4 walking sheet (rows: down, left, right, up; columns: stand, step, stand, step)
into a GBA overworld strip of nine frames (16x32, or 32x32 for wide sprites) in the order the game expects:
down, up, left, walk down x2, walk up x2, walk left x2 (right is mirrored by the game).
Sheets drawn at 2x size are scaled down (detected automatically). The background is removed by flood fill from the frame edges,
so the background colour may also appear inside the sprite. At most 15 colours are kept.

usage: import_sheet.py SHEET.png OUT.png
"""
import sys
from collections import deque
from PIL import Image

FRAME = 32
ORDER = [(0, 0), (3, 0), (1, 0), (0, 1), (0, 3), (3, 1), (3, 3), (1, 1), (1, 3)]  # (row, column)
TRANSPARENT = (255, 0, 255)


def is_doubled(im):
    px, bad, total = im.load(), 0, 0
    for y in range(0, im.height - 1, 2):
        for x in range(0, im.width - 1, 2):
            total += 1
            p = px[x, y]
            if px[x + 1, y] != p or px[x, y + 1] != p or px[x + 1, y + 1] != p:
                bad += 1
    return bad * 100 < total


def remove_background(fr, tolerance=0):
    px = fr.load()
    w, h = fr.size
    bg = px[0, 0]
    seen = set()
    todo = deque([(x, y) for x in range(w) for y in (0, h - 1)] + [(x, y) for y in range(h) for x in (0, w - 1)])
    while todo:
        x, y = todo.popleft()
        if (x, y) in seen or not (0 <= x < w and 0 <= y < h):
            continue
        seen.add((x, y))
        p = px[x, y]
        if p[3] == 0 or sum(abs(a - b) for a, b in zip(p[:3], bg[:3])) <= tolerance:
            px[x, y] = (0, 0, 0, 0)
            todo.extend(((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)))


def main(src, dst):
    im = Image.open(src).convert('RGBA')
    while is_doubled(im):
        im = im.resize((im.width // 2, im.height // 2), Image.NEAREST)
    cw, ch = im.width // 4, im.height // 4
    cells = []
    for r, c in ORDER:
        fr = im.crop((c * cw, r * ch, c * cw + cw, r * ch + ch))
        remove_background(fr)
        cells.append(fr)
    # common box of all frames, so the figure does not jump between frames
    boxes = [f.getbbox() for f in cells]
    x0 = min(b[0] for b in boxes); y0 = min(b[1] for b in boxes)
    x1 = max(b[2] for b in boxes); y1 = max(b[3] for b in boxes)
    width = 16 if x1 - x0 <= 16 else 32
    if x1 - x0 > 32 or y1 - y0 > FRAME:
        sys.exit('%s: figure is %dx%d, too big for a %dx%d frame' % (src, x1 - x0, y1 - y0, width, FRAME))
    ox = (width - (x1 - x0)) // 2 - x0
    # feet stay at the bottom: taller cells are cut at the figure's feet, shorter cells sit at the bottom
    oy = FRAME - y1 if ch > FRAME else FRAME - ch
    if ch <= FRAME and cw == width:
        ox = 0
    out = Image.new('RGBA', (width * len(ORDER), FRAME), (0, 0, 0, 0))
    for i, fr in enumerate(cells):
        out.paste(fr, (i * width + ox, oy), fr)
    # quantize the sprite colours to 15
    opaque = [p[:3] for p in out.get_flattened_data() if p[3]]
    colours = sorted(set(opaque))
    if len(colours) > 15:
        sample = Image.new('RGB', (len(opaque), 1))
        sample.putdata(opaque)
        q = sample.quantize(15, method=Image.MEDIANCUT)
        pal = q.getpalette()[:45]
        colours = [tuple(pal[i * 3:i * 3 + 3]) for i in range(15)]
    def nearest(p):
        return min(range(len(colours)), key=lambda i: sum((a - b) ** 2 for a, b in zip(p, colours[i])))
    res = Image.new('P', out.size, 0)
    res.putpalette(list(TRANSPARENT) + [v for c in colours for v in c] + [0] * 3 * (15 - len(colours)))
    cache = {}
    rp, op = res.load(), out.load()
    for y in range(out.height):
        for x in range(out.width):
            p = op[x, y]
            if p[3]:
                k = p[:3]
                if k not in cache:
                    cache[k] = 1 + nearest(k)
                rp[x, y] = cache[k]
    res.save(dst)
    print(dst, len(colours), 'colours')


if __name__ == '__main__':
    main(*sys.argv[1:3])
