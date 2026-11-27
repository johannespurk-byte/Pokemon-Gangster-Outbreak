#!/usr/bin/env python3
"""Gangster Outbreak: a HEAL-BOT and a SHOP-BOT on every route, in Viridian Forest and on the Sevii Islands.

The HEAL-BOT (Pokémon Center nurse sprite) heals the party and recharges the POKé VIAL / AUTO-REPEL.
The SHOP-BOT (vending machine sprite) sells a special offer of four items that fits the area and the point in
the game: one kind of ball, one healing item, one evolution item and one battle item (see OFFERS).

Writes data/scripts/gang_route_bots.inc (scripts + item lists) and adds the two bots to each map.json.
They stand side by side, facing down, near where the player enters the map, on plain ground; the spot is only
used if all walkable areas of the map stay connected and the cells in front of both bots are free.
Re-runnable: bots it added before (scripts GangRouteBot_*) are removed first.

Usage: python3 tools/apocalypse/route_bots.py
"""
import json, os, struct, sys
from collections import deque

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import place  # noqa: E402
from loot import behaviors  # noqa: E402

OUT = os.path.join(ROOT, 'data/scripts/gang_route_bots.inc')
HEAL_SCRIPT = 'GangRouteBot_EventScript_Heal'
EVENT_DISTANCE = 2
EDGE_DISTANCE = 2
GROUND = ('MB_NORMAL', 'MB_SAND', 'MB_MOUNTAIN_TOP')   # behaviors the bots and the player in front of them may stand on

