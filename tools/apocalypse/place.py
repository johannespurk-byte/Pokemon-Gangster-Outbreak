#!/usr/bin/env python3
"""Gangster Outbreak: place the apocalypse deco (deco.py) on the maps.

Starts from the ORIGINAL map.bin in git (BASE_REF) and only replaces cells whose ground matches the prop's
ground, so paths, doors and events stay free. Re-run after changing the lists below.
"""
import json, os, struct, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE_REF = 'ddfe1f3d'
GROUND_IDS = {'dirt': (1, 8, 9), 'road': (366,)}

# layout dir -> (tileset, width, [(x, y, prop)])
PLACEMENTS = {
    'data/layouts/ViridianCity_Frlg': ('data/tilesets/secondary/viridian_city_frlg', 48, [
        # road
        (24, 21, 'car_road'), (34, 28, 'car_road'), (21, 15, 'barrel_road'), (40, 24, 'barrier_road'),
        (31, 24, 'rubble_road'), (38, 21, 'graffiti_road'), (21, 13, 'graffiti_road'),
        (21, 19, 'crack_a'), (28, 21, 'crack_b'), (31, 19, 'crack_c'), (37, 28, 'crack_a'),
        (21, 26, 'crack_b'), (36, 5, 'crack_c'), (40, 9, 'crack_a'), (25, 7, 'crack_b'),
        # dirt
        (10, 18, 'rubble_a'), (12, 18, 'rubble_b'), (14, 25, 'car_dirt'), (17, 27, 'tires_dirt'),
        (8, 29, 'barrel_dirt'), (26, 13, 'rubble_b'), (30, 16, 'rubble_a'), (38, 16, 'barrel_dirt'),
        (26, 31, 'tires_dirt'), (41, 33, 'rubble_a'), (8, 31, 'car_dirt'),
    ]),
    'data/layouts/Route2_Frlg': ('data/tilesets/secondary/viridian_city_frlg', 24, [
        (15, 3, 'rubble_a'), (20, 5, 'barrel_dirt'), (15, 18, 'rubble_b'), (16, 28, 'car_dirt'),
        (16, 30, 'tires_dirt'), (20, 33, 'rubble_b'), (16, 35, 'barrel_dirt'), (10, 53, 'rubble_a'),
        (6, 61, 'car_dirt'), (2, 58, 'tires_dirt'), (21, 71, 'barrier_dirt'),
    ]),
}


def main():
    ids = json.load(open(os.path.join(ROOT, 'tools/apocalypse/deco_ids.json')))
    import deco
    grounds = {name: ground for name, _, _, ground, _ in deco.PROPS}
    ok = True
    for layout, (tileset, width, items) in PLACEMENTS.items():
        rel = layout + '/map.bin'
        raw = subprocess.run(['git', '-C', ROOT, 'show', '%s:%s' % (BASE_REF, rel)], capture_output=True, check=True).stdout
        blocks = list(struct.unpack('<%dH' % (len(raw) // 2), raw))
        for x, y, prop in items:
            info = ids[tileset][prop]
            for i, mid in enumerate(info['metatiles']):
                idx = y * width + x + i
                cur = blocks[idx]
                if (cur & 0x3FF) not in GROUND_IDS[grounds[prop]] or (cur >> 10) & 3:
                    print('SKIP %s (%d,%d): ground is metatile %d, collision %d' % (prop, x + i, y, cur & 0x3FF, (cur >> 10) & 3))
                    ok = False
                    continue
                collision = 1 if info['solid'] else 0
                blocks[idx] = (cur & 0xF000) | (collision << 10) | mid
        open(os.path.join(ROOT, rel), 'wb').write(struct.pack('<%dH' % len(blocks), *blocks))
    return 0 if ok else 1


if __name__ == '__main__':
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    sys.exit(main())
