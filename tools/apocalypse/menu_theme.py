#!/usr/bin/env python3
"""Gangster Outbreak: recolours the menu screens (Pokédex, party menu, summary screen, trainer card) in the
black / gold / blood-red look of the gangs.

Every screen gets an explicit colour map (old RGB -> new RGB) that is applied to its palette files. Colours that
carry meaning (HP bar, types, status, text) are not in the maps and stay as they are. Running the tool again
changes nothing (the old colours are gone).

Usage: python3 tools/apocalypse/menu_theme.py (text boxes: ui_theme.py)
"""
import os

from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

CHAR = [(14, 10, 16), (24, 20, 28), (32, 28, 38), (40, 36, 46), (52, 46, 58), (64, 58, 70), (76, 70, 82),
        (92, 84, 98), (110, 102, 114)]
CREAM = [(244, 236, 220), (236, 226, 206), (226, 214, 190), (208, 194, 168), (180, 160, 120)]
GOLD = [(248, 216, 120), (232, 184, 56), (200, 152, 40), (150, 110, 30), (110, 80, 20)]
RED = [(220, 72, 56), (190, 52, 44), (160, 40, 36), (130, 30, 30), (100, 24, 24), (70, 16, 16)]

POKEDEX = {
    (123, 164, 115): CHAR[5], (74, 115, 65): CHAR[3],         # national mode stripes
    (49, 213, 74): CHAR[5], (24, 131, 32): CHAR[3],           # regional mode stripes
    (238, 246, 57): (236, 204, 104),                          # list panel
    (255, 0, 189): RED[1], (189, 0, 0): RED[3],               # START / SELECT buttons, arrows
    (189, 156, 90): GOLD[2],                                  # scroll bar
    (41, 57, 65): CHAR[1], (41, 57, 106): CHAR[2], (0, 0, 41): CHAR[0],  # dark disc behind the counters
}

PARTY = {
    (139, 156, 41): CHAR[3], (180, 180, 90): CHAR[5], (205, 213, 123): CHAR[6], (230, 238, 139): CHAR[8],
    (98, 90, 0): CHAR[1],                                     # striped background and empty slots
    (131, 197, 222): CHAR[7], (57, 148, 222): CHAR[5], (41, 123, 180): CHAR[3], (0, 98, 148): CHAR[2],
    (65, 205, 255): CHAR[7], (49, 189, 238): CHAR[6], (0, 139, 189): CHAR[3],   # slots
    (172, 238, 255): GOLD[0], (123, 213, 238): GOLD[1], (74, 172, 205): GOLD[3],  # selected slot
    (255, 115, 49): GOLD[1], (197, 57, 0): GOLD[3],           # selection frame
}

SUMMARY = {
    # header bar
    (148, 32, 172): RED[3], (189, 106, 213): RED[1], (230, 189, 255): (236, 212, 140), (213, 148, 246): RED[0],
    (189, 139, 205): RED[2], (255, 230, 255): (248, 232, 190), (230, 180, 213): (220, 190, 130),
    (180, 82, 148): RED[1], (156, 41, 172): RED[2], (230, 156, 255): GOLD[0], (197, 98, 238): RED[0],
    (164, 49, 222): RED[2], (131, 0, 205): RED[4],
    # info page: blue / teal side panel and section bars, light green boxes
    (98, 156, 222): CHAR[6], (74, 148, 189): CHAR[5], (57, 148, 164): CHAR[4], (32, 148, 139): CHAR[3],
    (16, 148, 115): CHAR[3], (0, 106, 98): CHAR[1],
    (156, 222, 164): CREAM[2], (98, 180, 139): CREAM[4], (49, 139, 115): GOLD[3],
    # skills page greens
    (90, 222, 123): CHAR[7], (90, 205, 98): CHAR[6], (90, 189, 74): CHAR[5], (98, 172, 49): CHAR[4],
    (98, 156, 24): CHAR[3], (106, 148, 0): CHAR[2], (90, 106, 0): CHAR[1], (123, 139, 32): GOLD[2],
    # skills page: yellow -> purple ramp (labels, stat area, STATS bar)
    (238, 222, 24): GOLD[0], (213, 189, 57): GOLD[1], (189, 164, 98): GOLD[2], (164, 139, 131): (170, 90, 48),
    (139, 115, 172): RED[2], (115, 90, 213): RED[3], (205, 222, 123): CREAM[2], (172, 172, 148): CREAM[4],
    (139, 131, 180): RED[3],
    # moves page purples
    (222, 24, 238): RED[0], (189, 16, 238): RED[1], (164, 8, 238): RED[2], (131, 8, 246): RED[3],
    (106, 0, 246): RED[4], (82, 0, 255): RED[5], (172, 222, 222): CREAM[2], (139, 148, 230): CREAM[4],
    (106, 74, 238): GOLD[3],
}

CARD = {
    (230, 246, 246): CREAM[0], (222, 238, 255): CREAM[1], (213, 230, 246): CREAM[2], (197, 213, 230): CREAM[3],
    (131, 189, 230): CREAM[3], (106, 164, 222): CHAR[5], (90, 156, 205): CHAR[4], (57, 115, 156): CHAR[2],
    (82, 205, 180): CHAR[4], (49, 156, 148): CHAR[2],
    # female card
    (222, 255, 230): CREAM[0], (205, 246, 222): CREAM[1], (197, 238, 213): CREAM[2], (189, 230, 205): CREAM[3],
    (156, 205, 197): CREAM[3], (131, 180, 172): CHAR[5], (115, 156, 156): CHAR[4], (82, 131, 123): CHAR[2],
    (65, 172, 164): CHAR[2],
}

FILES = {
    'graphics/pokedex/bg_national.pal': POKEDEX,
    'graphics/pokedex/bg_hoenn.pal': POKEDEX,
    'graphics/party_menu/bg.png': PARTY,
    'graphics/summary_screen/tiles.png': SUMMARY,
    'graphics/trainer_card/frlg/tiles.png': CARD,
}
for name in ('blue', 'green', 'bronze', 'silver', 'gold', 'female_bg'):
    FILES['graphics/trainer_card/frlg/%s.pal' % name] = CARD


def recolour_pal(path, cmap):
    raw = open(path, newline='').read()
    nl = '\r\n' if '\r\n' in raw else '\n'
    lines = raw.split(nl)
    n = 0
    for i in range(3, len(lines)):
        if not lines[i].strip():
            continue
        c = tuple(map(int, lines[i].split()))
        if c in cmap:
            lines[i] = '%d %d %d' % cmap[c]
            n += 1
    open(path, 'w', newline='').write(nl.join(lines))
    return n


def recolour_png(path, cmap):
    im = Image.open(path)
    pal = im.getpalette()
    n = 0
    for i in range(1, len(pal) // 3):       # index 0 of every 16 is transparent anyway, but keep slot 0 of the file
        c = tuple(pal[i * 3:i * 3 + 3])
        if c in cmap:
            pal[i * 3:i * 3 + 3] = list(cmap[c])
            n += 1
    if n:
        im.putpalette(pal)
        im.save(path)
    return n


def main():
    for rel, cmap in FILES.items():
        path = os.path.join(ROOT, rel)
        n = (recolour_png if rel.endswith('.png') else recolour_pal)(path, cmap)
        print('%-44s %3d colours' % (rel, n))


if __name__ == '__main__':
    main()
