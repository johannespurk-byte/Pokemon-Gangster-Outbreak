#!/usr/bin/env python3
"""Gangster Outbreak: the houses of Pallet Town become gang hideouts.

The player's house (1F/2F) is the crew's safehouse, Silver's house is his crew's clubhouse and Oak's lab is
looted and occupied. The rooms keep their furniture; this tool paints props over it: boarded-up windows, gang tags
and cracks on the walls, crates, barrels, sandbags, tires, rubble, debris and stains on the floor.

New metatiles are appended to the secondary tilesets of the rooms (the original metatiles keep their ids, so the
other houses on the same tilesets don't change). New pixels use a deco palette in slot 11, which none of those
tilesets uses, and every 8x8 tile is re-encoded with whichever of the 13 palettes fits it best.
Like the other apocalypse tools it starts from the original files in git (BASE_REF), so it can be re-run.

Usage: pallet_houses.py [--preview DIR]
"""
import json, os, struct, subprocess, sys
from collections import Counter
from io import BytesIO
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import deco  # noqa: E402
import interiors  # noqa: E402
from deco import Canvas, DG, G, LG, PC, DR, R, LR, Y, RED, SOOT, DIRT, GL  # noqa: E402

ROOT = interiors.ROOT
BASE_REF = 'f8e876dd'
PRIM = 'data/tilesets/primary/building_frlg'
NUM_PRIM = 640
MAX_SEC_TILES = 384
MAX_SEC_META = 384
DECO_SLOT = 11
LAYER_NORMAL = 0

GB1 = 'data/tilesets/secondary/generic_building_1_frlg'
GB2 = 'data/tilesets/secondary/generic_building_2_frlg'
LAB = 'data/tilesets/secondary/lab_frlg'

PLANK, PLANK_D, NAIL = (124, 88, 52), (84, 56, 32), (40, 32, 24)


def git_bytes(path):
    return subprocess.run(['git', 'show', '%s:%s' % (BASE_REF, path)], cwd=ROOT, capture_output=True, check=True).stdout


def read_pal(text):
    return [tuple(int(v) for v in l.split()) for l in text.replace('\r', '').split('\n')[3:19]]


# ------------------------------------------------------------------ extra props

def boards(seed):
    """32x32: planks nailed across a window"""
    c = Canvas(32, 32, seed)
    rgba = [[None] * 32 for _ in range(32)]
    for y0, tilt in ((9, 2), (17, -2), (24, 1)):
        for x in range(1, 31):
            yc = y0 + (x - 16) * tilt // 16
            for dy in range(4):
                rgba[yc + dy][x] = PLANK if dy < 3 else PLANK_D
            if x % 7 == 3:
                rgba[yc + 1][x] = PLANK_D
        for nx in (3, 28):
            rgba[y0 + (nx - 16) * tilt // 16 + 1][nx] = NAIL
    for _ in range(10):  # wood grain
        x, y = c.rng.randint(2, 29), c.rng.randint(8, 27)
        if rgba[y][x] == PLANK:
            rgba[y][x] = PLANK_D
    return rgba


def cash(seed):
    """a bag of cash and loose bills"""
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14.5, 6, 1.4, SOOT)
    c.ellipse(8, 10, 5, 4.5, DIRT)
    c.rect(6, 3, 10, 6, DIRT)
    c.line(5, 6, 11, 6, DR)
    c.ellipse(7, 9, 2, 2, PC)
    c.set(7, 9, Y)
    c.set(7, 10, Y)
    for x, y in ((1, 13), (12, 12), (13, 14)):
        c.rect(x, y, x + 2, y + 1, LG)
        c.set(x + 1, y, G)
    c.outline()
    return c


PROPS = dict(interiors.PROPS)
PROPS.update({'cash': (cash, 31), 'crates_b': (interiors.crates, 33), 'barrel_b': (deco.barrel, 35),
              'tag_c': (interiors.wall_tag, 37), 'tag_d': (interiors.wall_tag, 41), 'tag_y': (deco.graffiti, 43),
              'rubble_b': (deco.rubble, 45), 'debris_c': (interiors.debris, 47)})


# ------------------------------------------------------------------ tileset

