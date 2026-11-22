#!/usr/bin/env python3
"""Gangster Outbreak: battle backgrounds in the look of the ruined, gang-ruled Kanto.

Writes graphics/battle_environment/gang/<scene>/{tiles.png,map.bin,palette.pal} for
  street (towns, routes, sand, mountains), grass, water, cave, building and one gym per gang,
plus an empty entry animation (graphics/battle_environment/gang/entry_*).

Every scene is drawn in full colour (256x112, the part above the text box) with three hand-made
palettes; each 8x8 tile picks the palette that fits it best. The platforms sit exactly where the
original ones are (the Pokémon sprites are placed for them).

Usage: python3 tools/apocalypse/battle_bg.py [--preview DIR]
"""
import math
import os
import random
import struct
import sys

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
import gyms  # noqa: E402  (gang colours and emblems)

W, H = 256, 112          # bg3 area above the text box
HORIZON = 48             # first ground row (tile aligned: sky tiles and ground tiles use different palettes)
ENEMY = (176, 64, 60, 15)   # platform centre x, y, radius x, radius y
PLAYER = (64, 116, 66, 17)


# ------------------------------------------------------------------ drawing helpers

class Canvas:
    def __init__(self):
        self.p = [[(0, 0, 0)] * W for _ in range(H)]

    def set(self, x, y, c):
        if 0 <= x < W and 0 <= y < H:
            self.p[int(y)][int(x)] = c

    def get(self, x, y):
        return self.p[y][x]

    def rect(self, x0, y0, x1, y1, c):
        for y in range(max(0, y0), min(H, y1)):
            for x in range(max(0, x0), min(W, x1)):
                self.p[y][x] = c

    def image(self):
        im = Image.new('RGB', (W, H))
        im.putdata([c for row in self.p for c in row])
        return im


BAYER = [[0, 8, 2, 10], [12, 4, 14, 6], [3, 11, 1, 9], [15, 7, 13, 5]]


def gradient(cv, y0, y1, colours):
    """vertical gradient through the colours, ordered dithering between neighbours"""
    n = len(colours) - 1
    for y in range(y0, y1):
        t = (y - y0) / max(1, (y1 - y0 - 1)) * n
        i = min(int(t), n - 1)
        f = t - i
        for x in range(W):
            cv.set(x, y, colours[i + 1] if f * 16 > BAYER[y % 4][x % 4] + 0.5 else colours[i])


def ellipse_inside(x, y, cx, cy, rx, ry):
    return ((x + 0.5 - cx) / rx) ** 2 + ((y + 0.5 - cy) / ry) ** 2


# ------------------------------------------------------------------ scene parts

