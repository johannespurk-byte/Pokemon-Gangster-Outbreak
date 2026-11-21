#!/usr/bin/env python3
"""Gangster Outbreak: fill every Kanto route up to MIN_TRAINERS trainers.

For each route with too few trainers this adds trainers on free spots:
- the spot and its surroundings are free ground (no water, ledges or walls), the trainer looks along a free
  line of sight, keeps a distance from events / other trainers / map edges, and the map stays connected
- a trainer definition in src/data/trainers_frlg.party (placeholder species: the randomizer picks the real team,
  levels follow the route's wild Pokémon), a constant in include/constants/opponents_frlg.h
- a script with gang-themed texts in data/maps/<Route>/scripts.inc

Run once; it refuses to run again when the trainers already exist.
"""
import json, os, random, struct, sys
from collections import deque

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MIN_TRAINERS = 5
SIGHT = 3
EVENT_DISTANCE = 3
TRAINER_DISTANCE = 5
EDGE_DISTANCE = 3
WALKABLE_BEHAVIORS = {0, 2}  # MB_NORMAL, MB_TALL_GRASS
MARK = 'GangTrainer'

ROUTES = ['Route%d_Frlg' % n for n in range(1, 26) if n != 21] + ['Route21_North_Frlg', 'Route21_South_Frlg']

# class, pic, overworld graphics, gender, names
TYPES = [
    ('Biker Frlg', 'OBJ_EVENT_GFX_BIKER', 'Male', ['RAZOR', 'KNUCKLES', 'DIESEL', 'BLADE', 'SPIKE', 'RUST', 'TANK', 'VIPER']),
    ('Rocker Frlg', 'OBJ_EVENT_GFX_ROCKER', 'Male', ['SLASH', 'RIOT', 'FUZZ', 'AMP', 'JOLT', 'HEX']),
    ('Black Belt Frlg', 'OBJ_EVENT_GFX_BLACK_BELT_FRLG', 'Male', ['GORO', 'TAKEO', 'RYU', 'KENJI']),
    ('Hiker Frlg', 'OBJ_EVENT_GFX_HIKER_FRLG', 'Male', ['BRICK', 'GRAVEL', 'STONE', 'BOULDER']),
    ('Youngster Frlg', 'OBJ_EVENT_GFX_YOUNGSTER_FRLG', 'Male', ['SKIP', 'TOMMY', 'NICKY', 'JOEY']),
    ('Lass Frlg', 'OBJ_EVENT_GFX_LASS_FRLG', 'Female', ['ROXY', 'DAISY', 'LULU', 'PEARL']),
    ('Picnicker Frlg', 'OBJ_EVENT_GFX_PICNICKER_FRLG', 'Female', ['TRIX', 'SADIE', 'NOVA', 'JADE']),
    ('Camper Frlg', 'OBJ_EVENT_GFX_CAMPER_FRLG', 'Male', ['DUKE', 'CODY', 'BUTCH', 'REX']),
    ('Beauty Frlg', 'OBJ_EVENT_GFX_BEAUTY_FRLG', 'Female', ['VELVET', 'SCARLET', 'IVY', 'ROSE']),
]
TYPE_WEIGHTS = [5, 4, 2, 2, 2, 2, 2, 2, 2]  # mostly bikers and rockers: this is gang territory

INTROS = [
    "This is our turf now.\\nPay the toll or get lost!",
    "Rocket's gone, so the streets\\nbelong to whoever's toughest!",
    "Nobody walks through here\\nwithout a fight!",
    "You look like easy money.\\nHand it over!",
    "Another gang punk? I'll show\\nyou who runs this road!",
    "Heh, fresh meat for my\\nPOKéMON!",
    "The city burned and I got\\nstronger. Wanna see?",
    "Wrong road, kid.\\nThis one's ours.",
]
DEFEATS = [
    "Tch… Fine, you win.",
    "Ugh! Who taught you to\\nfight like that?",
    "No way… I got wrecked!",
    "All right, all right!\\nYou made your point!",
    "My gang's gonna hear\\nabout this…",
    "Heh… Not bad, punk.",
]
POSTS = [
    "Take what you want and\\nget out of my sight.",
    "GIOVANNI's crew, huh?\\nI should've known.",
    "Ever since the outbreak,\\nonly the strong survive.",
    "Keep walking. This road's\\nyours… for now.",
    "Word travels fast. Everyone\\nwill know your name soon.",
    "Next time I'll bring the\\nwhole gang!",
]


def load(path):
    return open(os.path.join(ROOT, path)).read()


def layout_info(layout_id):
    for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts']:
        if l.get('id') == layout_id:
            return l
    raise KeyError(layout_id)


