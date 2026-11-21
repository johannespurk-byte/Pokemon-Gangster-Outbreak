#!/usr/bin/env python3
"""Gangster Outbreak: place the apocalypse deco (deco.py) on the maps.

Starts from the ORIGINAL map.bin in git (BASE_REF), so it can be re-run after changing the lists below.

- Manual props: (x, y, prop). Only placed if every cell has the prop's ground (free dirt / road centre).
- Building swaps: (x, y, variant). Only swapped if the cell still has the variant's source metatile;
  x/y may be None to swap every occurrence on the map.
- Auto props: scattered with a fixed seed. A solid prop is only placed if all walkable areas of the map stay
  connected and it keeps a distance from events, warps and the map edges (connections).
"""
import json, os, random, struct, subprocess, sys
from collections import deque

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import deco  # noqa: E402

BASE_REF = deco.BASE_REF
GROUND_IDS = {'dirt': (1, 8, 9, 16, 17), 'road': (366, 367)}
EVENT_DISTANCE = 2
EDGE_DISTANCE = 3

VIRIDIAN = 'data/tilesets/secondary/viridian_city_frlg'

# layout dir -> settings
PLACEMENTS = {
    'data/layouts/ViridianCity_Frlg': {
        'map': 'data/maps/ViridianCity_Frlg/map.json', 'tileset': VIRIDIAN, 'width': 48,
        'props': [
            # road
            (24, 21, 'car_road'), (34, 28, 'car_road'), (21, 15, 'barrel_road'), (40, 24, 'barrier_road'),
            (31, 24, 'rubble_road'), (38, 21, 'graffiti_road'), (21, 13, 'graffiti_road'),
            (21, 19, 'crack_a'), (28, 21, 'crack_b'), (31, 19, 'crack_c'), (37, 28, 'crack_a'),
            (21, 26, 'crack_b'), (36, 5, 'crack_c'), (40, 9, 'crack_a'), (25, 7, 'crack_b'),
            # dirt
            (10, 18, 'rubble_a'), (12, 18, 'rubble_b'), (14, 25, 'car_dirt'), (17, 27, 'tires_dirt'),
            (8, 29, 'barrel_dirt'), (26, 13, 'rubble_b'), (30, 16, 'rubble_a'), (38, 16, 'barrel_dirt'),
            (26, 31, 'tires_dirt'), (41, 33, 'rubble_a'), (8, 31, 'car_dirt'),
            # barricade with a gap on the western plaza
            (9, 20, 'sandbags_dirt'), (10, 20, 'sandbags_dirt'), (11, 20, 'sandbags_dirt'),
        ],
        'swaps': [
            (None, None, 'house_window_boarded'), (None, None, 'house_window_left_boarded'),
            (None, None, 'house_wall_graffiti_a'), (None, None, 'house_wall_graffiti_b'),
            (None, None, 'gym_window_a_boarded'), (None, None, 'gym_window_b_boarded'),
            (None, None, 'gym_window_c_boarded'), (None, None, 'gym_wall_graffiti'),
            (None, None, 'gym_wall_graffiti_b'), (None, None, 'gym_rocket_r'),
            (None, None, 'mart_wall_graffiti'), (None, None, 'pc_wall_graffiti'),
        ],
        'auto': {'seed': 11, 'solid': ['scrap_heap_a', 'scrap_heap_b', 'car_flipped_dirt', 'rubble_a', 'rubble_b',
                                       'tires_dirt', 'barrel_dirt', 'car_flipped_road', 'crater_road', 'sandbags_road'],
                 'count': 18,
                 'decor': ['crack_a', 'crack_b', 'crack_c', 'crack_a', 'crack_b', 'graffiti_road'], 'decor_count': 12},
    },
    'data/layouts/Route2_Frlg': {
        'map': 'data/maps/Route2_Frlg/map.json', 'tileset': VIRIDIAN, 'width': 24,
        'props': [
            (15, 3, 'rubble_a'), (20, 5, 'barrel_dirt'), (15, 18, 'rubble_b'), (16, 28, 'car_dirt'),
            (16, 30, 'tires_dirt'), (20, 33, 'rubble_b'), (16, 35, 'barrel_dirt'), (10, 53, 'rubble_a'),
            (6, 61, 'car_dirt'), (2, 58, 'tires_dirt'), (21, 71, 'barrier_dirt'),
        ],
        'swaps': [],
        'auto': {'seed': 5, 'solid': ['scrap_heap_a', 'scrap_heap_b', 'car_flipped_dirt', 'rubble_a', 'rubble_b',
                                      'tires_dirt', 'barrel_dirt', 'sandbags_dirt'],
                 'count': 16, 'decor': [], 'decor_count': 0},
    },
}