def dusk_sky(cv, pal, rng):
    gradient(cv, 0, HORIZON, pal['sky'])
    for _ in range(5):  # smoke columns drifting right
        x, y = rng.randint(10, 250), rng.randint(14, 40)
        for i in range(26):
            r = 2 + i // 6
            for dy in range(-r, r + 1):
                for dx in range(-r, r + 1):
                    if dx * dx + dy * dy <= r * r and rng.random() < 0.55:
                        cv.set(x + dx + i // 2, y - i + dy, pal['smoke'])


def skyline(cv, pal, rng, base=HORIZON):
    """two layers of ruined high-rises: broken tops, a few lit windows, burning spots"""
    for layer, (colour, hmin, hmax) in enumerate(((pal['far'], 10, 26), (pal['near'], 6, 20))):
        x = rng.randint(-8, 0)
        while x < W:
            w = rng.randint(14, 30) if layer == 0 else rng.randint(10, 22)
            h = rng.randint(hmin, hmax)
            top = base - h
            broken = rng.random() < 0.6
            for xx in range(x, x + w):
                t = top
                if broken:  # jagged, collapsed roof
                    t += abs(int(4 * math.sin((xx - x) * 0.9 + w))) + (rng.random() < 0.3)
                for y in range(t, base):
                    cv.set(xx, y, colour)
            if layer == 1:
                for wy in range(top + 4, base - 2, 4):
                    for wx in range(x + 2, x + w - 2, 4):
                        r = rng.random()
                        if r < 0.10:
                            cv.set(wx, wy, pal['window']); cv.set(wx + 1, wy, pal['window'])
                        elif r < 0.14:
                            cv.set(wx, wy, pal['fire']); cv.set(wx + 1, wy, pal['glow']); cv.set(wx, wy - 1, pal['glow'])
            x += w + rng.randint(0, 6)


def textured_ground(cv, colours, rng, y0=HORIZON, y1=H, speck=None, specks=0.0):
    gradient(cv, y0, y1, colours)
    if speck:
        for y in range(y0, y1):
            for x in range(W):
                if rng.random() < specks:
                    cv.set(x, y, speck)


def cracks(cv, colour, rng, n, y0=HORIZON + 2):
    for _ in range(n):
        x, y = rng.randint(0, W), rng.randint(y0, H - 1)
        for _ in range(rng.randint(6, 18)):
            cv.set(x, y, colour)
            x += rng.choice((-1, 1, 1))
            y += rng.choice((0, 0, 1, -1))


def platform(cv, centre, rim, fills, ring=None, ring2=None, rng=None, deco=None):
    cx, cy, rx, ry = centre
    for y in range(int(cy - ry - 2), int(cy + ry + 3)):
        for x in range(int(cx - rx - 2), int(cx + rx + 3)):
            d = ellipse_inside(x, y, cx, cy, rx, ry)
            if d > 1:
                continue
            if d > 0.82:
                c = rim
            elif ring and 0.46 < d < 0.6:
                c = ring
            elif ring2 and 0.6 <= d < 0.66:
                c = ring2
            else:
                # lighter towards the top-left
                shade = (x - cx) / rx * 0.5 + (y - cy) / ry
                i = 0 if shade < -0.3 else 1 if shade < 0.35 else 2
                c = fills[min(i, len(fills) - 1)]
                if deco and rng.random() < deco[1]:
                    c = deco[0]
            cv.set(x, y, c)
    # shadow under the near edge
    for x in range(int(cx - rx + 6), int(cx + rx - 6)):
        y = int(cy + ry * math.sqrt(max(0, 1 - ((x + 0.5 - cx) / rx) ** 2))) + 1
        cv.set(x, y, rim)


# ------------------------------------------------------------------ the scenes

SKY = [(28, 18, 36), (44, 26, 50), (68, 34, 58), (100, 44, 62), (144, 62, 56), (196, 98, 58), (236, 150, 78)]
SKY_PAL = dict(sky=SKY, smoke=(84, 66, 80), far=(60, 42, 66), near=(30, 22, 34), window=(236, 184, 80),
               fire=(244, 112, 40), glow=(176, 72, 40))


def scene_street(rng, platforms=True):
    cv = Canvas()
    dusk_sky(cv, SKY_PAL, rng)
    skyline(cv, SKY_PAL, rng)
    asphalt = [(58, 54, 60), (66, 62, 68), (76, 72, 78), (88, 84, 90)]
    textured_ground(cv, asphalt, rng, speck=(40, 36, 44), specks=0.05)
    cv.rect(0, HORIZON, W, HORIZON + 3, (108, 104, 108))        # curb
    cv.rect(0, HORIZON + 3, W, HORIZON + 4, (40, 36, 44))
    for x in range(0, W, 24):                                    # faded lane paint
        for i in range(10):
            if rng.random() < 0.8:
                cv.set(x + i, 88, (196, 164, 64))
                cv.set(x + i + 1, 89, (140, 118, 58))
    cracks(cv, (40, 36, 44), rng, 40)
    for _ in range(60):                                          # rubble
        x, y = rng.randint(0, W), rng.randint(HORIZON + 5, H)
        cv.set(x, y, (124, 116, 110)); cv.set(x + 1, y, (88, 84, 90))
    if platforms:
        for p in (ENEMY, PLAYER):
            platform(cv, p, (34, 30, 38), [(140, 134, 128), (118, 112, 108), (96, 92, 94)],
                     ring=(176, 44, 40), ring2=(118, 30, 32), rng=rng, deco=((76, 72, 78), 0.06))
    return cv


def scene_grass(rng, platforms=True):
    cv = Canvas()
    dusk_sky(cv, SKY_PAL, rng)
    skyline(cv, SKY_PAL, rng)
    grass = [(38, 58, 34), (50, 74, 40), (64, 92, 48), (80, 110, 56)]
    textured_ground(cv, grass, rng, speck=(118, 112, 62), specks=0.06)
    for _ in range(260):                                         # blades
        x, y = rng.randint(0, W), rng.randint(HORIZON + 2, H)
        for i in range(rng.randint(2, 4)):
            cv.set(x, y - i, (28, 44, 28) if i == 0 else (96, 128, 60))
    for _ in range(14):                                          # wreck parts in the weeds
        x, y = rng.randint(0, W), rng.randint(HORIZON + 4, H - 4)
        cv.rect(x, y, x + rng.randint(3, 7), y + 2, (88, 84, 90))
    if platforms:
        for p in (ENEMY, PLAYER):
            platform(cv, p, (58, 42, 30), [(142, 110, 72), (120, 90, 58), (100, 74, 48)],
                     rng=rng, deco=((80, 110, 56), 0.08))
    return cv


def scene_water(rng, platforms=True):
    cv = Canvas()
    dusk_sky(cv, SKY_PAL, rng)
    skyline(cv, SKY_PAL, rng)
    water = [(22, 40, 62), (28, 52, 80), (38, 68, 98), (52, 88, 118)]
    textured_ground(cv, water, rng)
    for _ in range(90):                                          # ripples and the burning sky mirrored
        x, y = rng.randint(0, W), rng.randint(HORIZON + 1, H)
        c = (150, 188, 208) if rng.random() < 0.6 else (196, 110, 66)
        for i in range(rng.randint(4, 10)):
            cv.set(x + i, y, c)
    for _ in range(8):                                           # sunken poles
        x = rng.randint(0, W)
        cv.rect(x, HORIZON - rng.randint(4, 12), x + 2, HORIZON + 4, (30, 22, 34))
    if platforms:
        for p in (ENEMY, PLAYER):
            platform(cv, p, (26, 30, 38), [(132, 128, 124), (110, 106, 104), (90, 88, 90)],
                     ring=(176, 44, 40), rng=rng, deco=((58, 54, 60), 0.06))
    return cv


def scene_cave(rng, platforms=True):
    cv = Canvas()
    rock = [(14, 12, 18), (22, 20, 28), (32, 28, 38), (44, 38, 50)]
    gradient(cv, 0, HORIZON, rock)
    for _ in range(18):                                          # stalactites / broken pipes
        x = rng.randint(0, W)
        h = rng.randint(6, 26)
        for y in range(h):
            half = max(0, (h - y) // 5)
            for dx in range(-half, half + 1):
                cv.set(x + dx, y, (52, 46, 58) if dx < 0 else (24, 20, 30))
    for x0 in (36, 150):                                         # flickering work lamps
        for y in range(0, 16):
            cv.set(x0, y, (70, 64, 72))
        for dy in range(-3, 4):
            for dx in range(-3, 4):
                if dx * dx + dy * dy <= 9:
                    cv.set(x0 + dx, 18 + dy, (236, 184, 80) if dx * dx + dy * dy <= 2 else (150, 100, 52))
    floor = [(36, 32, 40), (48, 42, 52), (60, 54, 62), (72, 66, 72)]
    textured_ground(cv, floor, rng, speck=(24, 20, 28), specks=0.08)
    cracks(cv, (22, 20, 28), rng, 30)
    if platforms:
        for p in (ENEMY, PLAYER):
            platform(cv, p, (20, 18, 24), [(104, 96, 100), (86, 80, 86), (70, 64, 72)],
                     rng=rng, deco=((48, 42, 52), 0.08))
    return cv


def brick_wall(cv, rng, bricks, mortar, y1=HORIZON):
    for y in range(0, y1):
        for x in range(W):
            row = y // 4
            off = 4 if row % 2 else 0
            if y % 4 == 3 or (x + off) % 8 == 7:
                c = mortar
            else:
                c = bricks[(row * 7 + (x + off) // 8 * 3) % len(bricks)]
            cv.set(x, y, c)


def graffiti(cv, rng, colours, n=6, y0=6, y1=HORIZON - 4):
    for _ in range(n):
        x, y = rng.randint(0, W - 30), rng.randint(y0, y1 - 8)
        c, c2 = rng.choice(colours), rng.choice(colours)
        for i in range(rng.randint(14, 30)):  # fat wavy tag
            yy = y + int(3 * math.sin(i * 0.7))
            for t in range(3):
                cv.set(x + i, yy + t, c)
            cv.set(x + i, yy + 3, c2)


def scene_building(rng, platforms=True):
    cv = Canvas()
    brick_wall(cv, rng, [(96, 50, 42), (110, 60, 48), (82, 44, 38)], (52, 40, 42))
    graffiti(cv, rng, [(232, 64, 152), (64, 200, 216), (240, 200, 64), (224, 232, 224)], n=7)
    for x0 in (12, 196):  # boarded windows
        cv.rect(x0, 10, x0 + 34, 34, (24, 20, 28))
        for i in range(3):
            cv.rect(x0 - 2, 14 + i * 7, x0 + 36, 18 + i * 7, (120, 86, 52))
    floor = [(60, 58, 62), (70, 68, 72), (80, 78, 82), (92, 90, 94)]
    textured_ground(cv, floor, rng, speck=(44, 42, 48), specks=0.05)
    for x in range(0, W, 32):  # floor tiles seams
        for y in range(HORIZON, H):
            cv.set(x + (y - HORIZON) // 3, y, (48, 46, 52))
    cracks(cv, (40, 38, 44), rng, 18)
    if platforms:
        for p in (ENEMY, PLAYER):
            platform(cv, p, (30, 28, 34), [(128, 124, 128), (108, 104, 110), (90, 88, 94)],
                     ring=(216, 168, 40), ring2=(140, 104, 24), rng=rng)
    return cv


def scene_gym(gym_key):
    def build(rng, platforms=True):
        spec = gyms.GYMS[gym_key]
        tag = spec['tag']
        dark = tuple(max(0, c // 5) for c in tag)
        mid = tuple(max(0, c // 2) for c in tag)
        cv = Canvas()
        gradient(cv, 0, HORIZON, [(18, 16, 22), (26, 24, 32), (34, 32, 42)])
        for y in (6, 7, 40, 41):  # neon bands
            cv.rect(0, y, W, y + 1, tag if y in (6, 41) else mid)
        for x in range(0, W, 32):  # wall panels
            cv.rect(x, 8, x + 1, 40, (14, 12, 18))
        # emblems on the wall: between the enemy health box and the enemy Pokémon, and far right
        spots = [(112, 8), (202, 8)]
        for (ex, ey), em in zip(spots, (spec['emblems'] * 2)[:2]):
            for y in range(32):
                for x in range(32):
                    c = em.p[y][x]
                    if c is not None:
                        cv.set(ex + x, ey + y, c)
        floor = [(30, 28, 36), (38, 36, 44), (46, 44, 52), (54, 52, 60)]
        textured_ground(cv, floor, rng)
        cv.rect(0, HORIZON, W, HORIZON + 2, tag)
        for i in range(6):  # floor lines running towards the viewer
            x = 20 + i * 44
            for y in range(HORIZON + 2, H):
                cv.set(x + (y - HORIZON) * (i - 2.5) * 0.4, y, dark if y % 2 else mid)
        if platforms:
            for p in (ENEMY, PLAYER):
                platform(cv, p, (14, 12, 18), [(72, 68, 80), (60, 56, 68), (50, 46, 58)],
                         ring=tag, ring2=mid, rng=rng)
        return cv
    return build


SCENES = {
    'street': scene_street,
    'grass': scene_grass,
    'water': scene_water,
    'cave': scene_cave,
    'building': scene_building,
}
for key in gyms.GYMS:
    SCENES['gym_' + key.lower()] = scene_gym(key)


# ------------------------------------------------------------------ conversion to GBA tiles

def make_palettes(img, sky_rows):
    """three palettes of 15 colours: the sky part, the ground part and whatever does not fit"""
    def quant(region_tiles):
        im = Image.new('RGB', (8 * len(region_tiles), 8))
        for i, t in enumerate(region_tiles):
            im.paste(t, (8 * i, 0))
        q = im.quantize(colors=15, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
        p = (q.getpalette() + [0] * 45)[:45]
        return [tuple(p[i:i + 3]) for i in range(0, 45, 3)]

    tiles = [(x, y, img.crop((x, y, x + 8, y + 8))) for y in range(0, H, 8) for x in range(0, W, 8)]
    sky = [t for (x, y, t) in tiles if y < sky_rows * 8]
    ground = [t for (x, y, t) in tiles if y >= sky_rows * 8]
    pals = [quant(sky), quant(ground)]
    # third palette: tiles that the first two match badly (emblems, graffiti, lamps)
    bad = [t for (x, y, t) in tiles if min(tile_error(t, p) for p in pals) > 600]
    pals.append(quant(bad) if bad else pals[0])
    return pals


def nearest(c, pal):
    return min(range(len(pal)), key=lambda i: sum((a - b) ** 2 for a, b in zip(c, pal[i])))


def tile_error(tile, pal):
    return sum(min(sum((a - b) ** 2 for a, b in zip(c, p)) for p in pal) for c in tile.get_flattened_data()) / 64


def convert(img, flat):
    """img: 256x112 scene, flat: the same without platforms (second half of the tilemap)"""
    pals = make_palettes(img, HORIZON // 8)
    tiles, index, tilemap = [bytes(32)], {bytes(32): 0}, []

    def add(tile_img):
        errs = [tile_error(tile_img, p) for p in pals]
        pi = errs.index(min(errs))
        px = [nearest(c, pals[pi]) + 1 for c in tile_img.get_flattened_data()]
        data = bytes(px)
        if data not in index:
            index[data] = len(tiles)
            tiles.append(data)
        return index[data] | ((2 + pi) << 12)

    for src in (img, flat):
        for ty in range(32):
            for tx in range(32):
                if ty * 8 < H:
                    tilemap.append(add(src.crop((tx * 8, ty * 8, tx * 8 + 8, ty * 8 + 8))))
                else:
                    tilemap.append(0)
    return pals, tiles, tilemap


def write_scene(name, img, flat):
    pals, tiles, tilemap = convert(img, flat)
    assert len(tiles) <= 512, '%s: %d tiles' % (name, len(tiles))
    out = os.path.join(ROOT, 'graphics/battle_environment/gang', name)
    os.makedirs(out, exist_ok=True)
    cols = 16
    rows = (len(tiles) + cols - 1) // cols
    sheet = Image.new('P', (cols * 8, rows * 8))
    flat_pal = []
    for i in range(16):
        flat_pal += [0, 0, 0] if i == 0 else list(pals[0][i - 1])
    sheet.putpalette(flat_pal + [0] * (768 - 48))
    for i, t in enumerate(tiles):
        tx, ty = (i % cols) * 8, (i // cols) * 8
        for j, v in enumerate(t if isinstance(t, bytes) else bytes(t)):
            sheet.putpixel((tx + j % 8, ty + j // 8), v)
    sheet.save(os.path.join(out, 'tiles.png'))
    with open(os.path.join(out, 'map.bin'), 'wb') as f:
        f.write(b''.join(struct.pack('<H', e) for e in tilemap))
    lines = ['JASC-PAL', '0100', '48']
    for p in pals:
        lines += ['0 0 0'] + ['%d %d %d' % c for c in p]
    with open(os.path.join(out, 'palette.pal'), 'w') as f:
        f.write('\r\n'.join(lines) + '\r\n')
    return len(tiles)


def preview(name, img_path_dir):
    """rebuild the scene from the written files, as the GBA will show it"""
    out = os.path.join(ROOT, 'graphics/battle_environment/gang', name)
    sheet = Image.open(os.path.join(out, 'tiles.png'))
    lines = open(os.path.join(out, 'palette.pal')).read().split()[3:]
    vals = list(map(int, lines))
    pal = [tuple(vals[i:i + 3]) for i in range(0, len(vals), 3)]
    data = open(os.path.join(out, 'map.bin'), 'rb').read()
    im = Image.new('RGB', (240, H))
    px = sheet.load()
    for ty in range(H // 8):
        for tx in range(30):
            e = struct.unpack_from('<H', data, (ty * 32 + tx) * 2)[0]
            t, p = e & 0x3ff, (e >> 12) - 2
            for y in range(8):
                for x in range(8):
                    v = px[(t % 16) * 8 + x, (t // 16) * 8 + y]
                    im.putpixel((tx * 8 + x, ty * 8 + y), pal[p * 16 + v])
    im.save(os.path.join(img_path_dir, name + '.png'))


def main():
    for name, build in SCENES.items():
        img = build(random.Random(name)).image()
        flat = build(random.Random(name), platforms=False).image()
        n = write_scene(name, img, flat)
        print('%-24s %3d tiles' % (name, n))
    # empty entry animation (the slide-in layer stays clear)
    out = os.path.join(ROOT, 'graphics/battle_environment/gang')
    blank = Image.new('P', (8, 8))
    blank.putpalette([0] * 768)
    blank.save(os.path.join(out, 'entry_tiles.png'))
    with open(os.path.join(out, 'entry_map.bin'), 'wb') as f:
        f.write(bytes(2048))
    if '--preview' in sys.argv:
        d = sys.argv[sys.argv.index('--preview') + 1]
        os.makedirs(d, exist_ok=True)
        for name in SCENES:
            preview(name, d)


if __name__ == '__main__':
    main()
