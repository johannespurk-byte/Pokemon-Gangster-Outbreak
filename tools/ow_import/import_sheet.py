#!/usr/bin/env python3
"""Gangster Outbreak: turn a 4x4 walking sheet (rows: down, left, right, up; columns: stand, step, stand, step)
into a GBA overworld strip of nine 32x32 frames in the order the game expects:
down, up, left, walk down x2, walk up x2, walk left x2 (right is mirrored by the game).
Sheets drawn at 2x size are scaled down. The background is removed by flood fill from the frame edges,
so the background colour may also appear inside the sprite. At most 15 colours are kept.

usage: import_sheet.py SHEET.png OUT.png
"""
import sys
from collections import deque
from PIL import Image

FRAME = 32
ORDER = [(0, 0), (3, 0), (1, 0), (0, 1), (0, 3), (3, 1), (3, 3), (1, 1), (1, 3)]  # (row, column)
TRANSPARENT = (255, 0, 255)


def main(src, dst):
    im = Image.open(src).convert('RGBA')
    cell = im.width // 4
    scale = cell // FRAME
    if scale > 1:
        im = im.resize((im.width // scale, im.height // scale), Image.NEAREST)
    out = Image.new('RGBA', (FRAME * len(ORDER), FRAME), (0, 0, 0, 0))
    for i, (r, c) in enumerate(ORDER):
        fr = im.crop((c * FRAME, r * FRAME, c * FRAME + FRAME, r * FRAME + FRAME))
        px = fr.load()
        bg = px[0, 0]
        seen = set()
        todo = deque((x, y) for x in range(FRAME) for y in (0, FRAME - 1)) + deque((x, y) for y in range(FRAME) for x in (0, FRAME - 1))
        while todo:
            x, y = todo.popleft()
            if (x, y) in seen or not (0 <= x < FRAME and 0 <= y < FRAME):
                continue
            seen.add((x, y))
            p = px[x, y]
            if p[3] == 0 or p[:3] == bg[:3]:
                px[x, y] = (0, 0, 0, 0)
                todo.extend(((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)))
        out.paste(fr, (i * FRAME, 0))
    # quantize the sprite colours to 15
    opaque = [p[:3] for p in out.getdata() if p[3]]
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
