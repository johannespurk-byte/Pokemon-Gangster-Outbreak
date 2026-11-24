#!/usr/bin/env python3
"""Gangster Outbreak: ruined Pokémon Center (1F) and Mart interiors.

Builds the secondary tileset data/tilesets/secondary/gang_interior_frlg from the FRLG Pokémon Center tileset:
every metatile keeps its id (the escalator animation and the heal machine rely on fixed ids), the pieces only
the 2F needs (cable club machines, union room doors) are dropped to free tiles, and new metatiles are appended:
cracks, rubble, scrap heaps, barrels (some burning), crates, sandbags, tires, graffiti, a sandbag counter for the
heal-bot and working / smashed vending machines.
New pixels are drawn with the deco palette (palette slot 11, which the 1F does not use) and every 8x8 tile is
re-encoded with whichever palette fits it best, so props blend into the floor and walls.

Then it writes the new, larger layouts LAYOUT_POKEMON_CENTER_1F_FRLG and LAYOUT_MART_FRLG (both now use the new
tileset) and moves the warps and people of every Center / Mart map to the new rooms.
Like the other apocalypse tools it starts from the original files in git (BASE_REF), so it can be re-run.

Usage: interiors.py [--preview DIR]
"""
import json, os, struct, subprocess, sys
from collections import Counter, deque
from io import BytesIO
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import deco  # noqa: E402
from deco import Canvas, O, DG, G, LG, PC, DR, R, LR, Y, DRED, RED, DGL, GL, SOOT, DIRT  # noqa: E402

deco.DECO_PAL = list(deco.DECO_PAL)
deco.DECO_PAL[DGL] = deco.LEGACY_DARK_GLASS  # interiors keep dark glass in slot 12

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE_REF = 'ddfe1f3d'
STAGE_A_REF = '862c4b3d'  # Center palettes after recolor.py (stage A); this tool dulls them further
PRIM = 'data/tilesets/primary/building_frlg'
SRC = 'data/tilesets/secondary/pokemon_center_frlg'
DST = 'data/tilesets/secondary/gang_interior_frlg'
NUM_PRIM = 640
MAX_SEC = 384
DECO_SLOT = 11
PC1F, MART = 'LAYOUT_POKEMON_CENTER_1F_FRLG', 'LAYOUT_MART_FRLG'
LAYER_NORMAL = 0

# escalator animation frames (src/fldeff_escalator.c) must stay in the tileset
ESCALATOR = [0x2D0, 0x30A, 0x308, 0x2D8, 0x312, 0x310, 0x2D1, 0x30B, 0x309, 0x2D9, 0x313, 0x311,
             0x2EB, 0x31E, 0x31C, 0x2E3, 0x316, 0x314, 0x2E4, 0x317, 0x315]
FLOOR, SHADOW, WALL_TOP, WALL_LOW = 0x281, 0x2B7, 0x284, 0x28C


def git_bytes(path, ref=BASE_REF):
    return subprocess.run(['git', 'show', '%s:%s' % (ref, path)], cwd=ROOT, capture_output=True, check=True).stdout


def read_pal(text):
    return [tuple(int(v) for v in l.split()) for l in text.replace('\r', '').split('\n')[3:19]]


