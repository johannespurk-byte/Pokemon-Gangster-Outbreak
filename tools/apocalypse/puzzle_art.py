#!/usr/bin/env python3
"""Gangster Outbreak: pixel art for the gym puzzle tiles (16x16 RGBA layers over the gym floor).

tile(ch, tag, barrier, behaviors) -> (layer or None, metatile behaviour or None, blocks the way)
"""
import os
import random
import re

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
INK = (24, 20, 28)
DARK = (72, 68, 80)
LIGHT = (150, 146, 160)
WHITE = (224, 224, 232)


def behaviors():
    out, n = {}, 0
    for line in open(os.path.join(ROOT, 'include/constants/metatile_behaviors.h')):
        m = re.match(r'\s*(MB_\w+)\s*(?:=\s*(\w+))?\s*,', line)
        if m:
            if m.group(2):
                n = int(m.group(2), 0)
            out[m.group(1)] = n
            n += 1
    return out


def blank():
    return [[None] * 16 for _ in range(16)]


def put(layer, x, y, c):
    if 0 <= x < 16 and 0 <= y < 16:
        layer[y][x] = c


def outline(layer, c=INK):
    add = [(x, y) for y in range(16) for x in range(16) if layer[y][x] is None and any(
        0 <= x + dx < 16 and 0 <= y + dy < 16 and layer[y + dy][x + dx] not in (None, c)
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)))]
    for x, y in add:
        layer[y][x] = c


def rotate(layer, d):
    """art is drawn pointing east; turn it to N / S / W"""
    out = blank()
    for y in range(16):
        for x in range(16):
            if d == 'E':
                sx, sy = x, y
            elif d == 'W':
                sx, sy = 15 - x, y
            elif d == 'S':
                sx, sy = y, 15 - x
            else:  # N
                sx, sy = 15 - y, x
            out[y][x] = layer[sy][sx]
    return out


def arrow(tag, d):
    a = blank()
    for x in range(2, 9):                 # shaft
        for y in range(6, 10):
            put(a, x, y, tag)
    for i in range(7):                    # head
        for y in range(8 - i - 1, 8 + i + 1):
            put(a, 14 - i, y, tag)
    for x in range(2, 14):                # highlight
        if a[6][x] is not None:
            put(a, x, 6, WHITE if x > 9 else tag)
    outline(a)
    return rotate(a, d)


def stop(tag):
    a = blank()
    for y in range(16):
        for x in range(16):
            r = ((x - 7.5) ** 2 + (y - 7.5) ** 2) ** 0.5
            if 4.5 <= r <= 6.5:
                put(a, x, y, tag)
            elif r < 4.5:
                put(a, x, y, DARK)
    for i in range(-2, 3):
        put(a, 7 + i, 7 + i, WHITE)
        put(a, 8 - i, 7 + i, WHITE)
    outline(a)
    return a