class Grid:
    def __init__(self, blocks, width):
        self.b = blocks
        self.w = width
        self.h = len(blocks) // width

    def mid(self, x, y):
        return self.b[y * self.w + x] & 0x3FF

    def col(self, x, y):
        return (self.b[y * self.w + x] >> 10) & 3

    def set(self, x, y, mid, collision):
        i = y * self.w + x
        self.b[i] = (self.b[i] & 0xF000) | (collision << 10) | mid

    def components(self):
        seen = set()
        n = 0
        for y in range(self.h):
            for x in range(self.w):
                if self.col(x, y) == 0 and (x, y) not in seen:
                    n += 1
                    q = deque([(x, y)])
                    seen.add((x, y))
                    while q:
                        cx, cy = q.popleft()
                        for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
                            if 0 <= nx < self.w and 0 <= ny < self.h and (nx, ny) not in seen and self.col(nx, ny) == 0:
                                seen.add((nx, ny))
                                q.append((nx, ny))
        return n


def event_cells(map_json):
    m = json.load(open(os.path.join(ROOT, map_json)))
    cells = []
    for k in ('object_events', 'warp_events', 'coord_events', 'bg_events'):
        for e in m.get(k) or []:
            cells.append((e['x'], e['y']))
    return cells


def prop_cells(info, x, y):
    cols = info['cols']
    return [(x + i % cols, y + i // cols, mid) for i, mid in enumerate(info['metatiles'])]


def fits(grid, info, ground, x, y):
    for cx, cy, _ in prop_cells(info, x, y):
        if not (0 <= cx < grid.w and 0 <= cy < grid.h):
            return False
        if grid.mid(cx, cy) not in GROUND_IDS[ground] or grid.col(cx, cy):
            return False
    return True


def put(grid, info, x, y):
    for cx, cy, mid in prop_cells(info, x, y):
        grid.set(cx, cy, mid, 1 if info['solid'] else 0)


def main():
    all_ids = json.load(open(os.path.join(ROOT, 'tools/apocalypse/deco_ids.json')))
    grounds = {name: ground for name, _, _, ground, _ in deco.PROPS}
    ok = True
    for layout, cfg in PLACEMENTS.items():
        ids = all_ids[cfg['tileset']]
        rel = layout + '/map.bin'
        raw = subprocess.run(['git', '-C', ROOT, 'show', '%s:%s' % (BASE_REF, rel)], capture_output=True, check=True).stdout
        grid = Grid(list(struct.unpack('<%dH' % (len(raw) // 2), raw)), cfg['width'])
        events = event_cells(cfg['map'])

        for x, y, prop in cfg['props']:
            if fits(grid, ids[prop], grounds[prop], x, y):
                put(grid, ids[prop], x, y)
            else:
                print('SKIP %s %s at (%d,%d)' % (layout, prop, x, y))
                ok = False

        for x, y, name in cfg['swaps']:
            info = ids[name]
            cells = [(x, y)] if x is not None else [(cx, cy) for cy in range(grid.h) for cx in range(grid.w)]
            hit = 0
            for cx, cy in cells:
                if grid.mid(cx, cy) == info['source']:
                    grid.set(cx, cy, info['metatiles'][0], grid.col(cx, cy))
                    hit += 1
            if not hit:
                print('SKIP %s swap %s: source metatile %d not found' % (layout, name, info['source']))

        auto = cfg.get('auto')
        if auto:
            rng = random.Random(auto['seed'])
            base_components = grid.components()

            def near_event(cells):
                return any(abs(cx - ex) + abs(cy - ey) <= EVENT_DISTANCE for cx, cy, _ in cells for ex, ey in events)

            def near_edge(cells):
                return any(cx < EDGE_DISTANCE or cy < EDGE_DISTANCE or cx >= grid.w - EDGE_DISTANCE or cy >= grid.h - EDGE_DISTANCE
                           for cx, cy, _ in cells)

            for names, count, solid in ((auto['solid'], auto['count'], True), (auto['decor'], auto['decor_count'], False)):
                placed, tries = 0, 0
                names_placed = []
                while placed < count and tries < 4000 and names:
                    tries += 1
                    name = rng.choice(names)
                    info = ids[name]
                    x, y = rng.randrange(grid.w), rng.randrange(grid.h)
                    if not fits(grid, info, grounds[name], x, y):
                        continue
                    cells = prop_cells(info, x, y)
                    if near_event(cells) or near_edge(cells):
                        continue
                    if solid:
                        # keep free space around solid props (also diagonally) so they never narrow a passage
                        ring_ok = all(0 <= cx + dx < grid.w and 0 <= cy + dy < grid.h and
                                      (grid.col(cx + dx, cy + dy) == 0 or (cx + dx, cy + dy) in {(a, b) for a, b, _ in cells})
                                      for cx, cy, _ in cells for dx in (-2, -1, 0, 1, 2) for dy in (-1, 0, 1))
                        if not ring_ok:
                            continue
                        saved = [grid.b[cy * grid.w + cx] for cx, cy, _ in cells]
                        put(grid, info, x, y)
                        if grid.components() != base_components:
                            for (cx, cy, _), v in zip(cells, saved):
                                grid.b[cy * grid.w + cx] = v
                            continue
                    else:
                        put(grid, info, x, y)
                    placed += 1
                    names_placed.append(name)
                print('%s: %d/%d auto %s props %s' % (layout, placed, count, 'solid' if solid else 'decor', sorted(names_placed)))

        open(os.path.join(ROOT, rel), 'wb').write(struct.pack('<%dH' % len(grid.b), *grid.b))
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
