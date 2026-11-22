#!/usr/bin/env python3
"""Gangster Outbreak: every gym looks like the hideout of the villain team that runs it.

Per gym:
  * the gym's own secondary palettes are recoloured in the team's colours (Team Yell black/pink, Team Skull
    grey/white, Neo Rocket black/red, Aqua & Magma blue/red, Plasma cold blue/white, Flare red/orange,
    Galactic teal/silver, Giovanni black/gold),
  * the team emblem is painted on the floor (2x2, walkable) and tags / small emblems are sprayed on the walls.
Only graphics change: collision, elevation and metatile behaviours stay as they are, so the gym puzzles
(water, trash cans, teleport pads, invisible walls, trees, quiz doors) keep working.

Pewter's tileset is shared with the Saffron Dojo, so Pewter gets its own copy (gang_pewter_gym_frlg).
Like the other apocalypse tools it starts from fixed git states, so it can be re-run.

Usage: gyms.py [--preview DIR]
"""
import colorsys, json, os, random, re, shutil, struct, subprocess, sys
from collections import Counter

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from interiors import load_tiles, read_pal, nearest  # noqa: E402
from gym_puzzles import GYM_PUZZLES, Puzzle, SPIN, LEDGE  # noqa: E402
import puzzle_art  # noqa: E402
from PIL import Image  # noqa: E402

STAGE_A_REF = '862c4b3d'  # tilesets after recolor.py (stage A); recolouring starts from here
BASE_REF = 'ddfe1f3d'     # gym layouts before any gang decoration
PRIM = 'data/tilesets/primary/building_frlg'
NUM_PRIM = 640
MAX_SEC = 384


def git_bytes(path, ref):
    return subprocess.run(['git', 'show', '%s:%s' % (ref, path)], cwd=ROOT, capture_output=True, check=True).stdout


# ------------------------------------------------------------------ team colour schemes

def hsv(rgb):
    return colorsys.rgb_to_hsv(*(c / 255 for c in rgb))


def rgb(h, s, v):
    return tuple(max(0, min(255, int(round(c * 255)))) for c in colorsys.hsv_to_rgb(h % 1.0, max(0, min(1, s)), max(0, min(1, v))))


def scheme_yell(c):          # black and grey with hot pink where there was colour
    h, s, v = hsv(c)
    if s > 0.35 and v > 0.35:
        return rgb(0.91, min(1, s * 1.25), v * 0.95)
    return rgb(0.85, s * 0.15, v * 0.62)


def scheme_skull(c):         # dirty concrete grey and white, murky water
    h, s, v = hsv(c)
    if 0.45 <= h <= 0.7 and s > 0.2:
        return rgb(0.42, s * 0.55, v * 0.72)  # the pool: murky green-grey broth
    return rgb(h, s * 0.12, min(1, v * 0.92))


def scheme_rocket(c):        # black and red
    h, s, v = hsv(c)
    if s > 0.3:
        return rgb(0.99, min(1, s * 1.1), v * 0.8)
    return rgb(0.0, s * 0.2, v * 0.55)


def scheme_aqua_magma(c):    # greens and blues become Aqua blue, the rest Magma red
    h, s, v = hsv(c)
    if 0.17 <= h <= 0.58 and s > 0.12:
        return rgb(0.6, min(1, s * 1.15), v * 0.9)
    return rgb(0.01, s * 0.95, v * 0.72)


def scheme_plasma(c):        # cold castle: blue-grey and white
    h, s, v = hsv(c)
    return rgb(0.61, s * 0.55, min(1, v * 1.02))


def scheme_flare(c):         # red and orange, chic
    h, s, v = hsv(c)
    if s > 0.15:
        return rgb(0.03, min(1, s * 1.15), v * 0.9)
    return rgb(0.03, 0.08, v * 0.7)


def scheme_galactic(c):      # teal and silver
    h, s, v = hsv(c)
    return rgb(0.49, s * 0.6, v * 0.95)


