#!/usr/bin/env python3
"""Gangster Outbreak: apocalypse decoration (stage B).

Draws wreckage props (rubble, car wrecks, barriers, barrels, tires, road cracks, graffiti) as pixel art,
appends them to FRLG city tilesets and writes the deco palette into the free palette slot 7.

Like recolor.py it always starts from the ORIGINAL tileset files in git (BASE_REF), so it can be re-run.
Writes tools/apocalypse/deco_ids.json (name -> metatile id per tileset) for place.py.

Usage: deco.py [--preview out.png]
"""
import json, os, random, struct, subprocess, sys
from io import BytesIO
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE_REF = 'ddfe1f3d'
DECO_PAL_SLOT = 7
NUM_TILES_IN_PRIMARY = 640
NUM_METATILES_IN_PRIMARY = 640
LAYER_TYPE_COVERED = 1

# secondary tileset dir -> ground metatiles (primary ids) the props stand on
GROUNDS = {'dirt': 1, 'road': 366}
# Viridian uses fixed grounds (hand-placed props refer to them); the other cities detect their most common
# free ground metatiles automatically (None) and get every prop for each of them.
AUTO_GROUND_COUNT = 6
AUTO_GROUND_FIRST = 3  # all props get these grounds first, small props then also the rest
TILESETS = {'data/tilesets/secondary/viridian_city_frlg': GROUNDS}
TILESETS.update({'data/tilesets/secondary/%s' % name: None for name in (
    'pallet_town_frlg', 'pewter_city_frlg', 'cerulean_city_frlg', 'vermilion_city_frlg',
    'lavender_town_frlg', 'celadon_city_frlg', 'fuchsia_city_frlg', 'saffron_city_frlg', 'cinnabar_island_frlg',
    'indigo_plateau_frlg')})
MAX_SECONDARY_TILES = 1024 - 640
MAX_SECONDARY_METATILES = 1024 - 640
VARIANT_SHARE = 0.55  # building damage / burnt nature may use this share of the free tiles and metatiles
# tiles that tileset animations overwrite at runtime (src/tileset_anims.c): never merged or reused
ANIM_TILES = {'data/tilesets/secondary/celadon_city_frlg': range(744 - 640, 752 - 640)}

# index: colour
DECO_PAL = [
    (0, 0, 0),        # 0 transparent
    (24, 20, 20),     # 1 outline
    (56, 52, 52),     # 2 dark grey
    (96, 92, 88),     # 3 grey
    (136, 132, 124),  # 4 light grey
    (176, 168, 156),  # 5 pale concrete
    (80, 36, 24),     # 6 dark rust
    (128, 60, 32),    # 7 rust
    (176, 96, 48),    # 8 light rust
    (216, 176, 40),   # 9 hazard yellow
    (112, 24, 24),    # 10 dark red
    (192, 40, 40),    # 11 spray red
    (112, 160, 40),   # 12 toxic green (was dark glass; interiors.py keeps the old colour)
    (120, 140, 152),  # 13 glass
    (40, 32, 24),     # 14 soot / shadow
    (104, 80, 56),    # 15 dirt brown
]

O, DG, G, LG, PC, DR, R, LR, Y, DRED, RED, DGL, GL, SOOT, DIRT = range(1, 16)
TOX = DGL  # slot 12 is toxic green outdoors
LEGACY_DARK_GLASS = (56, 72, 88)