def load_tiles(img):
    w = img.width // 8
    px = img.load()
    return [tuple(px[(t % w) * 8 + x, (t // w) * 8 + y] for y in range(8) for x in range(8))
            for t in range(w * (img.height // 8))]


class Tilesets:
    """Primary building tileset + the Pokémon Center secondary tileset, as RGB renderers and an encoder."""

    def __init__(self):
        self.prim_tiles = load_tiles(Image.open(os.path.join(ROOT, PRIM, 'tiles.png')))
        self.prim_meta = open(os.path.join(ROOT, PRIM, 'metatiles.bin'), 'rb').read()
        self.prim_attr = open(os.path.join(ROOT, PRIM, 'metatile_attributes.bin'), 'rb').read()
        self.prim_pals = [read_pal(open(os.path.join(ROOT, PRIM, 'palettes', '%02d.pal' % i)).read()) for i in range(7)]
        # the Center tileset as it is now (recoloured by recolor.py in stage A)
        self.src_tiles = load_tiles(Image.open(os.path.join(ROOT, SRC, 'tiles.png')))
        self.src_meta = open(os.path.join(ROOT, SRC, 'metatiles.bin'), 'rb').read()
        self.src_attr = open(os.path.join(ROOT, SRC, 'metatile_attributes.bin'), 'rb').read()
        self.sec_pals = [read_pal(git_bytes('%s/palettes/%02d.pal' % (SRC, i), STAGE_A_REF).decode()) for i in range(16)]

    def pals(self):
        return self.prim_pals + self.sec_pals[7:13]

    def entries(self, mid):
        if mid < NUM_PRIM:
            return list(struct.unpack_from('<8H', self.prim_meta, mid * 16))
        return list(struct.unpack_from('<8H', self.src_meta, (mid - NUM_PRIM) * 16))

    def attr(self, mid):
        if mid < NUM_PRIM:
            return struct.unpack_from('<I', self.prim_attr, mid * 4)[0]
        return struct.unpack_from('<I', self.src_attr, (mid - NUM_PRIM) * 4)[0]

    def tile_px(self, e):
        t = e & 0x3FF
        px = self.prim_tiles[t] if t < NUM_PRIM else self.src_tiles[t - NUM_PRIM]
        if e & 0x400:
            px = tuple(px[y * 8 + 7 - x] for y in range(8) for x in range(8))
        if e & 0x800:
            px = tuple(px[(7 - y) * 8 + x] for y in range(8) for x in range(8))
        return px

    def render(self, mid):
        """16x16 list of RGB (bottom layer, then top layer over it)"""
        pals = self.pals()
        img = [[pals[0][0]] * 16 for _ in range(16)]
        for n, e in enumerate(self.entries(mid)):
            pal = pals[(e >> 12) & 0xF] if ((e >> 12) & 0xF) < 13 else pals[0]
            ox, oy = (n % 2) * 8, ((n % 4) // 2) * 8
            for k, v in enumerate(self.tile_px(e)):
                if v:
                    img[oy + k // 8][ox + k % 8] = pal[v]
        return img


def nearest(pal, rgb, start=1):
    best, err = start, 1e18
    for i in range(start, 16):
        c = pal[i]
        d = (c[0] - rgb[0]) ** 2 + (c[1] - rgb[1]) ** 2 + (c[2] - rgb[2]) ** 2
        if d < err:
            best, err = i, d
    return best, err


# ------------------------------------------------------------------ new props (16x16 unless noted)

def debris(seed):
    c = Canvas(16, 16, seed)
    for _ in range(9):
        x, y = c.rng.randint(1, 14), c.rng.randint(2, 14)
        col = c.rng.choice([G, LG, DG, PC])
        c.set(x, y, col)
        if c.rng.random() < 0.5:
            c.set(x + 1, y, col)
            c.set(x, y + 1, SOOT)
    for _ in range(2):  # torn paper
        x, y = c.rng.randint(2, 11), c.rng.randint(3, 12)
        c.rect(x, y, x + 2, y + 1, PC)
        c.set(x + 2, y + 1, LG)
    return c


def stain(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8 + c.rng.uniform(-1, 1), 9, 6, 3.5, SOOT)
    c.ellipse(5, 11, 3, 2, SOOT)
    c.ellipse(9, 8, 2.5, 1.2, DG)
    c.set(7, 8, G)
    return c


def crates(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14.5, 7.5, 1.5, SOOT)
    for (x0, y0, x1, y1) in ((1, 7, 9, 14), (8, 9, 14, 14), (3, 1, 10, 7)):
        c.rect(x0, y0, x1, y1, DIRT)
        c.rect(x0, y0, x1, y0, LR)
        c.rect(x0, y0, x0, y1, LR)
        c.line(x0, y0, x1, y1, DR)           # cross brace
        c.line(x0, y1, x1, y0, DR)
        c.rect(x1, y0, x1, y1, DR)
    c.speckle([DR, SOOT], (DIRT,), 0.08)
    c.outline()
    return c


def barrel_fire(seed):
    c = deco.barrel(seed)
    for y in range(0, 4):  # no smoke: flames instead
        for x in range(16):
            if c.p[y][x] in (DG, G):
                c.p[y][x] = 0
    flames = [(5, 3, Y), (7, 1, Y), (9, 2, Y), (11, 3, Y), (6, 2, LR), (8, 0, LR), (10, 1, LR), (8, 3, RED), (6, 3, RED), (10, 3, RED)]
    for x, y, col in flames:
        c.set(x, y, col)
        c.set(x, y + 1, col if col != Y else LR)
    c.ellipse(8, 4, 3.5, 1, Y)
    return c


def counter_sandbags(seed):
    """sandbags stacked on the lower half of the counter front"""
    c = Canvas(16, 16, seed)
    for row, (y, off) in enumerate(((10, 0), (6, 4))):
        for x in range(-4 + off, 16, 8):
            c.ellipse(x + 4, y + 2, 4, 2.4, DIRT)
            c.ellipse(x + 3.5, y + 1.2, 2.6, 1.1, PC)
            c.line(x + 1, y + 4, x + 7, y + 4, SOOT)
    for y in range(16):
        for x in range(16):
            if c.p[y][x] == DIRT and c.rng.random() < 0.12:
                c.p[y][x] = SOOT
    c.outline()
    return c


def wall_tag(seed):
    """spray-painted gang tag for the wall"""
    c = Canvas(16, 16, seed)
    col = c.rng.choice([RED, Y])
    x, y = 1, c.rng.randint(5, 9)
    while x < 15:
        nx, ny = x + c.rng.randint(1, 2), max(3, min(12, y + c.rng.choice([-3, -2, 2, 3])))
        c.line(x, y, nx, ny, col)
        c.line(x, y + 1, nx, ny + 1, col)
        if c.rng.random() < 0.3:
            c.set(nx, ny + 2, DRED if col == RED else LR)
            c.set(nx, ny + 3, DRED if col == RED else LR)
        x, y = nx, ny
    return c


def wall_crack(seed):
    c = Canvas(16, 16, seed)
    x, y = c.rng.randint(4, 11), 0
    while y < 16:
        nx, ny = max(1, min(14, x + c.rng.choice([-1, 0, 1]))), y + c.rng.choice([1, 2])
        c.line(x, y, nx, ny, SOOT)
        if c.rng.random() < 0.3:
            c.line(nx, ny, nx + c.rng.choice([-3, 3]), ny + 2, DG)
        x, y = nx, ny
    return c


def vending(broken, seed):
    """16x32 vending machine drawn from the clerk overworld sprite (the shop machine the player talks to)"""
    im = Image.open(os.path.join(ROOT, 'graphics/object_events/pics/people/clerk.png'))
    pal = im.getpalette()
    src = im.crop((0, 0, 16, 32)).load()
    rgba = [[None] * 16 for _ in range(32)]
    for y in range(32):
        for x in range(16):
            v = src[x, y]
            if v:
                rgba[y][x] = tuple(pal[v * 3:v * 3 + 3])
    if broken:
        c = Canvas(16, 32, seed)
        for y in range(32):
            for x in range(16):
                if rgba[y][x] is not None and (sum(rgba[y][x]) > 520 or (3 <= y <= 12 and 3 <= x <= 12)):
                    c.p[y][x] = DGL  # dark, smashed display
        for _ in range(4):  # cracks in the glass
            x, y = c.rng.randint(4, 11), c.rng.randint(4, 11)
            c.line(x, y, x + c.rng.randint(-3, 3), y + c.rng.randint(-3, 3), GL)
        for _ in range(6):  # dents and soot
            x, y = c.rng.randint(2, 13), c.rng.randint(13, 28)
            c.set(x, y, SOOT)
        c.set(c.rng.randint(4, 11), 2, Y)  # spark
        overlay = c
    else:
        overlay = None
    return rgba, overlay


PROPS = {  # name -> (draw function, seed)
    'crack_a': (deco.crack, 3), 'crack_b': (deco.crack, 11), 'crack_c': (deco.crack, 21),
    'debris_a': (debris, 1), 'debris_b': (debris, 7), 'stain': (stain, 2),
    'rubble': (deco.rubble, 4), 'barrel': (deco.barrel, 5), 'barrel_fire': (barrel_fire, 6),
    'crates': (crates, 8), 'sandbags': (deco.sandbags, 9), 'tires': (deco.tires, 10),
    'tag_a': (wall_tag, 12), 'tag_b': (wall_tag, 19), 'tag_x': (deco.graffiti, 13), 'wall_crack': (wall_crack, 14),
    'counter_bags': (counter_sandbags, 15),
}


def grime(seed):
    """soot speckles and dirt smears for plain floor tiles"""
    c = Canvas(16, 16, seed)
    for _ in range(c.rng.randint(1, 2)):
        c.ellipse(c.rng.uniform(3, 13), c.rng.uniform(3, 13), c.rng.uniform(2.5, 5), c.rng.uniform(1.2, 2.5), DIRT)
    for y in range(16):
        for x in range(16):
            if c.p[y][x] == DIRT and c.rng.random() < 0.45:
                c.p[y][x] = 0
            elif c.rng.random() < 0.05:
                c.p[y][x] = c.rng.choice([SOOT, DG])
    return c


def scorch(seed):
    """burnt patch on the floor"""
    c = Canvas(16, 16, seed)
    c.ellipse(8, 8, 7, 5, DG)
    c.ellipse(8, 8, 5, 3.5, SOOT)
    for y in range(16):
        for x in range(16):
            if c.p[y][x] == DG and c.rng.random() < 0.35:
                c.p[y][x] = 0
    c.set(7, 7, DR); c.set(9, 8, DR)
    return c


PROPS.update({'grime_a': (grime, 31), 'grime_b': (grime, 32), 'grime_c': (grime, 33), 'grime_d': (grime, 34),
              'scorch': (scorch, 35)})


def grimy(rgb, f=0.82):
    """dull, darken and warm a palette colour (smoke and dust)"""
    r, g, b = rgb
    l = 0.3 * r + 0.59 * g + 0.11 * b
    r, g, b = (r * 0.62 + l * 0.38, g * 0.62 + l * 0.38, b * 0.62 + l * 0.38)
    return tuple(max(0, min(255, int(v))) for v in (r * f + 8, g * f + 2, b * f - 10))


def canvas_rgba(c):
    return [[None if v == 0 else deco.DECO_PAL[v] for v in row] for row in c.p]


# ------------------------------------------------------------------ tileset builder

class Builder:
    def __init__(self, ts):
        self.ts = ts
        self.tiles = []          # new secondary tile pixel tuples
        self.lookup = {}
        self.meta = {}           # metatile id -> 8 entries
        self.attrs = {}
        self.next_id = 0x380
        self.names = {}
        for slot in (7, 8, 9, 10, 12):  # Center colours: dull and smoky (12 = floor, 8 = walls, 7 = counter)
            ts.sec_pals[slot] = [ts.sec_pals[slot][0]] + [grimy(c, 0.74 if slot == 12 else 0.82) for c in ts.sec_pals[slot][1:]]
        pals = ts.sec_pals[:]
        pals[DECO_SLOT] = self.deco_palette()
        self.sec_pals = pals

    def deco_palette(self):
        """deco colours + the main floor / wall colours of the Center, so props blend into the room"""
        cols = Counter()
        for mid in (FLOOR, SHADOW, WALL_LOW, WALL_TOP):
            for row in self.ts.render(mid):
                cols.update(row)
        base = [c for c, _ in cols.most_common(6)]
        deco_cols = [deco.DECO_PAL[i] for i in (O, DG, G, LG, DR, R, LR, Y, RED, DIRT, SOOT, DGL)]
        pal = [(0, 0, 0)] + deco_cols
        for c in base:
            if len(pal) < 16 and all(sum((a - b) ** 2 for a, b in zip(c, p)) > 300 for p in pal[1:]):
                pal.append(c)
        while len(pal) < 16:
            pal.append(deco.DECO_PAL[GL])
        return pal

    def all_pals(self):
        return self.ts.prim_pals + self.sec_pals[7:13]

    def tile_id(self, px):
        if px not in self.lookup:
            self.lookup[px] = len(self.tiles)
            self.tiles.append(px)
        return NUM_PRIM + self.lookup[px]

    def keep(self, mid):
        """copy an original metatile (same id) with its tiles"""
        if mid < NUM_PRIM or mid in self.meta:
            return
        new = []
        for e in self.ts.entries(mid):
            t = e & 0x3FF
            if t >= NUM_PRIM:
                px = self.ts.src_tiles[t - NUM_PRIM]
                e = (e & ~0x3FF) | self.tile_id(px)
            new.append(e)
        self.meta[mid] = new
        self.attrs[mid] = self.ts.attr(mid)

    def encode(self, rgb, behavior_attr, name):
        """16x16 RGB -> new metatile; each 8x8 tile takes the best fitting palette"""
        pals = self.all_pals()
        entries = []
        for q in range(4):
            ox, oy = (q % 2) * 8, (q // 2) * 8
            cells = [rgb[oy + k // 8][ox + k % 8] for k in range(64)]
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
        entries += [0, 0, 0, 0]
        mid = self.next_id
        self.next_id += 1
        self.meta[mid] = entries
        self.attrs[mid] = (behavior_attr & ~(3 << 29)) | (LAYER_NORMAL << 29)
        self.names[name] = mid
        return mid

    def overlay(self, name, base_mid, layers, attr_mid=None):
        """base metatile with RGBA layers (16x16 lists, None = transparent) painted over it"""
        key = '%s@%x' % (name, base_mid)
        if key in self.names:
            return self.names[key]
        rgb = self.ts.render(base_mid)
        for layer in layers:
            for y in range(16):
                for x in range(16):
                    if layer[y][x] is not None:
                        rgb[y][x] = layer[y][x]
        return self.encode(rgb, self.ts.attr(base_mid if attr_mid is None else attr_mid), key)

    def prop(self, name, base_mid=FLOOR):
        fn, seed = PROPS[name]
        return self.overlay(name, base_mid, [canvas_rgba(fn(seed))])

    def big(self, name, canvas, bases):
        """32x32 prop over four base metatiles -> [[tl, tr], [bl, br]]"""
        rgba = canvas_rgba(canvas)
        out = []
        for qy in range(2):
            row = []
            for qx in range(2):
                part = [r[qx * 16:qx * 16 + 16] for r in rgba[qy * 16:qy * 16 + 16]]
                row.append(self.overlay('%s_%d%d' % (name, qx, qy), bases[qy][qx], [part]))
            out.append(row)
        return out

    def vending(self, broken, top_base, bottom_base, seed=0):
        rgba, over = vending(broken, seed)
        o = canvas_rgba(over) if over else None
        ids = []
        for half, base in ((0, top_base), (1, bottom_base)):
            layers = [rgba[half * 16:half * 16 + 16]]
            if o:
                layers.append(o[half * 16:half * 16 + 16])
            ids.append(self.overlay('vending%s_%d_%d' % ('_broken' if broken else '', half, seed), base, layers))
        return ids

    def save(self):
        assert len(self.tiles) <= MAX_SEC, 'too many tiles: %d' % len(self.tiles)
        os.makedirs(os.path.join(ROOT, DST, 'palettes'), exist_ok=True)
        w = 16
        rows = (len(self.tiles) + w - 1) // w
        img = Image.new('P', (w * 8, rows * 8))
        img.putpalette([v for c in self.sec_pals[7] for v in c])
        flat = [0] * (img.width * img.height)
        for i, t in enumerate(self.tiles):
            tx, ty = (i % w) * 8, (i // w) * 8
            for k, v in enumerate(t):
                flat[(ty + k // 8) * img.width + tx + k % 8] = v
        img.putdata(flat)
        img.save(os.path.join(ROOT, DST, 'tiles.png'), bits=4)
        count = max(self.meta) - NUM_PRIM + 1
        meta, attrs = bytearray(), bytearray()
        for i in range(count):
            mid = NUM_PRIM + i
            meta += struct.pack('<8H', *self.meta.get(mid, [0] * 8))
            attrs += struct.pack('<I', self.attrs.get(mid, 0))
        open(os.path.join(ROOT, DST, 'metatiles.bin'), 'wb').write(meta)
        open(os.path.join(ROOT, DST, 'metatile_attributes.bin'), 'wb').write(attrs)
        for i, pal in enumerate(self.sec_pals):
            with open(os.path.join(ROOT, DST, 'palettes', '%02d.pal' % i), 'w', newline='\r\n') as f:
                f.write('JASC-PAL\n0100\n16\n' + ''.join('%d %d %d\n' % c for c in pal))
        # the Center 2F (and the other maps on the old Center tileset) get the same smoky colours
        for slot in (7, 8, 9, 10, 12):
            with open(os.path.join(ROOT, SRC, 'palettes', '%02d.pal' % slot), 'w', newline='\r\n') as f:
                f.write('JASC-PAL\n0100\n16\n' + ''.join('%d %d %d\n' % c for c in self.sec_pals[slot]))
        return len(self.tiles), count


# ------------------------------------------------------------------ layouts

WALK, BLOCK = (0, 3), (1, 0)   # (collision, elevation)


class Grid:
    def __init__(self, w, h):
        self.w, self.h = w, h
        self.c = [[(0x008, 1, 0)] * w for _ in range(h)]  # black outside

    def set(self, x, y, mid, walk):
        col, el = WALK if walk else BLOCK
        self.c[y][x] = (mid, col, el)

    def decorate(self, x, y, mid):
        """new graphics, same collision and elevation (keeps the escalator's special elevations)"""
        _, col, el = self.c[y][x]
        self.c[y][x] = (mid, col, el)

    def copy(self, src, sx, sy, w, h, dx, dy):
        for y in range(h):
            for x in range(w):
                self.c[dy + y][dx + x] = src[sy + y][sx + x]

    def mid(self, x, y):
        return self.c[y][x][0]

    def walkable(self, x, y):
        return 0 <= x < self.w and 0 <= y < self.h and self.c[y][x][1] == 0

    def bytes(self):
        return b''.join(struct.pack('<H', mid | (col << 10) | (el << 12)) for row in self.c for mid, col, el in row)


def read_layout(layout_id, ref=BASE_REF):
    lay = next(l for l in json.loads(git_bytes('data/layouts/layouts.json', ref))['layouts'] if l and l['id'] == layout_id)
    d = git_bytes(lay['blockdata_filepath'], ref)
    w, h = lay['width'], lay['height']
    cells = [[None] * w for _ in range(h)]
    for y in range(h):
        for x in range(w):
            v = struct.unpack_from('<H', d, (y * w + x) * 2)[0]
            cells[y][x] = (v & 0x3FF, (v >> 10) & 3, v >> 12)
    return lay, cells


def room_shell(g, b, width, height):
    """walls (rows 0-1), floor, Center corner pieces; bottom row = height-2, black row = height-1"""
    for x in range(1, width - 1):
        g.set(x, 0, WALL_TOP, False)
        g.set(x, 1, WALL_LOW, False)
        g.set(x, 2, SHADOW, True)
    for y in range(3, height - 1):
        for x in range(width):
            g.set(x, y, FLOOR, True)
    for y, mid in enumerate((0x2A0, 0x2A8, 0x2B0)):  # top left corner
        g.set(0, y, mid, y == 2)
    for y, mid in enumerate((0x2A6, 0x2AE, 0x2B6)):  # top right corner
        g.set(width - 1, y, mid, y == 2)
    g.set(width - 1, 3, 0x2BE, True)
    for y in range(3, height - 2):
        g.set(0, y, 0x2B8, True)
    g.set(0, height - 2, 0x2A1, True)
    g.set(width - 1, height - 2, 0x2A2, True)
    for x in range(width):
        g.set(x, height - 1, 0x008, False)


def dirty(g, b, seed):
    """dirt, soot and burn marks on a good part of the plain floor"""
    import random
    rng = random.Random(seed)
    for y in range(g.h):
        for x in range(g.w):
            mid = g.mid(x, y)
            if mid in (FLOOR, SHADOW) and g.walkable(x, y):
                r = rng.random()
                if r < 0.05:
                    g.decorate(x, y, b.prop('scorch', mid))
                elif r < 0.45:
                    g.decorate(x, y, b.prop(rng.choice(['grime_a', 'grime_b', 'grime_c', 'grime_d']), mid))


def build_pc(b, orig):
    W, H = 19, 12
    g = Grid(W, H)
    room_shell(g, b, W, H)
    g.copy(orig, 1, 0, 3, 3, 1, 0)        # plant + bookshelf (journal objects stay at 2,1 / 3,1)
    g.copy(orig, 4, 0, 7, 5, 6, 0)        # heal-bot counter block (nurse 7,2 -> 9,2)
    g.copy(orig, 11, 0, 3, 3, 14, 0)      # PC + region map
    g.copy(orig, 0, 5, 4, 3, 0, 7)        # escalator with its elevation steps (warp 1,6 -> 1,8)
    g.copy(orig, 6, 5, 3, 3, 8, 6)        # Pokéball floor emblem
    g.copy(orig, 6, 8, 3, 2, 8, 10)       # door mat + exit
    for x in (4, 5, 13, 17):              # wall next to the copied blocks
        g.set(x, 2, SHADOW, True)
    # sandbags on the counter front (keeps MB_COUNTER so the heal-bot can be talked to)
    for x in range(7, 12):
        if g.mid(x, 3) == 0x2BC:
            g.set(x, 3, b.overlay('counter_bags', 0x2BC, [canvas_rgba(counter_sandbags(x))]), False)
    # graffiti and cracks on the walls
    g.set(4, 1, b.prop('tag_a', WALL_LOW), False)
    g.set(5, 0, b.prop('wall_crack', WALL_TOP), False)
    g.set(13, 1, b.prop('tag_x', WALL_LOW), False)
    g.set(17, 1, b.prop('tag_b', WALL_LOW), False)
    g.set(17, 0, b.prop('wall_crack', WALL_TOP), False)
    # collapsed corner top right: scrap heap behind a sandbag wall
    heap = b.big('scrap', deco.scrap_heap(31), [[FLOOR, FLOOR], [FLOOR, FLOOR]])
    for qy in range(2):
        for qx in range(2):
            g.set(17 + qx, 3 + qy, heap[qy][qx], False)
    g.set(18, 2, b.prop('rubble', 0x2B6), False)
    g.set(17, 2, b.prop('rubble', SHADOW), False)
    for x in (16, 17, 18):
        g.set(x, 5, b.prop('sandbags'), False)
    g.set(16, 4, b.prop('barrel'), False)
    # gang corner bottom right: burning barrel, crates, tires, broken stools
    g.set(16, 8, b.prop('barrel_fire'), False)
    g.set(18, 7, b.prop('crates'), False)
    g.set(18, 8, b.prop('crates'), False)
    g.set(17, 9, b.prop('tires'), False)
    g.set(15, 9, b.prop('barrel'), False)
    g.copy(orig, 11, 5, 2, 1, 14, 7)      # two stools by the fire
    # bottom left: barricade by the escalator, more rubble
    g.set(3, 9, b.prop('sandbags'), False)
    g.set(3, 10, b.prop('crates'), False)
    g.set(4, 10, b.prop('rubble'), False)
    g.set(5, 4, b.prop('barrel'), False)
    g.set(4, 4, b.prop('crates'), False)
    # sandbags left and right of the door
    g.set(6, 10, b.prop('sandbags'), False)
    g.set(12, 10, b.prop('sandbags'), False)
    # floor damage (walkable)
    for x, y, p in ((3, 5, 'crack_a'), (12, 6, 'crack_b'), (6, 8, 'crack_c'), (14, 5, 'debris_a'), (11, 9, 'stain'),
                    (2, 4, 'debris_b'), (13, 8, 'crack_a'), (7, 6, 'debris_a'), (15, 3, 'crack_c'), (10, 5, 'stain')):
        g.decorate(x, y, b.prop(p, g.mid(x, y)))
    # barricade in front of the gang corner (x=12/13 stay free: Bill's way out in Cinnabar)
    for x in (14, 15):
        g.set(x, 7, b.prop('sandbags'), False)
    g.set(1, 6, b.prop('rubble'), False)
    dirty(g, b, 1)
    return g


def build_mart(b):
    W, H = 13, 10
    g = Grid(W, H)
    room_shell(g, b, W, H)
    # mat + exit in the middle of the bottom row (same pieces as in the Center)
    _, orig_pc = read_layout(PC1F)
    g.copy(orig_pc, 6, 8, 3, 2, 5, 8)
    # vending machines along the back wall: the working one is the clerk object at (6,2)
    for x, broken in ((2, True), (3, False), (4, True), (8, True), (9, False), (10, True)):
        top, bottom = b.vending(broken, WALL_LOW, SHADOW, seed=x)
        g.set(x, 1, top, False)
        g.set(x, 2, bottom, False)
    g.set(6, 2, SHADOW, False)           # the clerk machine stands here
    g.set(1, 1, b.prop('tag_x', WALL_LOW), False)
    g.set(11, 1, b.prop('tag_a', WALL_LOW), False)
    g.set(5, 0, b.prop('wall_crack', WALL_TOP), False)
    # loot piles and barricades
    for x, y, p in ((1, 4, 'crates'), (1, 5, 'crates'), (2, 5, 'barrel'), (11, 4, 'barrel_fire'), (11, 5, 'crates'),
                    (10, 6, 'tires'), (3, 7, 'sandbags'), (9, 7, 'sandbags'), (1, 7, 'rubble'), (11, 7, 'crates')):
        g.set(x, y, b.prop(p), False)
    for x, y, p in ((4, 4, 'crack_a'), (8, 5, 'debris_a'), (6, 6, 'stain'), (3, 3, 'debris_b'), (9, 3, 'crack_b')):
        g.decorate(x, y, b.prop(p, g.mid(x, y)))
    dirty(g, b, 2)
    return g


# ------------------------------------------------------------------ maps

def free_tile(g, x, y, taken, avoid):
    """nearest walkable, unused tile (BFS over the room)"""
    seen, q = {(x, y)}, deque([(x, y)])
    while q:
        cx, cy = q.popleft()
        if g.walkable(cx, cy) and (cx, cy) not in taken and (cx, cy) not in avoid:
            return cx, cy
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            n = (cx + dx, cy + dy)
            if 0 <= n[0] < g.w and 0 <= n[1] < g.h and n not in seen:
                seen.add(n)
                q.append(n)
    raise RuntimeError('no free tile')


def move_events(layout_id, g, fixed_objects, warp_map, shift, avoid):
    maps = []
    for name in sorted(os.listdir(os.path.join(ROOT, 'data/maps'))):
        path = os.path.join(ROOT, 'data/maps', name, 'map.json')
        if not os.path.exists(path):
            continue
        cur = json.load(open(path))
        if name.endswith('_2F_Frlg') and cur.get('layout') == PC1F:
            # Lavender's 2F used the 1F layout in the original data: give it the real 2F room, events unchanged
            cur['layout'] = 'LAYOUT_POKEMON_CENTER_2F_FRLG'
            with open(path, 'w') as f:
                json.dump(cur, f, indent=2, ensure_ascii=False)
                f.write('\n')
            continue
        if cur.get('layout') != layout_id:
            continue
        orig = json.loads(git_bytes('data/maps/%s/map.json' % name))
        for w, ow in zip(cur['warp_events'], orig['warp_events']):
            w['x'], w['y'] = warp_map(ow['x'], ow['y'])
        taken = set()
        for o, oo in zip(cur['object_events'], orig['object_events']):
            key = o['script'].split('_EventScript_')[-1]
            if (oo['x'], oo['y']) in fixed_objects:
                o['x'], o['y'] = fixed_objects[(oo['x'], oo['y'])]
                if o['graphics_id'] == 'OBJ_EVENT_GFX_CLERK':
                    o['movement_type'] = 'MOVEMENT_TYPE_FACE_DOWN'  # the vending machine faces the room
            else:
                o['x'], o['y'] = free_tile(g, *shift(oo['x'], oo['y']), taken, avoid)
            taken.add((o['x'], o['y']))
        with open(path, 'w') as f:
            json.dump(cur, f, indent=2, ensure_ascii=False)
            f.write('\n')
        maps.append(name)
    return maps


def reachable(g, start, targets, name):
    """every target tile must be reachable from the door (collision only; elevation steps are copied as a whole)"""
    seen, q = {start}, deque([start])
    while q:
        x, y = q.popleft()
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            n = (x + dx, y + dy)
            if n not in seen and g.walkable(*n):
                seen.add(n)
                q.append(n)
    missing = [t for t in targets if t not in seen]
    assert not missing, '%s: not reachable from the door: %s' % (name, missing)


SCENE_PATHS = ([(12, 7), (11, 7), (10, 7), (9, 7), (9, 8), (9, 9)],      # Bill walks out (Cinnabar)
               [(13, 8), (13, 9), (12, 9), (11, 9), (10, 9), (9, 9)],  # the player follows him
               [(8, 7), (9, 7), (9, 8), (9, 9), (8, 10)])              # Blue meets the player (Six Island)


def write_layout(layout_id, g):
    path = os.path.join(ROOT, 'data/layouts/layouts.json')
    data = json.load(open(path))
    for lay in data['layouts']:
        if lay and lay['id'] == layout_id:
            lay['width'], lay['height'] = g.w, g.h
            lay['secondary_tileset'] = 'gTileset_GangInteriorFrlg'
            open(os.path.join(ROOT, lay['blockdata_filepath']), 'wb').write(g.bytes())
    with open(path, 'w') as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
        f.write('\n')


def main():
    ts = Tilesets()
    b = Builder(ts)
    _, orig_pc = read_layout(PC1F)
    keep = {c[0] for row in orig_pc for c in row} | set(ESCALATOR) | {FLOOR, SHADOW, WALL_TOP, WALL_LOW}
    for mid in sorted(keep):
        b.keep(mid)
    pc = build_pc(b, orig_pc)
    mart = build_mart(b)
    # door -> heal-bot counter, PC, escalator entry / door -> vending machine
    reachable(pc, (9, 10), [(9, 4), (14, 2), (2, 8), (3, 8)], 'Center')
    # scripted walks that must stay free: Bill (Cinnabar, from 13,7) and Blue (Six Island, from 7,7) to the door
    for path in SCENE_PATHS:
        assert all(pc.walkable(*p) for p in path), 'scripted path blocked: %s' % path
    reachable(mart, (6, 8), [(6, 3)], 'Mart')
    ntiles, nmeta = b.save()
    write_layout(PC1F, pc)
    write_layout(MART, mart)

    # Center: nurse block moved 2 right, escalator 2 down, door 2 right and 2 down
    def pc_warp(x, y):
        return (x + 2, y + 2) if y == 8 else (x, y + 2) if (x, y) == (1, 6) else (x, y)

    def pc_shift(x, y):
        return (x + 2, y + 2 if y >= 4 else y)
    pc_fixed = {(7, 2): (9, 2), (2, 1): (2, 1), (3, 1): (3, 1)}
    pc_avoid = {(9, 4), (14, 2), (8, 10), (9, 10), (10, 10), (9, 9), (1, 8), (2, 8), (3, 8)} | set(SCENE_PATHS[0] + SCENE_PATHS[1] + SCENE_PATHS[2])
    pcs = move_events(PC1F, pc, pc_fixed, pc_warp, pc_shift, pc_avoid)

    def mart_warp(x, y):
        return (x + 2, y + 1)   # door mat row 7 -> 8

    def mart_shift(x, y):
        return (x + 1, y + 1)
    mart_fixed = {(2, 3): (6, 2)}
    mart_avoid = {(6, 3), (5, 8), (6, 8), (7, 8), (6, 7)}
    marts = move_events(MART, mart, mart_fixed, mart_warp, mart_shift, mart_avoid)
    print('tiles %d/%d, metatiles %d, Center maps %d, Mart maps %d' % (ntiles, MAX_SEC, nmeta, len(pcs), len(marts)))

    if '--preview' in sys.argv:
        out = sys.argv[sys.argv.index('--preview') + 1]
        os.makedirs(out, exist_ok=True)
        import render_map
        render_map.render(PC1F, os.path.join(out, 'pc_1f.png'))
        render_map.render(MART, os.path.join(out, 'mart.png'))


if __name__ == '__main__':
    main()