class Tileset:
    def __init__(self, sec):
        self.sec = sec
        self.prim_tiles = interiors.load_tiles(Image.open(os.path.join(ROOT, PRIM, 'tiles.png')))
        self.prim_meta = open(os.path.join(ROOT, PRIM, 'metatiles.bin'), 'rb').read()
        self.prim_attr = open(os.path.join(ROOT, PRIM, 'metatile_attributes.bin'), 'rb').read()
        self.prim_pals = [read_pal(open(os.path.join(ROOT, PRIM, 'palettes', '%02d.pal' % i)).read()) for i in range(7)]
        self.tiles = interiors.load_tiles(Image.open(BytesIO(git_bytes(sec + '/tiles.png'))))
        meta = git_bytes(sec + '/metatiles.bin')
        attr = git_bytes(sec + '/metatile_attributes.bin')
        self.meta = [list(struct.unpack_from('<8H', meta, i * 16)) for i in range(len(meta) // 16)]
        self.attr = [struct.unpack_from('<I', attr, i * 4)[0] for i in range(len(attr) // 4)]
        self.pals = [read_pal(git_bytes('%s/palettes/%02d.pal' % (sec, i)).decode()) for i in range(16)]
        self.lookup = {t: i for i, t in enumerate(self.tiles)}
        self.names = {}
        self.deco_done = False

    # rendering
    def all_pals(self):
        return self.prim_pals + self.pals[7:13]

    def entries(self, mid):
        if mid < NUM_PRIM:
            return list(struct.unpack_from('<8H', self.prim_meta, mid * 16))
        return self.meta[mid - NUM_PRIM]

    def attr_of(self, mid):
        if mid < NUM_PRIM:
            return struct.unpack_from('<I', self.prim_attr, mid * 4)[0]
        return self.attr[mid - NUM_PRIM]

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
            pal = pals[(e >> 12) & 0xF] if ((e >> 12) & 0xF) < 13 else pals[0]
            ox, oy = (n % 2) * 8, ((n % 4) // 2) * 8
            for k, v in enumerate(self.tile_px(e)):
                if v:
                    img[oy + k // 8][ox + k % 8] = pal[v]
        return img

    # deco palette
    def set_deco_palette(self, base_mids):
        cols = Counter()
        for mid in base_mids:
            for row in self.render(mid):
                cols.update(row)
        base = [c for c, _ in cols.most_common(8)]
        deco_cols = [deco.DECO_PAL[i] for i in (DG, G, LG, DR, R, LR, Y, RED, DIRT, SOOT)] + [PLANK, PLANK_D]
        pal = [(255, 0, 255)] + deco_cols
        for c in base:
            if len(pal) < 16 and all(sum((a - b) ** 2 for a, b in zip(c, p)) > 300 for p in pal[1:]):
                pal.append(c)
        while len(pal) < 16:
            pal.append(deco.DECO_PAL[GL])
        self.pals[DECO_SLOT] = pal

    # encoding
    def tile_id(self, px):
        if px not in self.lookup:
            self.lookup[px] = len(self.tiles)
            self.tiles.append(px)
        return NUM_PRIM + self.lookup[px]

    def encode(self, rgb, attr, name):
        if name in self.names:
            return self.names[name]
        pals = self.all_pals()
        entries = []
        for q in range(4):
            ox, oy = (q % 2) * 8, (q // 2) * 8
            cells = [rgb[oy + k // 8][ox + k % 8] for k in range(64)]
            best = None
            for slot in range(13):
                if slot >= 7 and all(c == pals[slot][1] for c in pals[slot][1:]):
                    continue  # unused (all black) palette
                err, px = 0, []
                for c in cells:
                    i, e = interiors.nearest(pals[slot], c)
                    err += e
                    px.append(i)
                if best is None or err < best[0]:
                    best = (err, slot, tuple(px))
            entries.append(self.tile_id(best[2]) | (best[1] << 12))
        entries += [0, 0, 0, 0]
        mid = NUM_PRIM + len(self.meta)
        self.meta.append(entries)
        self.attr.append((attr & ~(3 << 29)) | (LAYER_NORMAL << 29))
        self.names[name] = mid
        return mid

    def overlay(self, name, base_mid, rgba):
        key = '%s@%x' % (name, base_mid)
        rgb = self.render(base_mid)
        for y in range(16):
            for x in range(16):
                if rgba[y][x] is not None:
                    rgb[y][x] = rgba[y][x]
        return self.encode(rgb, self.attr_of(base_mid), key)

    def prop(self, name, base_mid):
        fn, seed = PROPS[name]
        return self.overlay(name, base_mid, interiors.canvas_rgba(fn(seed)))

    def save(self):
        assert len(self.tiles) <= MAX_SEC_TILES, '%s: too many tiles (%d)' % (self.sec, len(self.tiles))
        assert len(self.meta) <= MAX_SEC_META, '%s: too many metatiles (%d)' % (self.sec, len(self.meta))
        w = 16
        rows = (len(self.tiles) + w - 1) // w
        img = Image.new('P', (w * 8, rows * 8))
        img.putpalette([v for c in self.pals[7] for v in c])
        flat = [0] * (img.width * img.height)
        for i, t in enumerate(self.tiles):
            tx, ty = (i % w) * 8, (i // w) * 8
            for k, v in enumerate(t):
                flat[(ty + k // 8) * img.width + tx + k % 8] = v
        img.putdata(flat)
        img.save(os.path.join(ROOT, self.sec, 'tiles.png'), bits=4)
        open(os.path.join(ROOT, self.sec, 'metatiles.bin'), 'wb').write(b''.join(struct.pack('<8H', *m) for m in self.meta))
        open(os.path.join(ROOT, self.sec, 'metatile_attributes.bin'), 'wb').write(b''.join(struct.pack('<I', a) for a in self.attr))
        with open(os.path.join(ROOT, self.sec, 'palettes', '%02d.pal' % DECO_SLOT), 'w', newline='\r\n') as f:
            f.write('JASC-PAL\n0100\n16\n' + ''.join('%d %d %d\n' % c for c in self.pals[DECO_SLOT]))


# ------------------------------------------------------------------ rooms

def layout_info(layout_id):
    for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']:
        if l.get('id') == layout_id:
            return l
    raise KeyError(layout_id)


class Room:
    def __init__(self, layout_id, ts):
        self.info = layout_info(layout_id)
        self.ts = ts
        self.w, self.h = self.info['width'], self.info['height']
        data = git_bytes(self.info['blockdata_filepath'])
        self.cells = list(struct.unpack('<%dH' % (self.w * self.h), data))

    def mid(self, x, y):
        return self.cells[y * self.w + x] & 0x3FF

    def put(self, x, y, mid, block=None):
        v = self.cells[y * self.w + x]
        if block is not None:
            v = (v & ~0xC00) | ((1 if block else 0) << 10)
        self.cells[y * self.w + x] = (v & ~0x3FF) | mid

    def prop(self, x, y, name, block):
        self.put(x, y, self.ts.prop(name, self.mid(x, y)), block)

    def big(self, x, y, name, rgba32):
        """32x32 picture over the 2x2 block at x,y (keeps collision)"""
        for qy in range(2):
            for qx in range(2):
                part = [r[qx * 16:qx * 16 + 16] for r in rgba32[qy * 16:qy * 16 + 16]]
                base = self.mid(x + qx, y + qy)
                self.put(x + qx, y + qy, self.ts.overlay('%s_%d%d' % (name, qx, qy), base, part))

    def save(self):
        open(os.path.join(ROOT, self.info['blockdata_filepath']), 'wb').write(struct.pack('<%dH' % len(self.cells), *self.cells))


# Decals (cracks, debris, stains, rubble) stay walkable; furniture-like props (crates, barrels, sandbags, tires,
# cash) block. Coordinates are chosen so doors, stairs, people and the paths between them stay free.
DECALS = {'crack_a', 'crack_b', 'crack_c', 'debris_a', 'debris_b', 'debris_c', 'stain', 'rubble', 'rubble_b'}

ROOMS = {
    'LAYOUT_PALLET_TOWN_PLAYERS_HOUSE_1F_FRLG': (GB1, {
        'boards': [(7, 0)],
        'wall': [(9, 1, 'tag_a'), (10, 1, 'tag_y'), (5, 1, 'wall_crack')],
        'floor': [(1, 3, 'crates'), (2, 3, 'crates_b'), (1, 4, 'cash'), (6, 8, 'sandbags'), (2, 8, 'sandbags'),
                  (12, 8, 'tires'), (11, 8, 'barrel'),
                  (3, 5, 'crack_a'), (10, 4, 'debris_a'), (7, 7, 'stain'), (9, 8, 'debris_b'), (11, 5, 'crack_b'),
                  (2, 6, 'rubble')],
    }),
    'LAYOUT_PALLET_TOWN_PLAYERS_HOUSE_2F_FRLG': (GB1, {
        'wall': [(6, 1, 'tag_c'), (7, 1, 'tag_x'), (9, 1, 'wall_crack'), (10, 1, 'tag_d')],
        'floor': [(11, 7, 'crates'), (11, 8, 'cash'), (10, 8, 'barrel_b'),
                  (3, 8, 'debris_a'), (9, 5, 'stain'), (2, 7, 'crack_c'), (10, 6, 'debris_c')],
    }),
    'LAYOUT_PALLET_TOWN_RIVALS_HOUSE': (GB2, {
        'boards': [(6, 0)],
        'wall': [(4, 1, 'tag_a'), (8, 1, 'tag_b'), (10, 1, 'tag_y'), (1, 1, 'wall_crack')],
        'floor': [(1, 3, 'barrel_fire'), (2, 3, 'tires'), (11, 3, 'crates'), (12, 3, 'crates_b'),
                  (7, 8, 'sandbags'), (8, 8, 'sandbags'), (12, 6, 'barrel'),
                  (2, 5, 'rubble'), (10, 4, 'debris_a'), (6, 7, 'stain'), (1, 6, 'crack_a'), (11, 7, 'debris_b'),
                  (3, 7, 'crack_b')],
    }),
    'LAYOUT_PALLET_TOWN_PROFESSOR_OAKS_LAB': (LAB, {
        'floor': [(12, 5, 'crates'), (12, 6, 'crates_b'), (11, 6, 'barrel'), (0, 6, 'barrel_b'),
                  (9, 10, 'sandbags'), (10, 10, 'sandbags'), (4, 3, 'rubble'), (10, 6, 'rubble_b'),
                  (3, 6, 'debris_a'), (7, 4, 'crack_a'), (4, 10, 'crack_b'), (2, 12, 'stain'), (9, 11, 'debris_b'),
                  (11, 12, 'debris_c'), (6, 6, 'crack_c'), (8, 3, 'stain')],
    }),
}


def main():
    preview = sys.argv[sys.argv.index('--preview') + 1] if '--preview' in sys.argv else None
    tilesets, rooms = {}, []
    for layout_id, (sec, plan) in ROOMS.items():
        if sec not in tilesets:
            tilesets[sec] = Tileset(sec)
        ts = tilesets[sec]
        room = Room(layout_id, ts)
        if not ts.deco_done:
            floor = Counter(room.mid(x, y) for y in range(room.h) for x in range(room.w)).most_common(2)
            ts.set_deco_palette([m for m, _ in floor] + [0x028])
            ts.deco_done = True
        for x, y in plan.get('boards', []):
            room.big(x, y, 'boards', boards(x))
        for x, y, name in plan.get('wall', []):
            room.prop(x, y, name, None)
        for x, y, name in plan.get('floor', []):
            room.prop(x, y, name, name not in DECALS)
        rooms.append(room)
    for ts in tilesets.values():
        ts.save()
        print('%s: %d tiles, %d metatiles' % (os.path.basename(ts.sec), len(ts.tiles), len(ts.meta)))
    for room in rooms:
        room.save()
    if preview:
        os.makedirs(preview, exist_ok=True)
        for layout_id in ROOMS:
            subprocess.run([sys.executable, os.path.join(ROOT, 'tools/apocalypse/render_map.py'), layout_id,
                            os.path.join(preview, layout_id + '.png')], check=True, stderr=subprocess.DEVNULL)


if __name__ == '__main__':
    main()
