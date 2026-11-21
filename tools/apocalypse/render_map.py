#!/usr/bin/env python3
"""Render a map layout from the game data to a PNG (preview for tileset/palette edits).

Usage: render_map.py LAYOUT_ID out.png [--pal-root DIR]
--pal-root lets you preview palettes from another checkout/copy of data/tilesets.
"""
import json, os, re, struct, sys
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def read_pal(path):
    lines = open(path).read().split('\n')
    return [tuple(int(v) for v in l.split()) for l in lines[3:19]]


def tileset_dir(symbol):
    headers = open(os.path.join(ROOT, 'src/data/tilesets/headers.h')).read()
    m = re.search(r'const struct Tileset %s =\s*\{(.*?)\};' % re.escape(symbol), headers, re.S)
    tiles_sym = re.search(r'\.tiles = (\w+)', m.group(1)).group(1)
    gfx = open(os.path.join(ROOT, 'src/data/tilesets/graphics.h')).read()
    path = re.search(r'%s\[\] = INCGFX_U32\("([^"]+)/tiles\.png"' % re.escape(tiles_sym), gfx).group(1)
    return path


def load_tileset(path, pal_root):
    tiles = Image.open(os.path.join(ROOT, path, 'tiles.png'))
    pals = [read_pal(os.path.join(pal_root, path, 'palettes', '%02d.pal' % i)) for i in range(16)]
    metatiles = open(os.path.join(ROOT, path, 'metatiles.bin'), 'rb').read()
    return tiles, pals, metatiles


def render(layout_id, out, pal_root=ROOT):
    layout = next(l for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts'] if l.get('id') == layout_id)
    frlg = layout.get('layout_version') == 'frlg'
    n_tiles = 640 if frlg else 512
    n_meta = 640 if frlg else 512
    n_pals = 7 if frlg else 6
    prim = load_tileset(tileset_dir(layout['primary_tileset']), pal_root)
    sec = load_tileset(tileset_dir(layout['secondary_tileset']), pal_root)
    pals = prim[1][:n_pals] + sec[1][n_pals:]
    tile_cache = {}

    def tile(idx, pal, hflip, vflip):
        key = (idx, pal, hflip, vflip)
        if key not in tile_cache:
            src = prim[0] if idx < n_tiles else sec[0]
            i = idx if idx < n_tiles else idx - n_tiles
            w = src.width // 8
            t = src.crop(((i % w) * 8, (i // w) * 8, (i % w) * 8 + 8, (i // w) * 8 + 8))
            if hflip: t = t.transpose(Image.FLIP_LEFT_RIGHT)
            if vflip: t = t.transpose(Image.FLIP_TOP_BOTTOM)
            px = list(t.getdata())
            colors = pals[pal]
            img = Image.new('RGBA', (8, 8))
            img.putdata([(0, 0, 0, 0) if p == 0 else colors[p] + (255,) for p in px])
            tile_cache[key] = img
        return tile_cache[key]

    def metatile(mid):
        data = prim[2] if mid < n_meta else sec[2]
        i = mid if mid < n_meta else mid - n_meta
        entries = struct.unpack_from('<8H', data, i * 16)
        img = Image.new('RGBA', (16, 16), pals[0][0] + (255,))
        for n, e in enumerate(entries):
            t = tile(e & 0x3FF, (e >> 12) & 0xF, e & 0x400, e & 0x800)
            img.alpha_composite(t, ((n % 2) * 8, ((n % 4) // 2) * 8))
        return img

    w, h = layout['width'], layout['height']
    blocks = struct.unpack('<%dH' % (w * h), open(os.path.join(ROOT, layout['blockdata_filepath']), 'rb').read())
    out_img = Image.new('RGBA', (w * 16, h * 16))
    cache = {}
    for y in range(h):
        for x in range(w):
            mid = blocks[y * w + x] & 0x3FF
            if mid not in cache:
                cache[mid] = metatile(mid)
            out_img.paste(cache[mid], (x * 16, y * 16))
    out_img.convert('RGB').save(out)


if __name__ == '__main__':
    pal_root = ROOT
    if '--pal-root' in sys.argv:
        pal_root = sys.argv[sys.argv.index('--pal-root') + 1]
    render(sys.argv[1], sys.argv[2], pal_root)
