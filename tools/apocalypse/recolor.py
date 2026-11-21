#!/usr/bin/env python3
"""Gangster Outbreak: recolor the Kanto (FRLG) tilesets into an apocalyptic look.

Always reads the ORIGINAL palettes from git (BASE_REF), so it can be re-run with new settings
without stacking the effect. Writes the recolored .pal files into data/tilesets.

Usage: recolor.py [--out DIR]   (default: write into the repo)
"""
import colorsys, json, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE_REF = 'ddfe1f3d'  # last commit with the untouched FRLG palettes

RUST = (120, 58, 34)  # ash/rust haze over everything outdoors


def clamp(v):
    return max(0, min(255, int(round(v))))


def blend(c, target, amount):
    return tuple(a + (b - a) * amount for a, b in zip(c, target))


def snap(c):
    # GBA colors are 5 bits per channel
    return tuple(clamp(v) // 8 * 8 for v in c)


def hue_deg(h):
    return h * 360


def outdoor(rgb):
    r, g, b = (v / 255 for v in rgb)
    h, s, v = colorsys.rgb_to_hsv(r, g, b)
    deg = hue_deg(h)
    if s > 0.15 and 65 <= deg < 170:
        # vegetation -> dead: light greens become dry ashen grass, dark greens charred trees
        if v > 0.6:
            h, s, v = 34 / 360, s * 0.45, v * 0.70
        else:
            h, s, v = 22 / 360, s * 0.30, v * 0.45
    elif s > 0.15 and 170 <= deg < 255:
        # water / blue -> polluted, murky
        h, s, v = 84 / 360, s * 0.60, v * 0.50
    elif s > 0.15:
        # roofs, flowers, signs -> faded and dirty
        s, v = s * 0.55, v * 0.70
    else:
        # greys / whites -> dusty concrete
        v = v * 0.72
    rgb = [c * 255 for c in colorsys.hsv_to_rgb(h, s, v)]
    return snap(blend(rgb, RUST, 0.16))


def indoor(rgb):
    r, g, b = (v / 255 for v in rgb)
    h, s, v = colorsys.rgb_to_hsv(r, g, b)
    s, v = s * 0.70, v * 0.86
    rgb = [c * 255 for c in colorsys.hsv_to_rgb(h, s, v)]
    return snap(blend(rgb, RUST, 0.07))


def tileset_dirs():
    import re
    headers = open(os.path.join(ROOT, 'src/data/tilesets/headers.h')).read()
    gfx = open(os.path.join(ROOT, 'src/data/tilesets/graphics.h')).read()

    def path_of(symbol):
        m = re.search(r'const struct Tileset %s =\s*\{(.*?)\};' % re.escape(symbol), headers, re.S)
        tiles = re.search(r'\.tiles = (\w+)', m.group(1)).group(1)
        return re.search(r'%s\[\] = INCGFX_U32\("([^"]+)/tiles\.png"' % re.escape(tiles), gfx).group(1)

    layouts = json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']
    kinds = {}
    for l in layouts:
        if l.get('layout_version') != 'frlg':
            continue
        kind = 'outdoor' if l['primary_tileset'] == 'gTileset_General_Frlg' else 'indoor'
        for sym in (l['primary_tileset'], l['secondary_tileset']):
            p = path_of(sym)
            if kinds.get(p) != 'outdoor':  # outdoor wins if a tileset is used in both
                kinds[p] = kind
    return kinds


def read_original(path):
    return subprocess.run(['git', '-C', ROOT, 'show', '%s:%s' % (BASE_REF, path)], capture_output=True, text=True, check=True).stdout


def main():
    out_root = ROOT
    if '--out' in sys.argv:
        out_root = sys.argv[sys.argv.index('--out') + 1]
    for path, kind in sorted(tileset_dirs().items()):
        fn = outdoor if kind == 'outdoor' else indoor
        for i in range(16):
            rel = '%s/palettes/%02d.pal' % (path, i)
            lines = read_original(rel).split('\n')
            colors = [tuple(int(v) for v in l.split()) for l in lines[3:19]]
            new = [colors[0]] + [fn(c) for c in colors[1:]]  # color 0 is transparent
            dst = os.path.join(out_root, rel)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with open(dst, 'w', newline='\r\n') as f:
                f.write('JASC-PAL\n0100\n16\n' + ''.join('%d %d %d\n' % c for c in new))
        print(kind, path)


if __name__ == '__main__':
    main()