class Canvas:
    def __init__(self, w, h, seed):
        self.w, self.h = w, h
        self.p = [[0] * w for _ in range(h)]
        self.rng = random.Random(seed)

    def set(self, x, y, c):
        if 0 <= x < self.w and 0 <= y < self.h:
            self.p[y][x] = c

    def get(self, x, y):
        return self.p[y][x] if 0 <= x < self.w and 0 <= y < self.h else 0

    def rect(self, x0, y0, x1, y1, c):
        for y in range(y0, y1 + 1):
            for x in range(x0, x1 + 1):
                self.set(x, y, c)

    def ellipse(self, cx, cy, rx, ry, c):
        for y in range(int(cy - ry) - 1, int(cy + ry) + 2):
            for x in range(int(cx - rx) - 1, int(cx + rx) + 2):
                if ((x + 0.5 - cx) / rx) ** 2 + ((y + 0.5 - cy) / ry) ** 2 <= 1:
                    self.set(x, y, c)

    def line(self, x0, y0, x1, y1, c):
        n = max(abs(x1 - x0), abs(y1 - y0), 1)
        for i in range(n + 1):
            self.set(round(x0 + (x1 - x0) * i / n), round(y0 + (y1 - y0) * i / n), c)

    def speckle(self, colors, src, chance):
        for y in range(self.h):
            for x in range(self.w):
                if self.p[y][x] in src and self.rng.random() < chance:
                    self.p[y][x] = self.rng.choice(colors)

    def outline(self, skip=(SOOT,)):
        """Dark outline around everything (except soft parts like shadows)."""
        add = []
        for y in range(self.h):
            for x in range(self.w):
                if self.p[y][x] == 0 and any(self.get(x + dx, y + dy) not in (0, O) + tuple(skip)
                                             for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                    add.append((x, y))
        for x, y in add:
            self.p[y][x] = O


# ---------------------------------------------------------------- props

def rubble(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 12.5, 7.5, 3, SOOT)
    chunks = [(5, 10, 4, 3), (10, 10, 4, 3), (8, 7, 4, 3), (3, 12, 3, 2), (12, 12, 3, 2), (7, 12, 3, 2)]
    for i, (cx, cy, rx, ry) in enumerate(chunks):
        cx += c.rng.uniform(-0.8, 0.8)
        cy += c.rng.uniform(-0.5, 0.5)
        c.ellipse(cx, cy, rx * c.rng.uniform(0.75, 1.0), ry, G)
        c.ellipse(cx - 0.6, cy - 0.8, rx * 0.65, ry * 0.55, LG)
        c.set(int(cx - rx * 0.4), int(cy - ry * 0.6), PC)
        c.line(int(cx - rx + 1), int(cy + ry - 1), int(cx + rx - 1), int(cy + ry - 1), DG)
    # rebar sticking out
    x = c.rng.choice([4, 11])
    c.line(x, 8, x + (2 if x < 8 else -2), 3, R)
    c.set(x + (2 if x < 8 else -2), 3, LR)
    c.speckle([DG, G], (LG,), 0.08)
    c.outline()
    return c


def car(seed):
    c = Canvas(32, 16, seed)
    c.ellipse(16, 13.5, 15.5, 2.5, SOOT)
    # body
    c.rect(2, 7, 29, 12, R)
    c.rect(2, 7, 29, 7, LR)
    c.rect(1, 8, 1, 11, R)
    c.rect(30, 8, 30, 11, R)
    c.rect(2, 12, 29, 12, DR)
    # cabin
    c.rect(8, 2, 22, 6, R)
    c.rect(9, 2, 21, 2, LR)
    for x in range(7, 9):
        c.line(x, 6, x + 2, 3, R)
    c.line(23, 6, 21, 3, R)
    # windows (one smashed)
    c.rect(10, 3, 14, 6, DG)
    c.rect(16, 3, 20, 6, DG)
    c.set(11, 3, GL); c.set(12, 4, GL); c.set(17, 3, GL)
    c.set(18, 4, O); c.set(19, 5, O); c.set(17, 5, O); c.set(19, 3, O)
    c.rect(15, 2, 15, 6, DR)
    # door line, handle, lights
    c.line(15, 7, 15, 11, DR)
    c.set(13, 8, O); c.set(19, 8, O)
    c.rect(2, 8, 3, 9, Y)
    c.rect(28, 8, 29, 9, RED)
    # wheels (tires gone, rims only)
    for wx in (7, 24):
        c.ellipse(wx, 12.5, 3, 2.5, O)
        c.ellipse(wx, 12.5, 1.6, 1.3, G)
        c.set(wx, 12, LG)
    # rust and soot
    c.speckle([DR, LR, SOOT], (R,), 0.18)
    c.speckle([DR], (LR,), 0.15)
    c.outline()
    return c


def barrier(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14, 7.5, 1.5, SOOT)
    for px in (2, 13):
        c.rect(px, 8, px + 1, 14, G)
        c.rect(px, 8, px, 14, LG)
    c.rect(0, 4, 15, 8, Y)
    for x in range(-8, 16, 4):
        for d in range(2):
            c.line(x + d, 8, x + 4 + d, 4, O)
    c.rect(0, 4, 15, 4, PC)
    c.speckle([R, DR], (Y,), 0.10)
    c.outline()
    return c


def barrel(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14, 6.5, 1.8, SOOT)
    c.rect(3, 4, 12, 13, R)
    c.rect(3, 4, 4, 13, LR)
    c.rect(11, 4, 12, 13, DR)
    for y in (6, 10):
        c.rect(3, y, 12, y, DR)
    c.ellipse(8, 4, 5, 1.6, DR)
    c.ellipse(8, 4, 3.5, 1, SOOT)
    c.set(7, 1, DG); c.set(8, 0, G); c.set(9, 2, DG)  # smoke
    c.speckle([DR, LR, SOOT], (R,), 0.2)
    c.outline(skip=(SOOT, DG, G))
    return c


def tires(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14, 7.5, 1.8, SOOT)
    for (cx, cy) in ((5, 11), (11, 11), (8, 6)):
        c.ellipse(cx, cy, 4, 3, DG)
        c.ellipse(cx, cy - 0.5, 3, 2, G)
        c.ellipse(cx, cy, 1.8, 1.1, SOOT)
    c.outline()
    return c


def crack(seed):
    c = Canvas(16, 16, seed)
    x, y = 0, c.rng.randint(3, 9)
    while x < 16:
        nx, ny = x + c.rng.choice([1, 2, 2]), max(1, min(14, y + c.rng.choice([-1, 0, 1, 1])))
        c.line(x, y, nx, ny, SOOT)
        c.line(x, y + 1, nx, ny + 1, DG)
        if c.rng.random() < 0.35:
            bx, by = nx + c.rng.choice([-2, -1, 1, 2]), ny + c.rng.choice([-3, 3])
            c.line(nx, ny, bx, by, SOOT)
        x, y = nx, ny
    return c


def graffiti(seed):
    c = Canvas(16, 16, seed)
    # gang tag: a crowned "X" with drips
    c.line(3, 4, 12, 12, RED)
    c.line(4, 4, 13, 12, RED)
    c.line(12, 4, 3, 12, RED)
    c.line(13, 4, 4, 12, RED)
    for x in (4, 7, 10, 12):
        c.set(x, 2, DRED)
    c.line(4, 2, 12, 2, DRED)
    c.set(5, 1, DRED); c.set(8, 1, DRED); c.set(11, 1, DRED)
    c.set(4, 13, DRED); c.set(4, 14, DRED); c.set(12, 13, DRED)
    return c


def scrap_heap(seed):
    c = Canvas(32, 32, seed)
    c.ellipse(16, 25, 15.5, 6, SOOT)
    # pile body: metal sheets, rust, tires
    for i in range(26):
        cx, cy = c.rng.uniform(5, 27), c.rng.uniform(10, 25)
        cy = max(cy, 6 + abs(cx - 16) * 0.9)  # cone shape
        col = c.rng.choice([R, DR, G, LG, DG, LR])
        if c.rng.random() < 0.5:
            w, h = c.rng.randint(3, 7), c.rng.randint(2, 4)
            c.rect(int(cx - w / 2), int(cy - h / 2), int(cx + w / 2), int(cy + h / 2), col)
            c.line(int(cx - w / 2), int(cy - h / 2), int(cx + w / 2), int(cy - h / 2), LG if col in (G, DG) else LR)
        else:
            c.ellipse(cx, cy, c.rng.uniform(1.5, 3.5), c.rng.uniform(1.2, 2.5), col)
    for _ in range(3):  # tires
        cx, cy = c.rng.uniform(8, 24), c.rng.uniform(16, 25)
        c.ellipse(cx, cy, 3, 2.2, DG)
        c.ellipse(cx, cy, 1.4, 0.9, SOOT)
    for _ in range(4):  # pipes / rebar
        x = c.rng.randint(8, 24)
        c.line(x, c.rng.randint(12, 18), x + c.rng.randint(-4, 4), c.rng.randint(4, 9), c.rng.choice([R, LG]))
    c.speckle([DR, SOOT, LR], (R,), 0.15)
    c.speckle([DG], (G, LG), 0.1)
    c.outline()
    return c


def sandbags(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14, 8, 1.5, SOOT)
    for row, (y, off) in enumerate(((11, 0), (7, 4), (3, 0))):
        for x in range(-4 + off, 16, 8):
            c.ellipse(x + 4, y + 1, 4, 2.2, DIRT)
            c.ellipse(x + 3.5, y + 0.3, 2.6, 1.1, PC)
            c.line(x + 1, y + 3, x + 7, y + 3, SOOT)
    for y in range(16):
        for x in range(16):
            if c.p[y][x] == DIRT and c.rng.random() < 0.12:
                c.p[y][x] = SOOT
    c.outline()
    return c


def car_flipped(seed):
    c = Canvas(32, 16, seed)
    c.ellipse(16, 13.5, 15.5, 2.5, SOOT)
    c.rect(2, 7, 29, 12, DG)          # underbody
    c.rect(2, 7, 29, 7, G)
    c.rect(5, 9, 26, 9, SOOT)         # axle / exhaust line
    c.rect(12, 8, 19, 11, R)          # rusty fuel tank
    c.rect(2, 12, 29, 12, R)          # roof edge at the bottom
    for wx in (7, 24):                # wheels sticking up
        c.ellipse(wx, 5.5, 3.4, 3, O)
        c.ellipse(wx, 5.5, 2, 1.7, G)
        c.set(wx, 5, LG)
    c.speckle([R, DR, SOOT], (DG,), 0.10)
    c.outline()
    return c


def crater(seed):
    c = Canvas(32, 32, seed)
    c.ellipse(16, 16, 15, 12, DG)
    c.ellipse(16, 15, 13, 10, SOOT)
    c.ellipse(16, 18, 9, 6, O)
    for i in range(40):
        a = c.rng.uniform(0, 6.283)
        import math
        r = c.rng.uniform(0.85, 1.05)
        c.set(int(16 + math.cos(a) * 15 * r), int(16 + math.sin(a) * 12 * r), c.rng.choice([G, LG, DG]))
    c.speckle([DG, G], (SOOT,), 0.06)
    return c


def fire_barrel(seed):
    c = barrel(seed)
    for y in range(0, 4):  # flames instead of smoke
        for x in range(16):
            if c.p[y][x] in (DG, G):
                c.p[y][x] = 0
    for x, y, col in ((5, 3, Y), (7, 1, Y), (9, 2, Y), (11, 3, Y), (6, 2, LR), (8, 0, LR), (10, 1, LR),
                      (8, 3, RED), (6, 3, RED), (10, 3, RED)):
        c.set(x, y, col)
        c.set(x, y + 1, col if col != Y else LR)
    c.ellipse(8, 4, 3.5, 1, Y)
    return c


def trash_bags(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14, 7.5, 1.8, SOOT)
    for cx, cy, rx, ry in ((4.5, 11, 3.8, 3.2), (11.5, 11.5, 3.8, 2.8), (8, 7.5, 3.4, 2.8)):
        c.ellipse(cx, cy, rx, ry, O)
        c.ellipse(cx, cy + 0.3, rx - 0.8, ry - 0.8, DG)
        c.line(int(cx - rx + 2), int(cy - 1), int(cx - 1), int(cy - ry + 1), G)   # glossy highlight
        kx, ky = int(cx), int(cy - ry)
        c.set(kx, ky, DG); c.set(kx, ky - 1, G); c.set(kx - 1, ky - 2, DG); c.set(kx + 1, ky - 2, DG)  # knot
    for x, y, col in ((14, 13, DIRT), (15, 12, TOX), (13, 14, PC), (1, 14, PC), (2, 13, R)):  # spilled garbage
        c.set(x, y, col)
    c.outline(skip=(SOOT, O))
    return c


def fence_broken(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 14, 7.5, 1.4, SOOT)
    for px_, top in ((1, 4), (7, 6), (13, 3)):
        c.rect(px_, top, px_ + 1, 14, DR)
        c.rect(px_, top, px_, 14, R)
    c.rect(0, 6, 4, 7, R)        # rail stub left
    c.rect(0, 6, 4, 6, LR)
    c.line(9, 10, 15, 6, R)      # rail hanging down
    c.line(9, 11, 15, 7, DR)
    c.line(2, 11, 7, 12, R)      # lower rail, bent
    c.speckle([DR, SOOT], (R,), 0.2)
    c.outline()
    return c


def pole_fallen(seed):
    c = Canvas(32, 16, seed)
    c.ellipse(16, 13, 15.5, 2, SOOT)
    c.ellipse(4, 12, 3, 1.5, DG)       # broken concrete base
    c.rect(2, 9, 5, 11, G)
    c.rect(2, 9, 5, 9, LG)
    c.line(5, 10, 25, 7, G)           # the pole lying on the ground
    c.line(5, 11, 25, 8, DG)
    c.line(6, 9, 24, 6, LG)
    c.rect(25, 4, 29, 8, DG)          # lamp head
    c.rect(26, 5, 28, 7, Y)
    c.set(27, 6, PC)
    for x, y in ((22, 12), (24, 13), (28, 11), (19, 13)):  # glass shards
        c.set(x, y, GL)
    c.outline()
    return c


def bus(seed):
    c = Canvas(48, 16, seed)
    c.ellipse(24, 13.5, 23.5, 2.5, SOOT)
    c.rect(1, 2, 46, 12, Y)
    c.rect(1, 2, 46, 2, PC)
    c.rect(1, 12, 46, 12, DR)
    c.rect(1, 8, 46, 8, DR)           # stripe
    for x in range(4, 44, 6):         # windows, most of them smashed
        c.rect(x, 3, x + 4, 6, DG)
        if c.rng.random() < 0.6:
            c.set(x + 1, 3, GL)
            c.set(x + 3, 5, O)
    c.rect(43, 3, 46, 11, DG)         # door
    c.line(44, 3, 44, 11, G)
    for wx in (8, 38):
        c.ellipse(wx, 12.5, 3, 2.5, O)
        c.ellipse(wx, 12.5, 1.5, 1.2, G)
    c.speckle([R, LR, DR, SOOT], (Y,), 0.22)   # rust and burn marks
    for _ in range(3):
        bx = c.rng.randint(4, 40)
        c.ellipse(bx, 6, 3, 2.5, SOOT)
    c.outline()
    return c


def camp(seed):
    c = Canvas(32, 16, seed)
    c.ellipse(16, 14, 15.5, 1.8, SOOT)
    for y in range(3, 14):            # tent (left)
        half = (y - 3) * 0.75
        c.rect(int(10 - half), y, int(10 + half), y, DIRT)
        c.set(int(10 - half), y, PC)
    c.line(10, 3, 10, 13, SOOT)
    c.rect(9, 9, 11, 13, O)           # entrance
    c.set(10, 2, R)                   # flag pole tip
    c.rect(11, 1, 14, 2, RED)         # gang flag
    for x, y in ((22, 12), (25, 13), (28, 12), (23, 14), (27, 14)):  # stones around the fire
        c.set(x, y, LG)
    c.ellipse(25, 12.5, 2.5, 1.2, SOOT)
    for x, y, col in ((24, 10, Y), (25, 8, LR), (26, 10, Y), (25, 11, Y), (24, 11, RED), (26, 11, RED), (25, 9, Y)):
        c.set(x, y, col)
    c.outline(skip=(SOOT, Y, LR, RED))
    return c


def dead_tree(seed):
    c = Canvas(16, 32, seed)
    c.ellipse(8, 29.5, 6, 2, SOOT)
    c.rect(7, 10, 9, 29, DG)
    c.rect(7, 10, 7, 29, G)
    c.set(6, 29, DG); c.set(10, 29, DG); c.set(5, 30, DG); c.set(11, 30, DG)  # roots

    def branch(x, y, dx, depth):
        if depth == 0:
            return
        nx, ny = x + dx * c.rng.randint(2, 3), y - c.rng.randint(2, 4)
        c.line(x, y, nx, ny, DG)
        if depth > 1:
            branch(nx, ny, dx, depth - 1)
            branch(nx, ny, -dx if c.rng.random() < 0.4 else dx, depth - 2)
    branch(8, 14, -1, 3)
    branch(8, 12, 1, 3)
    branch(8, 10, 1 if c.rng.random() < 0.5 else -1, 2)
    c.set(8, 18, SOOT); c.set(8, 22, SOOT)     # burn scars
    c.speckle([SOOT], (DG,), 0.15)
    c.outline()
    return c


def toxic_puddle(seed):
    c = Canvas(16, 16, seed)
    c.ellipse(8, 9, 7, 4.5, SOOT)
    c.ellipse(8, 9, 6, 3.6, TOX)
    c.ellipse(9.5, 10, 2.5, 1.4, Y)            # bright toxic spot
    c.ellipse(9.5, 10, 1.5, 0.8, TOX)
    for _ in range(3):                          # bubbles
        x, y = c.rng.randint(4, 12), c.rng.randint(7, 11)
        c.set(x, y, PC)
    c.speckle([SOOT], (TOX,), 0.06)
    return c


def burnt_patch(seed):
    c = Canvas(16, 16, seed)
    for y in range(16):
        for x in range(16):
            d = ((x - 7.5) / 7.5) ** 2 + ((y - 7.5) / 6.5) ** 2 + c.rng.uniform(-0.25, 0.25)
            if d < 0.35:
                c.p[y][x] = O if c.rng.random() < 0.5 else SOOT
            elif d < 0.75:
                c.p[y][x] = SOOT if c.rng.random() < 0.7 else DG
            elif d < 1.0 and c.rng.random() < 0.45:
                c.p[y][x] = SOOT
    for _ in range(3):                          # embers
        x, y = c.rng.randint(5, 10), c.rng.randint(5, 10)
        c.set(x, y, c.rng.choice([LR, Y]))
    return c


def weeds(seed):
    c = Canvas(16, 16, seed)
    for _ in range(7):
        x, y = c.rng.randint(1, 14), c.rng.randint(5, 14)
        for k in range(-2, 3):
            h = c.rng.randint(2, 4) if k else c.rng.randint(4, 6)
            c.line(x + k // 2, y, x + k, y - h, TOX if c.rng.random() < 0.6 else DIRT)
        c.set(x, y, SOOT)
    return c


# name -> (draw function, seed, ground, blocks movement). Ordered by priority: when a tileset runs out of
# tiles or metatiles, the props at the end are left out there.
PROPS = [
    ('rubble_a', rubble, 1, 'dirt', True),
    ('car_road', car, 5, 'road', True),
    ('car_dirt', car, 9, 'dirt', True),
    ('barrel_dirt', barrel, 2, 'dirt', True),
    ('barrel_road', barrel, 4, 'road', True),
    ('fire_barrel_dirt', fire_barrel, 6, 'dirt', True),
    ('fire_barrel_road', fire_barrel, 6, 'road', True),
    ('crack_a', crack, 11, 'road', False),
    ('crack_b', crack, 23, 'road', False),
    ('graffiti_road', graffiti, 1, 'road', False),
    ('burnt_patch_dirt', burnt_patch, 3, 'dirt', False),
    ('toxic_puddle_dirt', toxic_puddle, 2, 'dirt', False),
    ('weeds_road', weeds, 4, 'road', False),
    ('weeds_dirt', weeds, 9, 'dirt', False),
    ('trash_bags_dirt', trash_bags, 1, 'dirt', True),
    ('trash_bags_road', trash_bags, 1, 'road', True),
    ('dead_tree_dirt', dead_tree, 3, 'dirt', True),
    ('barrier_road', barrier, 1, 'road', True),
    ('barrier_dirt', barrier, 1, 'dirt', True),
    ('rubble_b', rubble, 7, 'dirt', True),
    ('rubble_road', rubble, 3, 'road', True),
    ('tires_dirt', tires, 1, 'dirt', True),
    ('fence_broken_dirt', fence_broken, 2, 'dirt', True),
    ('sandbags_dirt', sandbags, 1, 'dirt', True),
    ('sandbags_road', sandbags, 1, 'road', True),
    ('pole_fallen_road', pole_fallen, 1, 'road', True),
    ('pole_fallen_dirt', pole_fallen, 1, 'dirt', True),
    ('camp_dirt', camp, 4, 'dirt', True),
    ('bus_road', bus, 2, 'road', True),
    ('scrap_heap_a', scrap_heap, 3, 'dirt', True),
    ('car_flipped_road', car_flipped, 2, 'road', True),
    ('car_flipped_dirt', car_flipped, 4, 'dirt', True),
    ('crack_c', crack, 5, 'road', False),
    ('scrap_heap_b', scrap_heap, 8, 'dirt', True),
    ('crater_road', crater, 6, 'road', True),
]
# auto-ground tilesets: big props only get the most common ground(s)
PROP_MAX_GROUNDS = {'bus': 1, 'camp': 2, 'pole_fallen': 2, 'scrap_heap_a': 2, 'scrap_heap_b': 1, 'crater': 1,
                    'dead_tree': 2, 'car_flipped': 2}


# ---------------------------------------------------------------- building variants
# A variant copies an existing building metatile and paints over its top layer (boards over windows,
# graffiti, the Rocket "R"). Colors are matched to the tile's own palette; if that palette has nothing close
# (e.g. no red), the tile switches to the deco palette.

PLANK = (124, 88, 52)
PLANK_DARK = (92, 64, 40)
GAP = (40, 32, 24)
SPRAY = (192, 40, 40)
SPRAY_DARK = (112, 24, 24)


def is_glass(rgb):
    import colorsys
    h, s_, v = colorsys.rgb_to_hsv(*(c / 255 for c in rgb))
    return s_ > 0.15 and 0.47 <= h <= 0.70 and v > 0.3


# Overlay values: an RGB tuple (matched to the palette) or one of these tokens, resolved per tile from the
# colors the tile already uses, so the painted parts never clash with the rest of the building.
LIGHT, LIGHT2, DARK, CONTRAST = 'light', 'light2', 'dark', 'contrast'


def ov_boards(orig, rng):
    out = [[None] * 16 for _ in range(16)]
    glass = [(x, y) for y in range(16) for x in range(16) if orig[y][x] is not None and is_glass(orig[y][x])]
    if not glass:
        return out
    x0, x1 = min(x for x, _ in glass), max(x for x, _ in glass)
    y0, y1 = min(y for _, y in glass), max(y for _, y in glass)
    w, h = max(x1 - x0, 1), max(y1 - y0, 1)
    gs = set(glass)
    for x, y in glass:
        out[y][x] = DARK  # dark room behind the boards
    for x, y in glass:
        t = (y - y0) / h
        d = min(abs((x - x0) - t * w), abs((x1 - x) - t * w))
        if d <= 1.2:
            out[y][x] = LIGHT
        elif abs(y - (y0 + y1) / 2) <= 1:
            out[y][x] = LIGHT2
    for x, y in glass:  # nails
        if out[y][x] == LIGHT and rng.random() < 0.06:
            out[y][x] = DARK
    return out


def ov_graffiti(orig, rng):
    out = [[None] * 16 for _ in range(16)]
    x, y = 1, rng.randint(5, 9)
    while x < 15:
        nx, ny = x + rng.randint(1, 2), max(3, min(12, y + rng.choice([-3, -2, 2, 3])))
        n = max(abs(nx - x), abs(ny - y), 1)
        for i in range(n + 1):
            px, py = round(x + (nx - x) * i / n), round(y + (ny - y) * i / n)
            for dy in (0, 1):  # 2px thick spray
                if 0 <= px < 16 and py + dy < 16 and orig[py + dy][px] is not None:
                    out[py + dy][px] = CONTRAST
        if rng.random() < 0.3 and ny + 3 < 16 and nx < 16 and orig[ny + 2][nx] is not None:
            out[ny + 1][nx] = CONTRAST
            out[ny + 2][nx] = CONTRAST
        x, y = nx, ny
    return out


R_GLYPH = [
    "#######..",
    "##....##.",
    "##....##.",
    "##....##.",
    "#######..",
    "##..##...",
    "##...##..",
    "##....##.",
    "##.....##",
]


def ov_rocket_r(orig, rng):
    out = [[None] * 16 for _ in range(16)]
    for y in range(16):
        for x in range(16):
            if orig[y][x] is not None and sum(orig[y][x]) > 380:  # light emblem -> dark plaque
                out[y][x] = GAP
    oy, ox = 3, 4
    for gy, row in enumerate(R_GLYPH):
        for gx, ch in enumerate(row):
            if ch == '#':
                out[oy + gy][ox + gx] = SPRAY
    for gy, row in enumerate(R_GLYPH):  # dark outline
        for gx, ch in enumerate(row):
            if ch != '#':
                continue
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                tx, ty = ox + gx + dx, oy + gy + dy
                if 0 <= tx < 16 and 0 <= ty < 16 and out[ty][tx] != SPRAY:
                    out[ty][tx] = GAP
    return out


def ov_scorch(orig, rng):
    """soot plume rising up the wall (fire damage), burnt-out glass"""
    out = [[None] * 16 for _ in range(16)]
    cx = rng.uniform(4, 12)
    for y in range(16):
        width = 2 + (15 - y) * 0.45
        for x in range(16):
            if orig[y][x] is None:
                continue
            d = abs(x + 0.5 - cx) / width + rng.uniform(-0.25, 0.25)
            if is_glass(orig[y][x]) or d < 0.45:
                out[y][x] = DARK
            elif d < 0.9 and (x + y) % 2 == 0:
                out[y][x] = DARK
    return out


def ov_roof_hole(orig, rng):
    """a hole broken into the roof: dark attic, broken beams, splintered rim"""
    out = [[None] * 16 for _ in range(16)]
    cx, cy = rng.uniform(6, 10), rng.uniform(6, 10)
    rx, ry = rng.uniform(4.5, 6.5), rng.uniform(3.2, 4.8)
    for y in range(16):
        for x in range(16):
            if orig[y][x] is None:
                continue
            d = ((x + 0.5 - cx) / rx) ** 2 + ((y + 0.5 - cy) / ry) ** 2 + rng.uniform(-0.3, 0.3)
            if d < 1:
                out[y][x] = GAP
            elif d < 1.6 and rng.random() < 0.55:
                out[y][x] = LIGHT2
    for k in range(2):  # beams across the hole
        by = int(cy - ry / 2 + k * ry)
        for x in range(int(cx - rx), int(cx + rx) + 1):
            if 0 <= x < 16 and 0 <= by < 16 and out[by][x] == GAP:
                out[by][x] = PLANK_DARK
    return out


# name -> (source metatile, overlay, seed). Primary buildings (gym, mart, Pokémon Center) exist in every city.
COMMON_VARIANTS = [
    ('gym_window_a_boarded', 336, ov_boards, 2),
    ('gym_window_b_boarded', 337, ov_boards, 2),
    ('gym_window_c_boarded', 342, ov_boards, 2),
    ('gym_wall_graffiti', 344, ov_graffiti, 5),
    ('gym_wall_graffiti_b', 350, ov_graffiti, 12),
    ('mart_wall_graffiti', 64, ov_graffiti, 7),
    ('pc_wall_graffiti', 88, ov_graffiti, 4),
    ('gym_wall_scorch', 344, ov_scorch, 21),
    ('mart_wall_scorch', 64, ov_scorch, 22),
    ('pc_wall_scorch', 88, ov_scorch, 23),
]
TREE_METATILES = (20, 21, 22, 23, 28, 29, 30, 31, 36, 37)  # primary tileset forest
TALL_GRASS_METATILE = 13
GROUND_REFERENCE_METATILES = (1, 8, 9, 16, 17)  # plain ground of the primary tileset
ROOF_SOURCES_MAX = 6
VARIANTS = {
    'data/tilesets/secondary/viridian_city_frlg': [
        ('house_window_boarded', 678, ov_boards, 1),
        ('house_window_left_boarded', 677, ov_boards, 1),
        ('house_window_right_boarded', 679, ov_boards, 1),
        ('house_wall_graffiti_a', 657, ov_graffiti, 3),
        ('house_wall_graffiti_b', 659, ov_graffiti, 9),
    ] + COMMON_VARIANTS[:5] + [
        ('gym_rocket_r', 339, ov_rocket_r, 0),  # Giovanni's HQ
    ] + COMMON_VARIANTS[5:],
}
AUTO_WINDOW_MIN_GLASS = 10  # other cities: every metatile with this many glass pixels gets a boarded variant
AUTO_WINDOW_MAX = 6
WINDOW_SCORCH_MAX = 3



def read_pal_text(text):
    return [tuple(int(v) for v in l.split()) for l in text.replace('\r', '').split('\n')[3:19]]


def nearest(pal, rgb):
    best, err = 1, 1e9
    for i in range(1, 16):
        e = sum((a - b) ** 2 for a, b in zip(pal[i], rgb))
        if e < err:
            best, err = i, e
    return best, err ** 0.5


# ---------------------------------------------------------------- tileset building

def git_bytes(path):
    return subprocess.run(['git', '-C', ROOT, 'show', '%s:%s' % (BASE_REF, path)], capture_output=True, check=True).stdout


def canvas_tiles(c):
    """Split a canvas into 8x8 tiles, metatile by metatile (each metatile = 2x2 tiles)."""
    metas = []
    for my in range(0, c.h, 16):
        for mx in range(0, c.w, 16):
            quads = []
            for ty, tx in ((0, 0), (0, 8), (8, 0), (8, 8)):
                quads.append(tuple(c.p[my + ty + y][mx + tx + x] for y in range(8) for x in range(8)))
            metas.append(quads)
    return metas


def auto_grounds(tileset):
    """Most common free, plain ground metatiles on the Kanto maps that use this tileset."""
    import collections, re
    layouts = json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']
    headers = open(os.path.join(ROOT, 'src/data/tilesets/headers.h')).read()
    gfx = open(os.path.join(ROOT, 'src/data/tilesets/graphics.h')).read()

    def path_of(sym):
        m = re.search(r'const struct Tileset %s =\s*\{(.*?)\};' % re.escape(sym), headers, re.S)
        tiles = re.search(r'\.tiles = (\w+)', m.group(1)).group(1)
        return re.search(r'%s\[\] = INCGFX_U32\("([^"]+)/tiles\.png"' % re.escape(tiles), gfx).group(1)

    prim_meta = git_bytes('data/tilesets/primary/general_frlg/metatiles.bin')
    prim_attr = git_bytes('data/tilesets/primary/general_frlg/metatile_attributes.bin')
    sec_meta = git_bytes(tileset + '/metatiles.bin')
    sec_attr = git_bytes(tileset + '/metatile_attributes.bin')

    def plain(mid):
        if mid < NUM_METATILES_IN_PRIMARY:
            meta, attr, i = prim_meta, prim_attr, mid
        else:
            meta, attr, i = sec_meta, sec_attr, mid - NUM_METATILES_IN_PRIMARY
            if i * 16 >= len(meta):
                return False
        a = struct.unpack_from('<I', attr, i * 4)[0]
        top = struct.unpack_from('<8H', meta, i * 16)[4:]
        return (a & 0x1FF) == 0 and ((a >> 24) & 7) == 0 and all(e & 0x3FF == 0 for e in top)

    count = collections.Counter()
    for l in layouts:
        if l.get('layout_version') != 'frlg' or l['primary_tileset'] != 'gTileset_General_Frlg':
            continue
        if path_of(l['secondary_tileset']) != tileset or not re.match(r'^LAYOUT_(ROUTE|[A-Z]+_(CITY|TOWN|ISLAND)|INDIGO)', l['id']):
            continue
        raw = git_bytes(l['blockdata_filepath'])
        for v in struct.unpack('<%dH' % (len(raw) // 2), raw):
            if (v >> 10) & 3 == 0:
                count[v & 0x3FF] += 1
    return [mid for mid, _ in count.most_common() if plain(mid)][:AUTO_GROUND_COUNT]


def tileset_maps(tileset):
    """(blocks, width) of the original Kanto town/route layouts that use this tileset"""
    import re
    if not hasattr(tileset_maps, 'cache'):
        tileset_maps.cache = {}
    if tileset in tileset_maps.cache:
        return tileset_maps.cache[tileset]
    layouts = json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']
    headers = open(os.path.join(ROOT, 'src/data/tilesets/headers.h')).read()
    gfx = open(os.path.join(ROOT, 'src/data/tilesets/graphics.h')).read()
    out = []
    for l in layouts:
        if l.get('layout_version') != 'frlg' or l['primary_tileset'] != 'gTileset_General_Frlg':
            continue
        if not re.match(r'^LAYOUT_(ROUTE|[A-Z]+_(CITY|TOWN|ISLAND)|INDIGO)', l['id']):
            continue
        m = re.search(r'const struct Tileset %s =\s*\{(.*?)\};' % re.escape(l['secondary_tileset']), headers, re.S)
        tiles = re.search(r'\.tiles = (\w+)', m.group(1)).group(1)
        if re.search(r'%s\[\] = INCGFX_U32\("([^"]+)/tiles\.png"' % re.escape(tiles), gfx).group(1) != tileset:
            continue
        raw = git_bytes(l['blockdata_filepath'])
        out.append((struct.unpack('<%dH' % (len(raw) // 2), raw), l['width']))
    tileset_maps.cache[tileset] = out
    return out


def roof_sources(tileset, window_mids):
    """Roof metatiles: solid cells 2-3 rows above the doors (warps) of the buildings on this city's maps."""
    import collections, glob, re
    layouts = {l['id']: l for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']}
    count = collections.Counter()
    for mj in glob.glob(os.path.join(ROOT, 'data/maps/*/map.json')):
        m = json.load(open(mj))
        l = layouts.get(m.get('layout'))
        if not l or l.get('layout_version') != 'frlg' or not re.match(r'^LAYOUT_([A-Z]+_(CITY|TOWN|ISLAND)|ROUTE)', l['id']):
            continue
        raw = git_bytes(l['blockdata_filepath'])
        blocks, w = struct.unpack('<%dH' % (len(raw) // 2), raw), l['width']
        if (blocks, w) not in tileset_maps(tileset):
            continue
        for wp in m.get('warp_events') or []:
            for k in (2, 3):  # roof body, centred over the door
                for dx in (-1, 0, 1):
                    x, y = wp['x'] + dx, wp['y'] - k
                    if 0 <= x < w and 0 <= y < len(blocks) // w:
                        u = blocks[y * w + x]
                        if (u >> 10) & 3 and u & 0x3FF not in window_mids and u & 0x3FF >= NUM_METATILES_IN_PRIMARY:
                            count[u & 0x3FF] += 1
    return [mid for mid, n in count.most_common(ROOF_SOURCES_MAX) if n >= 2]


def build(tileset, grounds, preview_list=None):
    tiles_img = Image.open(BytesIO(git_bytes(tileset + '/tiles.png')))
    metatiles = bytearray(git_bytes(tileset + '/metatiles.bin'))
    attrs = bytearray(git_bytes(tileset + '/metatile_attributes.bin'))
    prim_meta = open(os.path.join(ROOT, 'data/tilesets/primary/general_frlg/metatiles.bin'), 'rb').read()

    w_tiles = tiles_img.width // 8
    n_tiles = w_tiles * (tiles_img.height // 8)
    tile_data = []
    px = list(tiles_img.getdata()) if hasattr(tiles_img, 'getdata') else None
    for t in range(n_tiles):
        tx, ty = (t % w_tiles) * 8, (t // w_tiles) * 8
        tile_data.append(tuple(px[(ty + y) * tiles_img.width + tx + x] for y in range(8) for x in range(8)))
    # Duplicate tiles of the original tileset: point the metatiles at the first copy and reuse the slot.
    protected = set(ANIM_TILES.get(tileset, ()))
    lookup, free = {}, []
    remap = {}
    for i, t in enumerate(tile_data):
        if t in lookup and i not in protected and lookup[t] not in protected:
            remap[i] = lookup[t]
            free.append(i)
        elif i not in protected:
            lookup.setdefault(t, i)
    for k in range(0, len(metatiles), 2):
        e = struct.unpack_from('<H', metatiles, k)[0]
        t = (e & 0x3FF) - NUM_TILES_IN_PRIMARY
        if t in remap:
            struct.pack_into('<H', metatiles, k, (e & ~0x3FF) | (remap[t] + NUM_TILES_IN_PRIMARY))

    def tile_id(pixels):
        if pixels not in lookup:
            if free:
                slot = free.pop(0)
                tile_data[slot] = pixels
            else:
                slot = len(tile_data)
                tile_data.append(pixels)
            lookup[pixels] = slot
        return NUM_TILES_IN_PRIMARY + lookup[pixels]

    def tiles_used():
        return len(tile_data) - len(free)

    ids = {}
    # building damage first (it is what makes a city look destroyed), then the props by priority
    build_variants(tileset, metatiles, attrs, tile_data, tile_id, tiles_used, ids)

    if grounds is None:
        auto = auto_grounds(tileset)
        seen = set()
        jobs, extra = [], []
        for name, fn, seed, ground, solid in PROPS:
            base = name.replace('_dirt', '').replace('_road', '')
            if base in seen:
                continue
            seen.add(base)
            small = len(canvas_tiles(fn(seed))) == 1
            for k, gmid in enumerate(auto[:PROP_MAX_GROUNDS.get(base, len(auto))]):
                if k < AUTO_GROUND_FIRST:
                    jobs.append(('%s@%d' % (base, gmid), fn, seed, gmid, solid))
                elif small:
                    extra.append(('%s@%d' % (base, gmid), fn, seed, gmid, solid))
        jobs += extra
    else:
        jobs = [(name, fn, seed, grounds[ground], solid) for name, fn, seed, ground, solid in PROPS]

    for name, fn, seed, ground_mid, solid in jobs:
        c = fn(seed)
        new_tiles = {q for quads in canvas_tiles(c) for q in quads if q not in lookup}
        if tiles_used() + len(new_tiles) > MAX_SECONDARY_TILES:
            continue  # tileset is full: leave this prop out here
        if len(metatiles) // 16 + len(canvas_tiles(c)) > MAX_SECONDARY_METATILES:
            continue
        if ground_mid < NUM_METATILES_IN_PRIMARY:
            ground_entries = struct.unpack_from('<4H', prim_meta, ground_mid * 16)
        else:
            ground_entries = struct.unpack_from('<4H', metatiles, (ground_mid - NUM_METATILES_IN_PRIMARY) * 16)
        mids = []
        for quads in canvas_tiles(c):
            top = [tile_id(q) | (DECO_PAL_SLOT << 12) for q in quads]
            mid = NUM_METATILES_IN_PRIMARY + len(metatiles) // 16
            metatiles += struct.pack('<8H', *ground_entries, *top)
            attrs += struct.pack('<I', LAYER_TYPE_COVERED << 29)
            mids.append(mid)
        ids[name] = {'metatiles': mids, 'solid': solid, 'cols': c.w // 16}
        if grounds is None:
            ids[name]['ground_mid'] = ground_mid
        if preview_list is not None:
            preview_list.append((name, c, 'dirt'))

    print('%s: %d/%d tiles, %d/%d metatiles, %d props' % (os.path.basename(tileset), tiles_used(), MAX_SECONDARY_TILES,
          len(metatiles) // 16, MAX_SECONDARY_METATILES, sum(1 for v in ids.values() if 'source' not in v)))
    assert len(tile_data) <= 1024 - NUM_TILES_IN_PRIMARY, 'too many tiles'
    assert len(metatiles) // 16 <= 1024 - NUM_METATILES_IN_PRIMARY, 'too many metatiles'

    rows = (len(tile_data) + w_tiles - 1) // w_tiles
    out = Image.new('P', (tiles_img.width, rows * 8))
    out.putpalette(tiles_img.getpalette())
    flat = [0] * (out.width * out.height)
    for i, t in enumerate(tile_data):
        tx, ty = (i % w_tiles) * 8, (i // w_tiles) * 8
        for k, v in enumerate(t):
            flat[(ty + k // 8) * out.width + tx + k % 8] = v
    out.putdata(flat)
    out.save(os.path.join(ROOT, tileset, 'tiles.png'), bits=4)
    open(os.path.join(ROOT, tileset, 'metatiles.bin'), 'wb').write(metatiles)
    open(os.path.join(ROOT, tileset, 'metatile_attributes.bin'), 'wb').write(attrs)
    with open(os.path.join(ROOT, tileset, 'palettes', '%02d.pal' % DECO_PAL_SLOT), 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n16\n' + ''.join('%d %d %d\n' % c for c in DECO_PAL))
    return ids


# Glass tiles of the standard Kanto house window (primary tileset, used by houses in every city).
WINDOW_REFERENCE_METATILES = (677, 678, 679)  # Viridian house window (left, middle, right)


def window_glass_tiles(tile_pixels):
    vir = git_bytes('data/tilesets/secondary/viridian_city_frlg/metatiles.bin')
    pal3 = read_pal_text(git_bytes('data/tilesets/primary/general_frlg/palettes/03.pal').decode())
    tiles = set()
    for mid in WINDOW_REFERENCE_METATILES:
        for e in struct.unpack_from('<8H', vir, (mid - NUM_METATILES_IN_PRIMARY) * 16)[4:]:
            t = e & 0x3FF
            if t < NUM_TILES_IN_PRIMARY and (e >> 12) & 0xF == 3 and any(v and is_glass(pal3[v]) for v in tile_pixels(e)):
                tiles.add(t)
    return tiles


def map_usage(tileset):
    import collections
    count = collections.Counter()
    for blocks, _ in tileset_maps(tileset):
        count.update(v & 0x3FF for v in blocks)
    return count


def auto_window_variants(tileset, metatiles, attrs, base_count, tile_pixels, pal):
    """Window metatiles of this city: the standard Kanto house window, or plenty of glass pixels in the top layer.
    The most used ones on the maps get a boarded variant."""
    glass_tiles = window_glass_tiles(tile_pixels)
    usage = map_usage(tileset)
    found = []
    for i in range(base_count):
        entries = struct.unpack_from('<8H', metatiles, i * 16)
        behavior = struct.unpack_from('<I', attrs, i * 4)[0] & 0x1FF
        mid = NUM_METATILES_IN_PRIMARY + i
        if any((e & 0x3FF) in glass_tiles for e in entries[4:]):
            found.append(mid)
            continue
        glass = 0
        for e in entries[4:]:
            opal = pal((e >> 12) & 0xF, False)
            glass += sum(1 for v in tile_pixels(e) if v and is_glass(opal[v]))
        if behavior == 0 and glass >= AUTO_WINDOW_MIN_GLASS and usage[mid]:
            found.append(mid)
    found.sort(key=lambda m: -usage[m])
    return [('window_%d' % mid, mid, ov_boards, mid) for mid in found[:AUTO_WINDOW_MAX]]


def build_variants(tileset, metatiles, attrs, sec_tiles, tile_id, tiles_used, ids):
    prim_dir = 'data/tilesets/primary/general_frlg'
    prim_meta = open(os.path.join(ROOT, prim_dir, 'metatiles.bin'), 'rb').read()
    prim_attr = open(os.path.join(ROOT, prim_dir, 'metatile_attributes.bin'), 'rb').read()
    prim_img = Image.open(os.path.join(ROOT, prim_dir, 'tiles.png'))
    prim_px = list(prim_img.getdata())
    pw = prim_img.width // 8

    def pal(slot, current):
        d = prim_dir if slot < 7 else tileset
        path = '%s/palettes/%02d.pal' % (d, slot)
        if current:
            return read_pal_text(open(os.path.join(ROOT, path)).read())
        return read_pal_text(git_bytes(path).decode())

    def tile_pixels(entry):
        t = entry & 0x3FF
        if t < NUM_TILES_IN_PRIMARY:
            px = [prim_px[((t // pw) * 8 + y) * prim_img.width + (t % pw) * 8 + x] for y in range(8) for x in range(8)]
        else:
            px = list(sec_tiles[t - NUM_TILES_IN_PRIMARY])
        if entry & 0x400:
            px = [px[y * 8 + 7 - x] for y in range(8) for x in range(8)]
        if entry & 0x800:
            px = [px[(7 - y) * 8 + x] for y in range(8) for x in range(8)]
        return px

    base_count = len(git_bytes(tileset + '/metatiles.bin')) // 16
    variants = VARIANTS.get(tileset)
    windows = auto_window_variants(tileset, metatiles, attrs, base_count, tile_pixels, pal)
    if variants is None:
        variants = list(COMMON_VARIANTS) + windows
    else:
        variants = list(variants) + COMMON_VARIANTS[-3:]
    window_mids = {src for _, src, _, _ in windows}
    roofs = roof_sources(tileset, window_mids)
    variants += [('roof_hole_%d' % mid, mid, ov_roof_hole, mid) for mid in roofs]
    late = [('window_scorch_%d' % src, src, ov_scorch, src + 7) for _, src, _, _ in windows[:WINDOW_SCORCH_MAX]]
    tile_limit = tiles_used() + int((MAX_SECONDARY_TILES - tiles_used()) * VARIANT_SHARE)
    meta_limit = len(metatiles) // 16 + int((MAX_SECONDARY_METATILES - len(metatiles) // 16) * VARIANT_SHARE)
    def add_overlay(name, src, overlay, seed):
        if tiles_used() + 4 > tile_limit or len(metatiles) // 16 + 1 > meta_limit:
            return
        if src < NUM_METATILES_IN_PRIMARY:
            entries = struct.unpack_from('<8H', prim_meta, src * 16)
            attr = struct.unpack_from('<I', prim_attr, src * 4)[0]
        else:
            i = src - NUM_METATILES_IN_PRIMARY
            assert i < base_count
            entries = struct.unpack_from('<8H', metatiles, i * 16)
            attr = struct.unpack_from('<I', attrs, i * 4)[0]
        top = entries[4:]
        quads = [(e, tile_pixels(e), (e >> 12) & 0xF) for e in top]
        orig = [[None] * 16 for _ in range(16)]
        for q, (e, px, slot) in enumerate(quads):
            opal = pal(slot, False)
            for k, v in enumerate(px):
                if v:
                    orig[(q // 2) * 8 + k // 8][(q % 2) * 8 + k % 8] = opal[v]
        over = overlay(orig, random.Random(seed))
        new_top = []
        for q, (e, px, slot) in enumerate(quads):
            cur = pal(slot, True)
            cells = [over[(q // 2) * 8 + k // 8][(q % 2) * 8 + k % 8] for k in range(64)]
            if all(c is None for c in cells):
                new_top.append(e)
                continue
            used = sorted({v for v in px if v}, key=lambda i: sum(cur[i]))
            if not used:
                used = list(range(1, 16))
            tokens = {DARK: used[0], LIGHT: used[-1], LIGHT2: used[-2] if len(used) > 1 else used[-1]}

            def resolve(k):
                c = cells[k]
                if c is None:
                    return None
                if c == CONTRAST:
                    return used[-1] if (px[k] and sum(cur[px[k]]) < 330) else used[0]
                if isinstance(c, str):
                    return tokens[c]
                return c

            resolved = [resolve(k) for k in range(64)]
            rgbs = [c for c in resolved if isinstance(c, tuple)]
            if rgbs and max(nearest(cur, c)[1] for c in rgbs) > 48:
                # palette has no fitting color (e.g. red): redraw the whole tile with the deco palette
                new_px = []
                for k, v in enumerate(px):
                    c = resolved[k]
                    c = cur[c] if isinstance(c, int) else c
                    if c is None:
                        c = cur[v] if v else None
                    new_px.append(0 if c is None else nearest(DECO_PAL, c)[0])
                new_slot = DECO_PAL_SLOT
            else:
                new_px = [v if resolved[k] is None else (resolved[k] if isinstance(resolved[k], int) else nearest(cur, resolved[k])[0])
                          for k, v in enumerate(px)]
                new_slot = slot
            new_top.append(tile_id(tuple(new_px)) | (new_slot << 12))
        if list(new_top) == list(top):
            return  # overlay found nothing to paint on (e.g. roof on the bottom layer)
        mid = NUM_METATILES_IN_PRIMARY + len(metatiles) // 16
        metatiles.extend(struct.pack('<8H', *entries[:4], *new_top))
        attrs.extend(struct.pack('<I', attr))
        kind = 'roof' if overlay is ov_roof_hole else 'damage'
        ids[name] = {'metatiles': [mid], 'solid': None, 'cols': 1, 'source': src, 'kind': kind}

    # burnt nature: charred forest, scorched ground and grass (whole metatile recoloured into the deco palette)
    # colours of the plain ground (per palette slot): they stay, only trees / grass blades get charred
    ground_cols = set()
    for gmid in GROUND_REFERENCE_METATILES:
        for e in struct.unpack_from('<4H', prim_meta, gmid * 16):
            ground_cols |= {((e >> 12) & 0xF, v) for v in tile_pixels(e) if v}

    def add_burnt(src):
        if tiles_used() + 8 > tile_limit or len(metatiles) // 16 + 1 > meta_limit:
            return
        if src < NUM_METATILES_IN_PRIMARY:
            entries = struct.unpack_from('<8H', prim_meta, src * 16)
            attr = struct.unpack_from('<I', prim_attr, src * 4)[0]
        else:
            entries = struct.unpack_from('<8H', metatiles, (src - NUM_METATILES_IN_PRIMARY) * 16)
            attr = struct.unpack_from('<I', attrs, (src - NUM_METATILES_IN_PRIMARY) * 4)[0]
        new = []
        for e in entries:
            px = tile_pixels(e)
            slot = (e >> 12) & 0xF
            keep = {v for s_, v in ground_cols if s_ == slot}
            if not any(v and v not in keep for v in px):
                new.append(e)  # plain ground: unchanged
                continue
            new.append(tile_id(burn_tile(px, pal(slot, True), keep)) | (slot << 12))
        mid = NUM_METATILES_IN_PRIMARY + len(metatiles) // 16
        metatiles.extend(struct.pack('<8H', *new))
        attrs.extend(struct.pack('<I', attr))
        ids['burnt_%d' % src] = {'metatiles': [mid], 'solid': None, 'cols': 1, 'source': src, 'kind': 'burnt'}

    n_first = len(COMMON_VARIANTS) if tileset not in VARIANTS else len(variants)
    for v in variants[:n_first]:
        add_overlay(*v)
    for src in list(TREE_METATILES) + [TALL_GRASS_METATILE]:
        add_burnt(src)
    for v in variants[n_first:]:
        add_overlay(*v)
    for v in late:
        add_overlay(*v)


def burn_tile(px, pal, keep):
    """Charred version of a tile in its own palette: tree / grass pixels become the darkest colours of the palette,
    ground colours (keep) stay. Deterministic per tile, so shared tiles stay shared."""
    import zlib
    rng = random.Random(zlib.crc32(bytes(px)))
    lum = [0.3 * r + 0.59 * g + 0.11 * b for r, g, b in pal]
    ramp = sorted(range(1, 16), key=lambda i: lum[i])[:4]  # darkest colours, darkest first
    lo, hi = min(lum[v] for v in px if v), max(lum[v] for v in px if v) + 1
    out = []
    for v in px:
        if not v or v in keep:
            out.append(v)
            continue
        k = int((lum[v] - lo) / (hi - lo) * len(ramp))
        if rng.random() < 0.12:
            k = 0  # soot
        out.append(ramp[min(k, len(ramp) - 1)])
    return tuple(out)


def main():
    preview = [] if '--preview' in sys.argv else None
    all_ids = {}
    for tileset, grounds in TILESETS.items():
        all_ids[tileset] = build(tileset, grounds, preview)
        preview_target = preview
        preview = None  # one preview is enough
        if preview_target is not None:
            save_preview(preview_target, sys.argv[sys.argv.index('--preview') + 1])
    with open(os.path.join(ROOT, 'tools/apocalypse/deco_ids.json'), 'w') as f:
        json.dump(all_ids, f, indent=2, sort_keys=True)
        f.write('\n')


def save_preview(items, path):
    width = sum(c.w + 8 for _, c, _ in items)
    img = Image.new('RGB', (width, 24), (90, 80, 70))
    x = 4
    for name, c, ground in items:
        bg = (112, 96, 72) if ground == 'dirt' else (140, 132, 128)
        for yy in range(16):
            for xx in range(c.w):
                v = c.p[yy][xx]
                img.putpixel((x + xx, 4 + yy), DECO_PAL[v] if v else bg)
        x += c.w + 8
    img.resize((img.width * 4, img.height * 4), Image.NEAREST).save(path)


if __name__ == '__main__':
    main()