# map folder -> (ball, healing item, evolution item, battle item); roughly in story order
OFFERS = {
    'Route1_Frlg':          ('POKE_BALL',   'POTION',        'MOON_STONE',     'X_ATTACK'),
    'Route22_Frlg':         ('POKE_BALL',   'POTION',        'LEAF_STONE',     'X_DEFENSE'),
    'Route2_Frlg':          ('GREAT_BALL',  'POTION',        'FIRE_STONE',     'X_SPEED'),
    'ViridianForest_Frlg':  ('NEST_BALL',   'ENERGY_POWDER', 'LEAF_STONE',     'X_SP_ATK'),
    'Route3_Frlg':          ('GREAT_BALL',  'SUPER_POTION',  'MOON_STONE',     'DIRE_HIT'),
    'Route4_Frlg':          ('GREAT_BALL',  'FRESH_WATER',   'THUNDER_STONE',  'X_ACCURACY'),
    'Route24_Frlg':         ('NET_BALL',    'SUPER_POTION',  'WATER_STONE',    'X_SP_DEF'),
    'Route25_Frlg':         ('NET_BALL',    'SODA_POP',      'OVAL_STONE',     'GUARD_SPEC'),
    'Route5_Frlg':          ('GREAT_BALL',  'SUPER_POTION',  'LINKING_CORD',   'MUSCLE_BAND'),
    'Route6_Frlg':          ('QUICK_BALL',  'SUPER_POTION',  'METAL_COAT',     'WISE_GLASSES'),
    'Route11_Frlg':         ('REPEAT_BALL', 'LEMONADE',      'THUNDER_STONE',  'QUICK_CLAW'),
    'Route9_Frlg':          ('ULTRA_BALL',  'HYPER_POTION',  'FIRE_STONE',     'SCOPE_LENS'),
    'Route10_Frlg':         ('DUSK_BALL',   'FULL_HEAL',     'ELECTIRIZER',    'FOCUS_BAND'),
    'Route8_Frlg':          ('QUICK_BALL',  'HYPER_POTION',  'REAPER_CLOTH',   'BLACK_SLUDGE'),
    'Route7_Frlg':          ('TIMER_BALL',  'MOOMOO_MILK',   'SUN_STONE',      'MENTAL_HERB'),
    'Route12_Frlg':         ('DIVE_BALL',   'HYPER_POTION',  'KINGS_ROCK',     'SHELL_BELL'),
    'Route13_Frlg':         ('NET_BALL',    'REVIVE',        'DRAGON_SCALE',   'MUSCLE_BAND'),
    'Route14_Frlg':         ('ULTRA_BALL',  'ENERGY_ROOT',   'RAZOR_CLAW',     'WHITE_HERB'),
    'Route15_Frlg':         ('TIMER_BALL',  'HYPER_POTION',  'SHINY_STONE',    'AIR_BALLOON'),
    'Route16_Frlg':         ('HEAL_BALL',   'FULL_HEAL',     'PROTECTOR',      'FOCUS_BAND'),
    # Route 17 is the Cycling Road: the player can't stop there, its gates on Routes 16 and 18 have bots
    'Route18_Frlg':         ('ULTRA_BALL',  'ETHER',         'RAZOR_FANG',     'EXPERT_BELT'),
    'Route19_Frlg':         ('DIVE_BALL',   'MAX_POTION',    'DEEP_SEA_SCALE', 'SHELL_BELL'),
    'Route20_Frlg':         ('DIVE_BALL',   'MAX_POTION',    'PRISM_SCALE',    'WHITE_HERB'),
    'Route21_North_Frlg':   ('NET_BALL',    'MAX_POTION',    'DEEP_SEA_TOOTH', 'SCOPE_LENS'),
    'Route21_South_Frlg':   ('LUXURY_BALL', 'ELIXIR',        'WATER_STONE',    'LEFTOVERS'),
    'Route23_Frlg':         ('ULTRA_BALL',  'FULL_RESTORE',  'DUSK_STONE',     'LIFE_ORB'),
    'OneIsland_KindleRoad_Frlg':   ('ULTRA_BALL',  'MAX_POTION',  'MAGMARIZER',  'CHARCOAL'),
    'OneIsland_TreasureBeach_Frlg': ('DIVE_BALL',  'ELIXIR',      'ICE_STONE',   'SHELL_BELL'),
    'MtEmber_Exterior_Frlg':        ('DUSK_BALL',  'FULL_RESTORE', 'FIRE_STONE', 'POWER_HERB'),
    'TwoIsland_CapeBrink_Frlg':     ('LUXURY_BALL', 'MAX_POTION', 'DAWN_STONE',  'ROCKY_HELMET'),
    'ThreeIsland_Port_Frlg':        ('ULTRA_BALL', 'FULL_RESTORE', 'SACHET',     'EXPERT_BELT'),
    'ThreeIsland_BondBridge_Frlg':  ('NET_BALL',   'MAX_ETHER',    'WHIPPED_DREAM', 'FOCUS_SASH'),
    'ThreeIsland_BerryForest_Frlg': ('NEST_BALL',  'REVIVAL_HERB', 'SWEET_APPLE', 'SITRUS_BERRY'),
    'FiveIsland_Meadow_Frlg':       ('REPEAT_BALL', 'MAX_REVIVE',  'TART_APPLE', 'ASSAULT_VEST'),
    'FiveIsland_MemorialPillar_Frlg': ('TIMER_BALL', 'FULL_RESTORE', 'CRACKED_POT', 'WEAKNESS_POLICY'),
    'FiveIsland_ResortGorgeous_Frlg': ('LUXURY_BALL', 'MAX_ELIXIR', 'SHINY_STONE', 'CHOICE_SCARF'),
    'FiveIsland_WaterLabyrinth_Frlg': ('DIVE_BALL', 'MAX_REVIVE',   'PRISM_SCALE', 'LIFE_ORB'),
    'SixIsland_GreenPath_Frlg':     ('NEST_BALL',  'FULL_RESTORE', 'GALARICA_CUFF', 'EVIOLITE'),
    'SixIsland_OutcastIsland_Frlg': ('DUSK_BALL',  'MAX_REVIVE',   'BLACK_AUGURITE', 'CHOICE_BAND'),
    'SixIsland_WaterPath_Frlg':     ('DIVE_BALL',  'MAX_ELIXIR',   'GALARICA_WREATH', 'CHOICE_SPECS'),
    'SixIsland_RuinValley_Frlg':    ('ULTRA_BALL', 'SACRED_ASH',   'PEAT_BLOCK',  'LOADED_DICE'),
    'SixIsland_PatternBush_Frlg':   ('NEST_BALL',  'FULL_RESTORE', 'AUSPICIOUS_ARMOR', 'COVERT_CLOAK'),
    'SevenIsland_TrainerTower_Frlg': ('QUICK_BALL', 'MAX_REVIVE',  'MALICIOUS_ARMOR', 'CLEAR_AMULET'),
    'SevenIsland_SevaultCanyon_Entrance_Frlg': ('ULTRA_BALL', 'FULL_RESTORE', 'DUBIOUS_DISC', 'HEAVY_DUTY_BOOTS'),
    'SevenIsland_SevaultCanyon_Frlg': ('DUSK_BALL', 'MAX_REVIVE',  'RAZOR_CLAW',  'FOCUS_SASH'),
    'SevenIsland_TanobyRuins_Frlg':  ('TIMER_BALL', 'MAX_ELIXIR',  'OVAL_STONE',  'ROCKY_HELMET'),
}