def tileset_dir(symbol):
    import re
    headers = load('src/data/tilesets/headers.h')
    gfx = load('src/data/tilesets/graphics.h')
    m = re.search(r'const struct Tileset %s =\s*\{(.*?)\};' % re.escape(symbol), headers, re.S)
    tiles = re.search(r'\.tiles = (\w+)', m.group(1)).group(1)
    return re.search(r'%s\[\] = INCGFX_U32\("([^"]+)/tiles\.png"' % re.escape(tiles), gfx).group(1)


def behaviors(layout):
    prim = open(os.path.join(ROOT, tileset_dir(layout['primary_tileset']), 'metatile_attributes.bin'), 'rb').read()
    sec = open(os.path.join(ROOT, tileset_dir(layout['secondary_tileset']), 'metatile_attributes.bin'), 'rb').read()

    def beh(mid):
        data, i = (prim, mid) if mid < 640 else (sec, mid - 640)
        if (i + 1) * 4 > len(data):
            return 0xFFFF
        return struct.unpack_from('<I', data, i * 4)[0] & 0x1FF
    return beh


def wild_level(map_const):
    """Highest level of the route's land Pokémon (water only if the route has no land encounters)."""
    d = json.load(open(os.path.join(ROOT, 'src/data/wild_encounters.json')))
    levels = {'land_mons': 0, 'water_mons': 0}
    for g in d['wild_encounter_groups']:
        for e in g.get('encounters', []):
            if e.get('map') != map_const:
                continue
            for key in levels:
                for m in (e.get(key) or {}).get('mons', []):
                    levels[key] = max(levels[key], m['max_level'])
    return levels['land_mons'] or levels['water_mons']


DIRS = {'UP': (0, -1), 'DOWN': (0, 1), 'LEFT': (-1, 0), 'RIGHT': (1, 0)}


