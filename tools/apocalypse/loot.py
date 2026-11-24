#!/usr/bin/env python3
"""Gangster Outbreak: extra loot bags (item balls) on the Kanto towns and routes.

Their contents are random per save file (src/gang_items.c, GangItems_RandomizeFoundItem); this script only
places them. Re-runnable: bags it added before (flags FLAG_0x0B0..FLAG_0x0FF) are removed first.
A bag is solid, so it is only placed where all walkable areas of the map stay connected, on plain ground,
away from events, warps and map edges.
"""
import json, os, random, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import place  # noqa: E402

FIRST_FLAG, LAST_FLAG = 0x0B0, 0x0FF
SCRIPT = 'GangLoot_EventScript_Bag'
LAYOUTS = re.compile(r'^LAYOUT_(ROUTE([0-9]+)(_NORTH|_SOUTH)?|PALLET_TOWN|VIRIDIAN_CITY|PEWTER_CITY|CERULEAN_CITY|'
                     r'VERMILION_CITY|LAVENDER_TOWN|CELADON_CITY|FUCHSIA_CITY|SAFFRON_CITY|CINNABAR_ISLAND)$')
CELLS_PER_BAG = 420   # one bag per this many map cells
MAX_PER_MAP = 3
EVENT_DISTANCE = 2
EDGE_DISTANCE = 3


def flag_name(n):
    return 'FLAG_0x%03X' % n


def is_ours(obj):
    m = re.match(r'^FLAG_0x([0-9A-F]{3})$', obj.get('flag', ''))
    return obj.get('script') == SCRIPT or (m and FIRST_FLAG <= int(m.group(1), 16) <= LAST_FLAG)


def behaviors(layout):
    import render_map
    out = {}
    for sym, base in ((layout['primary_tileset'], 0), (layout['secondary_tileset'], 640)):
        d = render_map.tileset_dir(sym)
        raw = open(os.path.join(ROOT, d, 'metatile_attributes.bin'), 'rb').read()
        for i in range(len(raw) // 4):
            out[base + i] = struct.unpack_from('<I', raw, i * 4)[0] & 0x1FF
    return out


def main():
    layouts = json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']
    by_id = {l['id']: l for l in layouts}
    maps = []
    for d in sorted(os.listdir(os.path.join(ROOT, 'data/maps'))):
        mj = os.path.join(ROOT, 'data/maps', d, 'map.json')
        if not os.path.exists(mj):
            continue
        m = json.load(open(mj))
        l = by_id.get(m.get('layout'))
        if not l or l.get('layout_version') != 'frlg' or not LAYOUTS.match(l['id']):
            continue
        g = LAYOUTS.match(l['id']).group(2)
        if g and not 1 <= int(g) <= 25:
            continue
        maps.append((mj, m, l))

    next_flag = FIRST_FLAG
    total = 0
    for mj, m, l in maps:
        objs = [o for o in (m.get('object_events') or []) if not is_ours(o)]
        w = l['width']
        raw = open(os.path.join(ROOT, l['blockdata_filepath']), 'rb').read()
        grid = place.Grid(list(struct.unpack('<%dH' % (len(raw) // 2), raw)), w)
        beh = behaviors(l)
        events = [(e['x'], e['y']) for k in ('object_events', 'warp_events', 'coord_events', 'bg_events')
                  for e in (m.get(k) or []) if k != 'object_events' or not is_ours(e)]
        for e in objs:  # existing objects block their cell
            if 0 <= e['x'] < grid.w and 0 <= e['y'] < grid.h:
                grid.set(e['x'], e['y'], grid.mid(e['x'], e['y']), 1)
        base = grid.components()
        want = min(MAX_PER_MAP, max(1, grid.w * grid.h // CELLS_PER_BAG))
        rng = random.Random(sum(map(ord, l['id'])))
        cells = [(x, y) for y in range(EDGE_DISTANCE, grid.h - EDGE_DISTANCE) for x in range(EDGE_DISTANCE, grid.w - EDGE_DISTANCE)
                 if grid.col(x, y) == 0 and beh.get(grid.mid(x, y), 1) == 0
                 and all(abs(x - ex) + abs(y - ey) > EVENT_DISTANCE for ex, ey in events)]
        rng.shuffle(cells)
        added = []
        for x, y in cells:
            if len(added) >= want or next_flag > LAST_FLAG:
                break
            if any(abs(x - ax) + abs(y - ay) < 8 for ax, ay in added):
                continue  # spread them over the map
            v = grid.b[y * w + x]
            grid.set(x, y, grid.mid(x, y), 1)
            if grid.components() != base:
                grid.b[y * w + x] = v
                continue
            added.append((x, y))
            objs.append({
                'type': 'object', 'graphics_id': 'OBJ_EVENT_GFX_ITEM_BALL', 'x': x, 'y': y,
                'elevation': (v >> 12) & 0xF, 'movement_type': 'MOVEMENT_TYPE_FACE_DOWN',
                'movement_range_x': 1, 'movement_range_y': 1, 'trainer_type': 'TRAINER_TYPE_NONE',
                'trainer_sight_or_berry_tree_id': '0', 'script': SCRIPT, 'flag': flag_name(next_flag),
            })
            next_flag += 1
        assert len(objs) <= 64, mj
        m['object_events'] = objs
        with open(mj, 'w') as f:
            json.dump(m, f, indent=2, ensure_ascii=False)
            f.write('\n')
        total += len(added)
        print('%s: %d bags' % (l['id'], len(added)))
    print('total %d bags, flags 0x%03X..0x%03X' % (total, FIRST_FLAG, next_flag - 1))


if __name__ == '__main__':
    main()