def scheme_giovanni(c):      # black and gold
    h, s, v = hsv(c)
    if s > 0.25:
        return rgb(0.12, min(1, s * 0.9), v * 0.85)
    return rgb(0.1, s * 0.15, v * 0.5)


# ------------------------------------------------------------------ emblems (32x32 RGBA, None = transparent)

GLYPHS = {
    'Y': ["#.....#", "##...##", ".##.##.", "..###..", "...#...", "...#...", "...#...", "...#...", "..###.."],
    'R': ["######.", "##...##", "##...##", "##...##", "######.", "##.##..", "##..##.", "##...##", "##...##"],
    'A': ["..###..", ".##.##.", "##...##", "##...##", "#######", "##...##", "##...##", "##...##", "##...##"],
    'M': ["##...##", "###.###", "#######", "##.#.##", "##...##", "##...##", "##...##", "##...##", "##...##"],
    'P': ["######.", "##...##", "##...##", "##...##", "######.", "##.....", "##.....", "##.....", "##....."],
    'F': ["#######", "##.....", "##.....", "#####..", "##.....", "##.....", "##.....", "##.....", "##....."],
    'G': [".#####.", "##...##", "##.....", "##.....", "##.####", "##...##", "##...##", "##...##", ".#####."],
}


class Img:
    def __init__(self, w=32, h=32):
        self.w, self.h = w, h
        self.p = [[None] * w for _ in range(h)]

    def set(self, x, y, c):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.p[y][x] = c

    def disc(self, cx, cy, r, c, ry=None):
        ry = ry or r
        for y in range(self.h):
            for x in range(self.w):
                if ((x + 0.5 - cx) / r) ** 2 + ((y + 0.5 - cy) / ry) ** 2 <= 1:
                    self.p[y][x] = c

    def glyph(self, ch, x0, y0, c, scale=2):
        for gy, row in enumerate(GLYPHS[ch]):
            for gx, v in enumerate(row):
                if v == '#':
                    for dy in range(scale):
                        for dx in range(scale):
                            self.set(x0 + gx * scale + dx, y0 + gy * scale + dy, c)

    def outline(self, c):
        add = [(x, y) for y in range(self.h) for x in range(self.w) if self.p[y][x] is None and any(
            0 <= x + dx < self.w and 0 <= y + dy < self.h and self.p[y + dy][x + dx] not in (None, c)
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))]
        for x, y in add:
            self.p[y][x] = c

    def worn(self, seed, chance=0.12):
        """paint on a floor: scuffed and partly worn off"""
        rng = random.Random(seed)
        for y in range(self.h):
            for x in range(self.w):
                if self.p[y][x] is not None and rng.random() < chance:
                    self.p[y][x] = None

    def scaled(self, w, h):
        out = Img(w, h)
        for y in range(h):
            for x in range(w):
                out.p[y][x] = self.p[y * self.h // h][x * self.w // w]
        return out


INK = (24, 20, 28)


def emblem_letter(ch, ring, letter, inner=None):
    e = Img()
    e.disc(16, 16, 15, ring)
    e.disc(16, 16, 12, inner or INK)
    e.glyph(ch, 9, 7, letter)
    e.outline(INK)
    return e


def emblem_skull():
    e = Img()
    white, bone, dark = (232, 232, 224), (200, 196, 184), INK
    for a, b in (((4, 6), (27, 27)), ((27, 6), (4, 27))):  # crossed bones
        for i in range(40):
            x = round(a[0] + (b[0] - a[0]) * i / 39)
            y = round(a[1] + (b[1] - a[1]) * i / 39)
            for d in (-1, 0, 1):
                e.set(x + d, y, bone)
        for (x, y) in (a, b):
            e.disc(x + 0.5, y + 0.5, 2.2, bone)
    e.disc(16, 13, 9, white, 8)
    e.disc(16, 21, 6, white, 4)
    e.disc(12, 13, 2.6, dark)
    e.disc(20, 13, 2.6, dark)
    e.set(16, 17, dark); e.set(15, 18, dark); e.set(17, 18, dark)
    for x in (13, 15, 17, 19):
        e.set(x, 23, dark); e.set(x, 24, dark)
    e.outline(INK)
    return e


def emblem_flare():
    e = Img()
    red, orange, yellow = (200, 40, 24), (240, 120, 24), (248, 200, 64)
    e.disc(16, 20, 10, red, 9)
    e.disc(16, 13, 6.5, red, 9)
    e.disc(16, 21, 7, orange, 6)
    e.disc(16, 16, 4, orange, 6)
    e.disc(16, 22, 3.5, yellow, 4)
    e.glyph('F', 9, 7, INK)
    e.outline(INK)
    return e


def emblem_plasma():
    e = Img()
    blue, white, grey = (40, 64, 160), (232, 232, 240), (150, 156, 176)
    for y in range(3, 30):  # shield
        half = 13 if y < 18 else max(1, 13 - (y - 18) * 13 // 11)
        for x in range(16 - half, 16 + half):
            e.set(x, y, blue if x < 16 else grey)
    e.glyph('P', 9, 7, white)
    e.outline(INK)
    return e


# ------------------------------------------------------------------ gyms

GYMS = {
    'PEWTER_CITY': dict(scheme=scheme_yell, copy_as='gang_pewter_gym_frlg', seed=1,
                        emblems=[emblem_letter('Y', (232, 64, 152), (240, 96, 176))], tag=(232, 64, 152)),
    'CERULEAN_CITY': dict(scheme=scheme_skull, seed=2, emblems=[emblem_skull()], tag=(232, 232, 224)),
    'VERMILION_CITY': dict(scheme=scheme_rocket, seed=3, emblems=[emblem_letter('R', (200, 32, 32), (216, 40, 40))], tag=(216, 40, 40)),
    'CELADON_CITY': dict(scheme=scheme_aqua_magma, seed=4,
                         emblems=[emblem_letter('A', (40, 88, 200), (64, 128, 232)), emblem_letter('M', (192, 40, 24), (232, 72, 40))],
                         tag=(232, 72, 40)),
    'FUCHSIA_CITY': dict(scheme=scheme_plasma, seed=5, emblems=[emblem_plasma()], tag=(64, 96, 200)),
    'SAFFRON_CITY': dict(scheme=scheme_flare, seed=6, emblems=[emblem_flare()], tag=(240, 120, 24)),
    'CINNABAR_ISLAND': dict(scheme=scheme_galactic, seed=7, emblems=[emblem_letter('G', (64, 176, 168), (232, 216, 96))], tag=(64, 192, 184)),
    'VIRIDIAN_CITY': dict(scheme=scheme_giovanni, seed=8, emblems=[emblem_letter('R', (200, 160, 48), (232, 192, 64))], tag=(216, 176, 56)),
}


def headers():
    return open(os.path.join(ROOT, 'src/data/tilesets/headers.h')).read()


def tileset_dir(symbol):
    m = re.search(r'const struct Tileset %s =\s*\{(.*?)\};' % re.escape(symbol), headers(), re.S)
    tiles_sym = re.search(r'\.tiles = (\w+)', m.group(1)).group(1)
    gfx = open(os.path.join(ROOT, 'src/data/tilesets/graphics.h')).read()
    return re.search(r'%s\[\] = INCGFX_U32\("([^"]+)/tiles\.png"' % re.escape(tiles_sym), gfx).group(1)


def ensure_copy(src_symbol, src_dir, name):
    """Pewter: register a copy of its tileset under its own name, so the Saffron Dojo keeps the old look"""
    dst = 'data/tilesets/secondary/' + name
    sym, cname = 'gTileset_GangPewterGymFrlg', 'GangPewterGym_Frlg'
    if sym in headers():
        return sym, dst
    src_block = re.search(r'const struct Tileset %s =\s*\{.*?\};\n' % re.escape(src_symbol), headers(), re.S).group(0)
    p = os.path.join(ROOT, 'src/data/tilesets/headers.h')
    block = re.sub(r'\.tiles = \w+', '.tiles = gTilesetTiles_%s' % cname, src_block)
    block = re.sub(r'\.palettes = \w+', '.palettes = gTilesetPalettes_%s' % cname, block)
    block = re.sub(r'\.metatiles = \w+', '.metatiles = gMetatiles_%s' % cname, block)
    block = re.sub(r'\.metatileAttributes = \w+', '.metatileAttributes = gMetatileAttributes_%s' % cname, block)
    block = block.replace(src_symbol, sym)
    s = open(p).read()
    open(p, 'w').write(s.replace(src_block, src_block + '\n' + block, 1))
    p = os.path.join(ROOT, 'src/data/tilesets/graphics.h')
    s = open(p).read()
    pals = ''.join('    INCGFX_U16("%s/palettes/%02d.pal", ".gbapal"),\n' % (dst, i) for i in range(16))
    s += ('\n// Gangster Outbreak: Pewter gym in Team Yell colours (tools/apocalypse/gyms.py)\n'
          'const u32 gTilesetTiles_%s[] = INCGFX_U32("%s/tiles.png", ".4bpp.fastSmol");\n\n'
          'const u16 gTilesetPalettes_%s[][16] =\n{\n%s};\n' % (cname, dst, cname, pals))
    open(p, 'w').write(s)
    p = os.path.join(ROOT, 'src/data/tilesets/metatiles.h')
    s = open(p).read()
    s += ('const u16 gMetatiles_%s[] = INCBIN_U16("%s/metatiles.bin");\n'
          'const u16 gMetatileAttributes_%s[] = INCBIN_U16("%s/metatile_attributes.bin");\n' % (cname, dst, cname, dst))
    open(p, 'w').write(s)
    p = os.path.join(ROOT, 'include/tilesets.h')
    s = open(p).read()
    anchor = 'extern const struct Tileset %s;\n' % src_symbol
    if anchor in s:
        s = s.replace(anchor, anchor + 'extern const struct Tileset %s;\n' % sym, 1)
    else:
        s = s.replace('#endif', 'extern const struct Tileset %s;\n\n#endif' % sym, 1)
    open(p, 'w').write(s)
    return sym, dst


class Gym:
    def __init__(self, key, cfg):
        self.key, self.cfg = key, cfg
        layouts = json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']
        self.layout = next(l for l in layouts if l and l['id'] == 'LAYOUT_%s_GYM' % key)
        self.orig_symbol = json.loads(git_bytes('data/layouts/layouts.json', BASE_REF).decode())
        self.orig_symbol = next(l for l in self.orig_symbol['layouts'] if l and l['id'] == self.layout['id'])['secondary_tileset']
        self.src = tileset_dir(self.orig_symbol)
        self.dst = self.src
        if cfg.get('copy_as'):
            sym, self.dst = ensure_copy(self.orig_symbol, self.src, cfg['copy_as'])
            self.layout['secondary_tileset'] = sym
        # original data (stage A colours, gym layout before decoration)
        self.tiles = load_tiles(Image.open(BytesIO(git_bytes(self.src + '/tiles.png', STAGE_A_REF))))
        self.tiles_img_pal = Image.open(BytesIO(git_bytes(self.src + '/tiles.png', STAGE_A_REF))).getpalette()
        self.meta = bytearray(git_bytes(self.src + '/metatiles.bin', STAGE_A_REF))
        self.attr = bytearray(git_bytes(self.src + '/metatile_attributes.bin', STAGE_A_REF))
        self.pals = [read_pal(git_bytes('%s/palettes/%02d.pal' % (self.src, i), STAGE_A_REF).decode()) for i in range(16)]
        self.prim_tiles = load_tiles(Image.open(os.path.join(ROOT, PRIM, 'tiles.png')))
        self.prim_meta = open(os.path.join(ROOT, PRIM, 'metatiles.bin'), 'rb').read()
        self.prim_attr = open(os.path.join(ROOT, PRIM, 'metatile_attributes.bin'), 'rb').read()
        self.prim_pals = [read_pal(open(os.path.join(ROOT, PRIM, 'palettes', '%02d.pal' % i)).read()) for i in range(7)]
        lay_bytes = git_bytes(self.layout['blockdata_filepath'], BASE_REF)
        self.w, self.h = self.layout['width'], self.layout['height']
        self.blocks = list(struct.unpack('<%dH' % (self.w * self.h), lay_bytes))
        used = {(e >> 12) for i in range(len(self.meta) // 2) for e in [struct.unpack_from('<H', self.meta, i * 2)[0]]}
        self.free_slot = max(s for s in range(7, 13) if s not in used)
        self.lookup = {t: i for i, t in reversed(list(enumerate(self.tiles)))}

    # --- colours
    def recolour(self):
        scheme = self.cfg['scheme']
        used = {(e >> 12) for i in range(len(self.meta) // 2) for e in [struct.unpack_from('<H', self.meta, i * 2)[0]]}
        for slot in range(7, 13):
            if slot in used:
                self.pals[slot] = [self.pals[slot][0]] + [scheme(c) for c in self.pals[slot][1:]]

    def all_pals(self):
        return self.prim_pals + self.pals[7:13]

    def entries(self, mid):
        if mid < NUM_PRIM:
            return struct.unpack_from('<8H', self.prim_meta, mid * 16)
        return struct.unpack_from('<8H', self.meta, (mid - NUM_PRIM) * 16)

    def behavior(self, mid):
        a = self.prim_attr if mid < NUM_PRIM else self.attr
        return struct.unpack_from('<I', a, (mid % NUM_PRIM) * 4)[0] & 0x1FF

    def tile_px(self, e):
        t = e & 0x3FF
        px = self.prim_tiles[t] if t < NUM_PRIM else self.tiles[t - NUM_PRIM]
        if e & 0x400:
            px = tuple(px[y * 8 + 7 - x] for y in range(8) for x in range(8))
        if e & 0x800:
            px = tuple(px[(7 - y) * 8 + x] for y in range(8) for x in range(8))
        return px

    def render(self, mid):
        pals = self.all_pals()
        img = [[pals[0][0]] * 16 for _ in range(16)]
        for n, e in enumerate(self.entries(mid)):
            slot = (e >> 12) & 0xF
            pal = pals[slot] if slot < 13 else pals[0]
            ox, oy = (n % 2) * 8, ((n % 4) // 2) * 8
            for k, v in enumerate(self.tile_px(e)):
                if v:
                    img[oy + k // 8][ox + k % 8] = pal[v]
        return img

    # --- encoding new metatiles
    def gang_palette(self, floor_mids):
        cols = Counter()
        for mid in floor_mids:
            for row in self.render(mid):
                cols.update(row)
        art = Counter()
        for em in self.cfg['emblems']:
            for row in em.p:
                art.update(c for c in row if c is not None)
        art[self.cfg['tag']] += 999
        for c in (INK, (72, 68, 80), (150, 146, 160), (224, 224, 232)):
            art[c] += 500
        pal = [(0, 0, 0)]
        for c, _ in art.most_common(9) + cols.most_common(10):
            if len(pal) < 16 and all(sum((a - b) ** 2 for a, b in zip(c, p)) > 200 for p in pal[1:]):
                pal.append(c)
        while len(pal) < 16:
            pal.append(INK)
        self.pals[self.free_slot] = pal

    def tile_id(self, px):
        if px not in self.lookup:
            self.lookup[px] = len(self.tiles)
            self.tiles.append(px)
        return NUM_PRIM + self.lookup[px]

    def new_metatile(self, base_mid, layer, behavior=None):
        """base metatile with a 16x16 RGBA layer painted over it (same behaviour unless given)"""
        rgbimg = self.render(base_mid)
        for y in range(16):
            for x in range(16):
                if layer[y][x] is not None:
                    rgbimg[y][x] = layer[y][x]
        pals = self.all_pals()
        entries = []
        for q in range(4):
            ox, oy = (q % 2) * 8, (q // 2) * 8
            cells = [rgbimg[oy + k // 8][ox + k % 8] for k in range(64)]
            best = None
            for slot in range(13):
                err, px = 0, []
                for c in cells:
                    i, e = nearest(pals[slot], c)
                    err += e
                    px.append(i)
                if best is None or err < best[0]:
                    best = (err, slot, tuple(px))
            entries.append(self.tile_id(best[2]) | (best[1] << 12))
        mid = NUM_PRIM + len(self.meta) // 16
        self.meta += struct.pack('<8H', *entries, 0, 0, 0, 0)
        a = self.prim_attr if base_mid < NUM_PRIM else self.attr
        attr = struct.unpack_from('<I', a, (base_mid % NUM_PRIM) * 4)[0] & ~(3 << 29)
        if behavior is not None:
            attr = (attr & ~0x1FF) | behavior
        self.attr += struct.pack('<I', attr)
        return mid

    # --- placement
    def cell(self, x, y):
        return self.blocks[y * self.w + x]

    def walkable(self, x, y):
        return (self.cell(x, y) >> 10) & 3 == 0

    def set_mid(self, x, y, mid):
        v = self.cell(x, y)
        self.blocks[y * self.w + x] = (v & ~0x3FF) | mid

    def prepare(self):
        """team palette from the gym as it was (floors and walls) plus the team colours"""
        mids = [b & 0x3FF for b in self.blocks]
        walk = Counter(mids[i] for i in range(len(mids)) if self.walkable(i % self.w, i // self.w)
                       and self.behavior(mids[i]) == 0)
        self.floor_mid = walk.most_common(1)[0][0]
        walls = Counter(mids[y * self.w + x] for y in range(0, 3) for x in range(self.w)
                        if not self.walkable(x, y) and self.behavior(mids[y * self.w + x]) == 0 and mids[y * self.w + x] != 0x008)
        self.gang_palette([m for m, _ in walk.most_common(2)] + [m for m, _ in walls.most_common(2)])

    def apply_puzzle(self, spec):
        """rebuild the room from the puzzle text, move the people, check that the gym can be solved"""
        beh = puzzle_art.behaviors()
        rows = spec['rows']
        assert len(rows) == self.h and all(len(r) == self.w for r in rows), '%s: puzzle size' % self.key
        tag = self.cfg['tag']
        cache = {}

        def art(ch):
            if ch not in cache:
                layer, behavior, blocks = puzzle_art.tile(ch, tag, spec.get('barrier', 'crates'), beh)
                cache[ch] = (self.new_metatile(self.floor_mid, layer, behavior) if layer is not None else self.floor_mid, blocks)
            return cache[ch]

        orig = list(self.blocks)
        for y, row in enumerate(rows):
            for x, ch in enumerate(row):
                if ch == 'K':
                    continue
                mid, blocks = art(ch)
                col, el = (1, 0) if blocks else (0, 3)
                self.blocks[y * self.w + x] = mid | (col << 10) | (el << 12)
        # people
        path = os.path.join(ROOT, 'data/maps/%s_Gym_Frlg/map.json' % ''.join(p.capitalize() for p in self.key.split('_')))
        data = json.load(open(path))
        for o in data['object_events']:
            name = o['script'].split('_EventScript_')[-1]
            if name in spec.get('objects', {}):
                x, y, move, sight = spec['objects'][name]
                o['x'], o['y'], o['movement_type'] = x, y, move
                o['trainer_sight_or_berry_tree_id'] = str(sight)
        with open(path, 'w') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
            f.write('\n')
        blocked = {(x, y) for y in range(self.h) for x in range(self.w)
                   if rows[y][x] == 'K' and (orig[y * self.w + x] >> 10) & 3}
        people = {(o['x'], o['y']) for o in data['object_events']}
        blocked -= set(spec.get('open_cells', ()))  # e.g. Vermilion's laser gate counts as opened
        return Puzzle(rows, blocked, people).check(spec['door'], spec['leader_front'], self.key)

    def decorate(self):
        rng = random.Random(self.cfg['seed'])
        mids = [b & 0x3FF for b in self.blocks]
        walk = Counter(mids[i] for i in range(len(mids)) if self.walkable(i % self.w, i // self.w)
                       and self.behavior(mids[i]) == 0)
        floors = [m for m, _ in walk.most_common(3)]
        floor_any = {m for m, _ in walk.most_common(8)}
        walls = Counter(mids[y * self.w + x] for y in range(0, 3) for x in range(self.w)
                        if not self.walkable(x, y) and self.behavior(mids[y * self.w + x]) == 0 and mids[y * self.w + x] != 0x008)
        # floor emblems on 2x2 spots of plain floor, spread over the room
        spots = []
        for y in range(2, self.h - 3):
            for x in range(1, self.w - 2):
                cells = [(x, y), (x + 1, y), (x, y + 1), (x + 1, y + 1)]
                ms = {self.cell(*c) & 0x3FF for c in cells}
                if all(self.walkable(*c) for c in cells) and ms <= floor_any \
                        and all(self.behavior(self.cell(*c) & 0x3FF) == 0 for c in cells):
                    spots.append((x, y))
        n = 2 if self.w * self.h < 300 else 3 if self.w * self.h < 500 else 4
        chosen = []
        door = (self.w // 2, self.h - 2)
        for _ in range(n):
            best = max(spots, key=lambda p: min([abs(p[0] - q[0]) + abs(p[1] - q[1]) for q in chosen + [door]] or [99])
                       + rng.random(), default=None) if spots else None
            if best is None:
                break
            chosen.append(best)
            spots = [p for p in spots if abs(p[0] - best[0]) > 2 or abs(p[1] - best[1]) > 2]
        cache = {}
        for k, (x, y) in enumerate(chosen):
            em = self.cfg['emblems'][k % len(self.cfg['emblems'])]
            worn = Img()
            worn.p = [row[:] for row in em.p]
            worn.worn(self.cfg['seed'] * 10 + k)
            for qy in range(2):
                for qx in range(2):
                    base = self.cell(x + qx, y + qy) & 0x3FF
                    part = [r[qx * 16:qx * 16 + 16] for r in worn.p[qy * 16:qy * 16 + 16]]
                    self.set_mid(x + qx, y + qy, self.new_metatile(base, part))
        # small emblems / tags on the back wall
        wall_cells = [(x, y) for y in range(0, 3) for x in range(1, self.w - 1)
                      if not self.walkable(x, y) and (self.cell(x, y) & 0x3FF) in dict(walls.most_common(3))]
        rng.shuffle(wall_cells)
        placed = []
        for x, y in wall_cells:
            if len(placed) >= max(2, self.w // 5):
                break
            if any(abs(x - px) < 3 for px, _ in placed):
                continue
            base = self.cell(x, y) & 0x3FF
            if (base, len(placed) % 2) not in cache:
                if len(placed) % 2 == 0:
                    small = self.cfg['emblems'][len(placed) // 2 % len(self.cfg['emblems'])].scaled(14, 14)
                    layer = [[None] * 16 for _ in range(16)]
                    for yy in range(14):
                        for xx in range(14):
                            layer[yy + 1][xx + 1] = small.p[yy][xx]
                else:
                    layer = spray_tag(self.cfg['tag'], self.cfg['seed'] + x)
                cache[(base, len(placed) % 2)] = self.new_metatile(base, layer)
            self.set_mid(x, y, cache[(base, len(placed) % 2)])
            placed.append((x, y))
        return len(chosen), len(placed)

    def save(self):
        assert len(self.tiles) <= MAX_SEC, '%s: too many tiles (%d)' % (self.key, len(self.tiles))
        d = os.path.join(ROOT, self.dst)
        os.makedirs(os.path.join(d, 'palettes'), exist_ok=True)
        if self.dst != self.src:
            for f in os.listdir(os.path.join(ROOT, self.src)):
                if f.startswith('anim'):
                    shutil.copytree(os.path.join(ROOT, self.src, f), os.path.join(d, f), dirs_exist_ok=True)
        w = 16
        rows = (len(self.tiles) + w - 1) // w
        img = Image.new('P', (w * 8, rows * 8))
        img.putpalette(self.tiles_img_pal)
        flat = [0] * (img.width * img.height)
        for i, t in enumerate(self.tiles):
            tx, ty = (i % w) * 8, (i // w) * 8
            for k, v in enumerate(t):
                flat[(ty + k // 8) * img.width + tx + k % 8] = v
        img.putdata(flat)
        img.save(os.path.join(d, 'tiles.png'), bits=4)
        open(os.path.join(d, 'metatiles.bin'), 'wb').write(self.meta)
        open(os.path.join(d, 'metatile_attributes.bin'), 'wb').write(self.attr)
        for i, pal in enumerate(self.pals):
            with open(os.path.join(d, 'palettes', '%02d.pal' % i), 'w', newline='\r\n') as f:
                f.write('JASC-PAL\n0100\n16\n' + ''.join('%d %d %d\n' % c for c in pal))
        open(os.path.join(ROOT, self.layout['blockdata_filepath']), 'wb').write(struct.pack('<%dH' % len(self.blocks), *self.blocks))


def spray_tag(col, seed):
    rng = random.Random(seed)
    layer = [[None] * 16 for _ in range(16)]
    drip = tuple(max(0, v - 60) for v in col)
    x, y = 1, rng.randint(5, 9)
    while x < 15:
        nx, ny = x + rng.randint(1, 2), max(3, min(12, y + rng.choice([-3, -2, 2, 3])))
        n = max(abs(nx - x), abs(ny - y), 1)
        for i in range(n + 1):
            px, py = round(x + (nx - x) * i / n), round(y + (ny - y) * i / n)
            for dy in (0, 1):
                if 0 <= px < 16 and 0 <= py + dy < 16:
                    layer[py + dy][px] = col
        if rng.random() < 0.3 and ny + 3 < 16 and nx < 16:
            layer[ny + 2][nx] = drip
            layer[ny + 3][nx] = drip
        x, y = nx, ny
    return layer


from io import BytesIO  # noqa: E402


def main():
    layouts_path = os.path.join(ROOT, 'data/layouts/layouts.json')
    gyms = []
    for key, cfg in GYMS.items():
        g = Gym(key, cfg)
        g.recolour()
        g.prepare()
        if key in GYM_PUZZLES:
            print('%-16s puzzle: %d steps to the leader, %d back' % (key, *g.apply_puzzle(GYM_PUZZLES[key])))
        n_em, n_tags = g.decorate()
        g.save()
        gyms.append(g)
        print('%-16s emblems %d, wall tags %d, tiles %d/%d, deco palette slot %d'
              % (key, n_em, n_tags, len(g.tiles), MAX_SEC, g.free_slot))
    data = json.load(open(layouts_path))
    for lay in data['layouts']:
        for g in gyms:
            if lay and lay['id'] == g.layout['id']:
                lay['secondary_tileset'] = g.layout['secondary_tileset']
    with open(layouts_path, 'w') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
        f.write('\n')
    if '--preview' in sys.argv:
        out = sys.argv[sys.argv.index('--preview') + 1]
        os.makedirs(out, exist_ok=True)
        import render_map
        for g in gyms:
            render_map.render(g.layout['id'], os.path.join(out, g.key + '.png'))


if __name__ == '__main__':
    main()