def place(route, needed, rng):
    m = json.load(open(os.path.join(ROOT, 'data/maps', route, 'map.json')))
    layout = layout_info(m['layout'])
    w, h = layout['width'], layout['height']
    raw = open(os.path.join(ROOT, layout['blockdata_filepath']), 'rb').read()
    blocks = list(struct.unpack('<%dH' % (len(raw) // 2), raw))
    beh = behaviors(layout)

    def walk(x, y):
        if not (0 <= x < w and 0 <= y < h):
            return False
        v = blocks[y * w + x]
        return (v >> 10) & 3 == 0 and beh(v & 0x3FF) in WALKABLE_BEHAVIORS

    def elev(x, y):
        return blocks[y * w + x] >> 12

    events = []
    for k in ('object_events', 'warp_events', 'coord_events', 'bg_events'):
        for e in m.get(k) or []:
            events.append((e['x'], e['y']))
    blocked = set()

    def components():
        seen, n = set(), 0
        for y in range(h):
            for x in range(w):
                if (blocks[y * w + x] >> 10) & 3 == 0 and (x, y) not in seen and (x, y) not in blocked:
                    n += 1
                    q = deque([(x, y)])
                    seen.add((x, y))
                    while q:
                        cx, cy = q.popleft()
                        for nx, ny in ((cx + 1, cy), (cx - 1, cy), (cx, cy + 1), (cx, cy - 1)):
                            if 0 <= nx < w and 0 <= ny < h and (nx, ny) not in seen and (nx, ny) not in blocked \
                               and (blocks[ny * w + nx] >> 10) & 3 == 0:
                                seen.add((nx, ny))
                                q.append((nx, ny))
        return n

    base = components()
    cells = [(x, y) for y in range(EDGE_DISTANCE, h - EDGE_DISTANCE) for x in range(EDGE_DISTANCE, w - EDGE_DISTANCE)]
    rng.shuffle(cells)
    placed = []
    for x, y in cells:
        if len(placed) >= needed:
            break
        if not walk(x, y) or elev(x, y) in (0, 15):
            continue
        if any(abs(x - ex) + abs(y - ey) < EVENT_DISTANCE for ex, ey in events):
            continue
        if any(abs(x - px) + abs(y - py) < TRAINER_DISTANCE for px, py, _, _ in placed):
            continue
        if not all(walk(x + dx, y + dy) and elev(x + dx, y + dy) == elev(x, y)
                   for dx in (-1, 0, 1) for dy in (-1, 0, 1)):
            continue
        dirs = [d for d, (dx, dy) in DIRS.items()
                if all(walk(x + dx * i, y + dy * i) and elev(x + dx * i, y + dy * i) == elev(x, y) for i in range(1, SIGHT + 1))]
        if not dirs:
            continue
        blocked.add((x, y))
        if components() != base:
            blocked.discard((x, y))
            continue
        placed.append((x, y, rng.choice(dirs), elev(x, y)))
    return m, placed


def main():
    party_path = 'src/data/trainers_frlg.party'
    opp_path = 'include/constants/opponents_frlg.h'
    party = load(party_path)
    if MARK.upper() in party:
        sys.exit('route trainers already generated')
    opp = load(opp_path)
    import re
    count = int(re.search(r'#define TRAINERS_COUNT_FRLG\s+(\d+)', opp).group(1))
    next_id = count
    new_consts, new_party = [], []
    used_names = set(re.findall(r'^Name: (\w+)', party, re.M))
    for route in ROUTES:
        path = os.path.join(ROOT, 'data/maps', route, 'map.json')
        if not os.path.exists(path):
            continue
        m = json.load(open(path))
        have = sum(1 for o in m.get('object_events') or [] if o.get('trainer_type', 'TRAINER_TYPE_NONE') != 'TRAINER_TYPE_NONE')
        needed = MIN_TRAINERS - have
        if needed <= 0:
            continue
        rng = random.Random(route)
        m, placed = place(route, needed, rng)
        map_const = m['id']
        level = max(4, wild_level(map_const) + 2)
        short = route.replace('_Frlg', '')
        scripts = ['', '@ Gangster Outbreak: gang trainers (tools/route_trainers.py)']
        for n, (x, y, d, el) in enumerate(placed, 1):
            cls, gfx, gender, names = rng.choices(TYPES, weights=TYPE_WEIGHTS)[0]
            name = next((nm for nm in rng.sample(names, len(names)) if nm not in used_names), rng.choice(names))
            used_names.add(name)
            const = 'TRAINER_%s_%s_%d' % (MARK.upper(), short.upper(), n)
            new_consts.append('#define %-42s %d' % (const, next_id))
            next_id += 1
            size = 2 if level < 20 else 3
            mons = ''.join('\nRattata\nLevel: %d\n' % max(2, level - (size - 1 - i)) for i in range(size))
            new_party.append('=== %s ===\nName: %s\nClass: %s\nPic: %s\nGender: %s\nMusic: Serious\nDouble Battle: No\nAI: Check Bad Move\n%s'
                             % (const, name, cls, cls, gender, mons))
            label = '%s_EventScript_%s%d' % (short, MARK, n)
            m.setdefault('object_events', []).append({
                'type': 'object', 'graphics_id': gfx, 'x': x, 'y': y, 'elevation': el,
                'movement_type': 'MOVEMENT_TYPE_FACE_%s' % d, 'movement_range_x': 1, 'movement_range_y': 1,
                'trainer_type': 'TRAINER_TYPE_NORMAL', 'trainer_sight_or_berry_tree_id': str(SIGHT),
                'script': label, 'flag': '0'})
            tx = '%s_Text_%s%d' % (short, MARK, n)
            scripts += ['%s::' % label,
                        '\ttrainerbattle_single %s, %sIntro, %sDefeat' % (const, tx, tx),
                        '\tmsgbox %sPost, MSGBOX_AUTOCLOSE' % tx, '\tend', '']
            for suffix, pool in (('Intro', INTROS), ('Defeat', DEFEATS), ('Post', POSTS)):
                scripts += ['%s%s:' % (tx, suffix), '\t.string "%s$"' % rng.choice(pool), '']
        text = json.dumps(m, indent=2, ensure_ascii=False)
        text = text.replace(': []', ': [\n\n  ]')  # keep the editor's format for empty lists
        with open(path, 'w') as f:
            f.write(text + '\n')
        with open(os.path.join(ROOT, 'data/maps', route, 'scripts.inc'), 'a') as f:
            f.write('\n'.join(scripts) + '\n')
        print('%-22s had %d, added %d (level %d)' % (route, have, len(placed), level))
    opp = opp.replace('#define TRAINERS_COUNT_FRLG                      %d' % count,
                      '#define TRAINERS_COUNT_FRLG                      %d' % next_id)
    opp = opp.replace('\n// NOTE: Because each Trainer uses a flag',
                      '\n// Gangster Outbreak: gang trainers on the routes (tools/route_trainers.py)\n'
                      + '\n'.join(new_consts) + '\n\n// NOTE: Because each Trainer uses a flag', 1)
    open(os.path.join(ROOT, opp_path), 'w').write(opp)
    open(os.path.join(ROOT, party_path), 'a').write('\n' + '\n'.join(new_party))
    print('trainers', count, '->', next_id)


if __name__ == '__main__':
    main()