def label(map_dir):
    return 'GangRouteBot_EventScript_Shop_' + map_dir.replace('_Frlg', '')


def write_scripts(placed):
    out = ['@ Gangster Outbreak: HEAL-BOTs and SHOP-BOTs on the routes, in Viridian Forest and on the Sevii Islands.',
           '@ Generated by tools/apocalypse/route_bots.py - edit the offers there.', '',
           HEAL_SCRIPT + '::',
           '\tlock', '\tfaceplayer',
           '\tmsgbox GangRouteBot_Text_HealOffer, MSGBOX_YESNO',
           '\tgoto_if_eq VAR_RESULT, NO, GangRouteBot_EventScript_HealBye',
           '\tmessage GangRouteBot_Text_Healing', '\twaitmessage',
           '\tplayfanfare MUS_HEAL', '\twaitfanfare',
           '\tspecial HealPlayerParty', '\tspecial GangItems_Recharge',
           '\tmsgbox GangRouteBot_Text_HealDone', '\trelease', '\tend', '',
           'GangRouteBot_EventScript_HealBye::',
           '\tmsgbox GangRouteBot_Text_HealBye', '\trelease', '\tend', '',
           'GangRouteBot_Text_HealOffer:',
           '\t.string "BZZT… FIELD HEAL-BOT.\\p"',
           '\t.string "Restore POKéMON and recharge\\n"',
           '\t.string "your gadgets?$"', '',
           'GangRouteBot_Text_Healing:',
           '\t.string "Scanning POKéMON…$"', '',
           'GangRouteBot_Text_HealDone:',
           '\t.string "Treatment complete.\\n"',
           '\t.string "Stay dangerous out there.$"', '',
           'GangRouteBot_Text_HealBye:',
           '\t.string "HEAL-BOT returning to standby.$"', '',
           'GangRouteBot_Text_ShopHello:',
           '\t.string "KLONK! FIELD SHOP-BOT.\\p"',
           '\t.string "Special offer for this area.\\n"',
           '\t.string "Insert money. Choose product.$"', '']
    for map_dir in placed:
        items = OFFERS[map_dir]
        lab = label(map_dir)
        out += [lab + '::', '\tlock', '\tfaceplayer',
                '\tmessage GangRouteBot_Text_ShopHello', '\twaitmessage',
                '\tpokemart %s_Items' % lab, '\tmsgbox gText_PleaseComeAgain', '\trelease', '\tend', '',
                '\t.align 2', '%s_Items:' % lab] + ['\t.2byte ITEM_' + i for i in items] + ['\tpokemartlistend', '']
    with open(OUT, 'w') as f:
        f.write('\n'.join(out))


def is_ours(obj):
    return (obj.get('script') or '').startswith('GangRouteBot_')


def behavior_ids(names):
    import re
    order = [re.match(r'\s*(MB_\w+)', line).group(1) for line in open(os.path.join(ROOT, 'include/constants/metatile_behaviors.h'))
             if re.match(r'\s*MB_\w+,', line)]
    return {order.index(n) for n in names}