def ice(_tag):
    a = blank()
    rng = random.Random(4)
    for y in range(16):
        for x in range(16):
            put(a, x, y, (176, 208, 232) if (x + y) % 7 else (208, 232, 248))
    for _ in range(4):
        x, y = rng.randint(1, 12), rng.randint(1, 12)
        for i in range(4):
            put(a, x + i, y + i // 2, WHITE)
    return a


def conveyor(tag, d):
    a = blank()
    for y in range(16):
        for x in range(16):
            put(a, x, y, DARK if 2 <= y <= 13 else INK)
    for y in (2, 13):
        for x in range(16):
            put(a, x, y, LIGHT)
    for cx in (3, 10):                    # chevrons pointing east
        for i in range(4):
            put(a, cx + i, 4 + i, tag)
            put(a, cx + i, 11 - i, tag)
    return rotate(a, d)


def lava(_tag):
    a = blank()
    rng = random.Random(9)
    for y in range(16):
        for x in range(16):
            put(a, x, y, rng.choice([(200, 64, 16), (232, 104, 24), (176, 40, 16)]))
    for _ in range(5):
        x, y = rng.randint(1, 13), rng.randint(1, 13)
        put(a, x, y, (248, 200, 64)); put(a, x + 1, y, (248, 168, 48))
    for _ in range(6):
        x, y = rng.randint(0, 14), rng.randint(0, 14)
        put(a, x, y, (64, 24, 16)); put(a, x + 1, y + 1, (64, 24, 16))
    return a


def ledge(_tag, d):
    """a broken concrete step, drawn for a jump to the east"""
    a = blank()
    for y in range(16):
        for x in range(9, 13):
            put(a, x, y, LIGHT if x == 9 else DARK)
        for x in range(13, 16):
            put(a, x, y, INK)
    for y in range(0, 16, 4):
        put(a, 10, y, INK)
    return rotate(a, d)


def barrier(kind, tag):
    a = blank()
    rng = random.Random(len(kind))
    if kind == 'amp':          # Team Yell: speaker stack
        for y in range(1, 15):
            for x in range(2, 14):
                put(a, x, y, (40, 36, 44))
        for cy in (5, 11):
            for y in range(16):
                for x in range(16):
                    r = ((x - 7.5) ** 2 + (y - cy) ** 2) ** 0.5
                    if r <= 3.2:
                        put(a, x, y, tag if r > 1.6 else DARK)
        for x in range(2, 14):
            put(a, x, 8, DARK)
    elif kind == 'trash':      # Team Skull: trash bags
        for (cx, cy, r) in ((5, 10, 4.5), (11, 10, 4.5), (8, 5, 4.5)):
            for y in range(16):
                for x in range(16):
                    if (x - cx) ** 2 + (y - cy) ** 2 <= r * r:
                        put(a, x, y, DARK if (x + y) % 5 else LIGHT)
        put(a, 8, 1, tag); put(a, 7, 1, tag)
    elif kind == 'rock':       # Aqua & Magma: volcanic boulder
        for y in range(16):
            for x in range(16):
                if (x - 7.5) ** 2 / 49 + (y - 8.5) ** 2 / 42 <= 1:
                    put(a, x, y, DARK if rng.random() < 0.7 else LIGHT)
        for i in range(5):
            put(a, 4 + i, 6 + i // 2, tag)
    elif kind == 'iceblock':   # Plasma: block of ice
        for y in range(1, 15):
            for x in range(1, 15):
                put(a, x, y, (160, 200, 232) if x + y < 20 else (120, 160, 208))
        for i in range(10):
            put(a, 3 + i, 3, WHITE); put(a, 3, 3 + i, WHITE)
    elif kind == 'laser':      # Flare: laser fence post
        for y in range(1, 15):
            put(a, 7, y, DARK); put(a, 8, y, LIGHT)
        for y in (3, 7, 11):
            for x in range(0, 16):
                if x not in (7, 8):
                    put(a, x, y, tag)
    elif kind == 'console':    # Galactic: machine block
        for y in range(2, 15):
            for x in range(1, 15):
                put(a, x, y, LIGHT if y < 5 else DARK)
        for x in range(3, 13, 3):
            put(a, x, 9, tag); put(a, x + 1, 9, tag)
    else:                      # crates (Rocket / Giovanni)
        for y in range(2, 15):
            for x in range(1, 15):
                put(a, x, y, (128, 88, 48))
        for i in range(13):
            put(a, 1 + i, 2 + i, (96, 64, 32)); put(a, 14 - i, 2 + i, (96, 64, 32))
        for y in range(6, 11):
            for x in range(5, 11):
                put(a, x, y, INK)
        put(a, 7, 7, tag); put(a, 8, 7, tag); put(a, 7, 8, tag); put(a, 7, 9, tag); put(a, 8, 9, tag)
    outline(a)
    return a


def tile(ch, tag, barrier_kind, beh):
    spin = {'>': ('E', 'MB_SPIN_RIGHT'), '<': ('W', 'MB_SPIN_LEFT'), '^': ('N', 'MB_SPIN_UP'), 'v': ('S', 'MB_SPIN_DOWN')}
    belts = {'E': 'MB_WALK_EAST', 'W': 'MB_WALK_WEST', 'N': 'MB_WALK_NORTH', 'S': 'MB_WALK_SOUTH'}
    ledges = {')': ('E', 'MB_JUMP_EAST'), '(': ('W', 'MB_JUMP_WEST'), ']': ('S', 'MB_JUMP_SOUTH'), '[': ('N', 'MB_JUMP_NORTH')}
    if ch == '.':
        return None, None, False
    if ch in spin:
        d, b = spin[ch]
        return arrow(tag, d), beh[b], False
    if ch == 'o':
        return stop(tag), beh['MB_STOP_SPINNING'], False
    if ch == '~':
        return ice(tag), beh['MB_ICE'], False
    if ch in belts:
        return conveyor(tag, ch), beh[belts[ch]], False
    if ch in ledges:
        d, b = ledges[ch]
        return ledge(tag, d), beh[b], True
    if ch == 'L':
        return lava(tag), None, True
    if ch == '#':
        return barrier(barrier_kind, tag), None, True
    raise ValueError('unknown puzzle tile %r' % ch)