def find_spot(m, l, min_dist=4, edge=EDGE_DISTANCE, event_dist=EVENT_DISTANCE):
    ground = behavior_ids(GROUND)
    w = l['width']
    raw = open(os.path.join(ROOT, l['blockdata_filepath']), 'rb').read()
    grid = place.Grid(list(struct.unpack('<%dH' % (len(raw) // 2), raw)), w)
    beh = behaviors(l)
    objs = [o for o in (m.get('object_events') or []) if not is_ours(o)]
    events = [(e['x'], e['y']) for k in ('object_events', 'warp_events', 'coord_events', 'bg_events')
              for e in (m.get(k) or []) if k != 'object_events' or not is_ours(e)]
    for e in objs:
        if 0 <= e['x'] < grid.w and 0 <= e['y'] < grid.h:
            grid.set(e['x'], e['y'], grid.mid(e['x'], e['y']), 1)
    base = grid.components()

    def plain(x, y):
        return 0 <= x < grid.w and 0 <= y < grid.h and grid.col(x, y) == 0 and beh.get(grid.mid(x, y), -1) in ground

    # entrances: walkable cells on the map border and warps; the bots go near the closest one
    entries = [(x, y) for y in range(grid.h) for x in range(grid.w)
               if (x in (0, grid.w - 1) or y in (0, grid.h - 1)) and grid.col(x, y) == 0]
    entries += [(e['x'], e['y']) for e in (m.get('warp_events') or [])]
    if not entries:
        return None
    dist = {}
    q = deque()
    for c in entries:
        if 0 <= c[0] < grid.w and 0 <= c[1] < grid.h:
            dist[c] = 0
            q.append(c)
    while q:   # walking distance from the nearest entrance (ignores elevation, good enough for picking a spot)
        x, y = q.popleft()
        for nx, ny in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
            if 0 <= nx < grid.w and 0 <= ny < grid.h and (nx, ny) not in dist and grid.col(nx, ny) == 0:
                dist[(nx, ny)] = dist[(x, y)] + 1
                q.append((nx, ny))

    cands = []
    for y in range(edge, grid.h - edge - 1):
        for x in range(edge, grid.w - edge - 1):
            cells = [(x, y), (x + 1, y)]
            fronts = [(x, y + 1), (x + 1, y + 1)]
            if not all(plain(*c) for c in cells + fronts):
                continue
            if any(abs(cx - ex) + abs(cy - ey) <= event_dist for cx, cy in cells + fronts for ex, ey in events):
                continue
            d = min(dist.get(c, 9999) for c in fronts)
            if d < min_dist or d >= 9999:
                continue
            cands.append((d, y, x))
    for d, y, x in sorted(cands):
        saved = [grid.b[y * w + x], grid.b[y * w + x + 1]]
        grid.set(x, y, grid.mid(x, y), 1)
        grid.set(x + 1, y, grid.mid(x + 1, y), 1)
        ok = grid.components() == base
        grid.b[y * w + x], grid.b[y * w + x + 1] = saved
        if ok:
            return x, y, (saved[0] >> 12) & 0xF
    return None


def main():
    layouts = {l['id']: l for l in json.load(open(os.path.join(ROOT, 'data/layouts/layouts.json')))['layouts'] if 'id' in l}
    placed = []
    for map_dir in OFFERS:
        mj = os.path.join(ROOT, 'data/maps', map_dir, 'map.json')
        m = json.load(open(mj))
        l = layouts[m['layout']]
        m['object_events'] = [o for o in (m.get('object_events') or []) if not is_ours(o)]
        spot = find_spot(m, l) or find_spot(m, l, min_dist=1) or find_spot(m, l, min_dist=1, edge=1, event_dist=1)
        if spot is None:
            print('%s: no free spot' % map_dir)
        else:
            x, y, elev = spot
            base = {'type': 'object', 'y': y, 'elevation': elev, 'movement_type': 'MOVEMENT_TYPE_FACE_DOWN',
                    'movement_range_x': 0, 'movement_range_y': 0, 'trainer_type': 'TRAINER_TYPE_NONE',
                    'trainer_sight_or_berry_tree_id': '0', 'flag': '0'}
            m['object_events'].append(dict(base, graphics_id='OBJ_EVENT_GFX_NURSE_FRLG', x=x, script=HEAL_SCRIPT))
            m['object_events'].append(dict(base, graphics_id='OBJ_EVENT_GFX_CLERK', x=x + 1, script=label(map_dir)))
            assert len(m['object_events']) <= 64, map_dir
            placed.append(map_dir)
            print('%s: bots at %d,%d' % (map_dir, x, y))
        with open(mj, 'w') as f:
            json.dump(m, f, indent=2, ensure_ascii=False)
            f.write('\n')
    write_scripts(placed)
    print('%d of %d areas have bots' % (len(placed), len(OFFERS)))


if __name__ == '__main__':
    main()
