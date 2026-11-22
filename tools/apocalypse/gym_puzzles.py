#!/usr/bin/env python3
"""Gangster Outbreak: a new puzzle for every gym, used by gyms.py.

A gym puzzle is drawn as text, one character per map cell (the full layout, same size as the gym):
  K  keep the original cell (outer walls, leader platform, statues, door ...)
  .  plain floor                    #  barrier (team prop, blocks)
  > < ^ v  spin arrows (the player keeps moving in the arrow's direction, like the Rocket Hideout)
  o  stop tile (ends spinning)      ~  ice (slide until the ice ends or something blocks)
  E W N S  conveyor belts (one forced step per tile)
  L  lava / acid (blocks)           ) ( ] [  ledges: jump east / west / south / north
  0-9  teleport pads, pads with the same digit are linked both ways
  x  hidden switch spot (Vermilion: crates with switches) handled by the gym's script
The solver below moves a player through the puzzle exactly like src/field_player_avatar.c does
(spinning, slipping, conveyors, ledges, teleports) and checks that the leader can be reached from the door
and the door from the leader.
"""
from collections import deque

DIRS = {'N': (0, -1), 'S': (0, 1), 'W': (-1, 0), 'E': (1, 0)}
SPIN = {'>': 'E', '<': 'W', '^': 'N', 'v': 'S'}
CONVEYOR = {'E': 'E', 'W': 'W', 'N': 'N', 'S': 'S'}
LEDGE = {')': 'E', '(': 'W', ']': 'S', '[': 'N'}
BLOCKING = set('#L') | set(LEDGE)


class Puzzle:
    def __init__(self, rows, blocked_cells, objects):
        """rows: list of strings; blocked_cells: cells blocked in the final layout (kept walls etc.);
        objects: cells taken by people (they block too)"""
        self.rows = rows
        self.h, self.w = len(rows), len(rows[0])
        self.blocked = set(blocked_cells) | set(objects)
        pads = {}
        for y, row in enumerate(rows):
            for x, ch in enumerate(row):
                if ch.isdigit():
                    pads.setdefault(ch, []).append((x, y))
        self.links = {}
        for ch, cells in pads.items():
            assert len(cells) == 2, 'teleport %s needs exactly two pads: %s' % (ch, cells)
            a, b = cells
            self.links[a], self.links[b] = b, a

    def ch(self, x, y):
        return self.rows[y][x]

    def free(self, x, y):
        return 0 <= x < self.w and 0 <= y < self.h and (x, y) not in self.blocked and self.ch(x, y) not in BLOCKING

    def step(self, pos, d):
        """player at rest on pos walks in direction d -> resting position (or None if nothing happens)"""
        x, y = pos
        dx, dy = DIRS[d]
        t = (x + dx, y + dy)
        if not (0 <= t[0] < self.w and 0 <= t[1] < self.h):
            return None
        c = self.ch(*t)
        if c in LEDGE and t not in self.blocked:
            if LEDGE[c] != d:
                return None
            land = (t[0] + dx, t[1] + dy)
            return self.settle(land, d) if self.free(*land) else None
        if not self.free(*t):
            return None
        return self.settle(t, d)

    def settle(self, pos, d):
        """apply the tile effects after arriving on pos while moving in direction d"""
        seen = set()
        cur, moving = pos, d
        spin = None
        while True:
            key = (cur, moving, spin)
            if key in seen:
                return None  # endless loop: the player would be stuck spinning
            seen.add(key)
            c = self.ch(*cur)
            if cur in self.links and spin is None:
                return self.links[cur]  # arrive on the linked pad (it does not fire again)
            if c in SPIN:
                spin = SPIN[c]
            if spin is not None:
                if c == 'o':
                    return cur
                nxt = (cur[0] + DIRS[spin][0], cur[1] + DIRS[spin][1])
                if not self.free(*nxt):
                    return cur
                cur = nxt
                continue
            if c == '~':
                nxt = (cur[0] + DIRS[moving][0], cur[1] + DIRS[moving][1])
                if not self.free(*nxt):
                    return cur
                cur = nxt
                continue
            if c in CONVEYOR and c not in '.':
                moving = CONVEYOR[c]
                nxt = (cur[0] + DIRS[moving][0], cur[1] + DIRS[moving][1])
                if not self.free(*nxt):
                    return cur
                cur = nxt
                continue
            return cur

    def reachable(self, start):
        seen, q = {start: None}, deque([start])
        while q:
            p = q.popleft()
            for d in 'NSWE':
                n = self.step(p, d)
                if n is not None and n not in seen:
                    seen[n] = p
                    q.append(n)
        return seen

    def path(self, start, goal):
        seen = self.reachable(start)
        if goal not in seen:
            return None
        out = [goal]
        while seen[out[-1]] is not None:
            out.append(seen[out[-1]])
        return out[::-1]

    def check(self, door, leader_front, name):
        there = self.path(door, leader_front)
        assert there, '%s: the leader cannot be reached from the door' % name
        back = self.path(leader_front, door)
        assert back, '%s: no way back from the leader to the door' % name
        return len(there) - 1, len(back) - 1


# ------------------------------------------------------------------ the gyms
# objects: script name suffix -> (x, y, movement type, trainer sight); people not listed keep their place.
GYM_PUZZLES = {
    # Team Yell's mosh pit: spin arrows fling the player around; only the arrows next to the stage stop in
    # front of Piers. A one-way chute in the middle leads back to the door.
    'PEWTER_CITY': dict(
        rows=[
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "K...........K",
            "K###########K",
            "K#####K#####K",
            "K###>.o.<###K",
            "K#..^#v#^..#K",
            "K#.v#.v.#v.#K",
            "K<..v...v..>K",
            "K.#..#v#..#.K",
            "K..<.....>..K",
            "KKKKKKoKKKKKK",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
        ],
        door=(6, 13), leader_front=(6, 6),
        objects={'Liam': (5, 8, 'MOVEMENT_TYPE_FACE_DOWN', 3), 'Marla': (10, 8, 'MOVEMENT_TYPE_FACE_LEFT', 3)},
        barrier='amp'),
    # Team Skull's skate park: rows of ramps you can only jump down; one ramp per row leads up, the others
    # end in pockets. Going back down is easy (every ramp can be jumped down).
    'CERULEAN_CITY': dict(
        rows=[
            "KKKKKKKKKKKKKKKKK",
            "KKKKKKKKKKKKKKKKK",
            "KKKKKKKKKKKKKKKKK",
            "K###############K",
            "K###############K",
            "K###############K",
            "K#######K#######K",
            "K######...######K",
            "K]]]]]]][]]]]]]]K",
            "K.#...........#.K",
            "K.#.###.#.###.#.K",
            "K[][]]]]]]]]]]][K",
            "K......#.#......K",
            "K.####.#.#.####.K",
            "K]]]]][]]]]]]]][K",
            "K...............K",
            "K...............K",
            "K.....K...K.....K",
            "KKKKKKKKKKKKKKKKK",
            "KKKKKKKKKKKKKKKKK",
        ],
        door=(8, 18), leader_front=(8, 7),
        objects={'Diana': (9, 10, 'MOVEMENT_TYPE_FACE_UP', 2), 'Luis': (15, 12, 'MOVEMENT_TYPE_FACE_LEFT', 3)},
        barrier='trash'),
    # Neo Rocket's warehouse: the switches for the laser gate are hidden in two neighbouring Rocket crates
    # (same logic as the old trash cans, see SetVermilionTrashCans); barricades make a zigzag through the
    # crates. Behind the gate a small spinner room: only the left arrow brings you in front of Archer, the
    # middle one throws you back through the gate.
    'VERMILION_CITY': dict(
        rows=[
            "KKKKKKKKKKK",
            "KKKKKKKKKKK",
            "KKKKKKKKKKK",
            "K.o>.o.>o.K",
            "K#^##v##^#K",
            "K.........K",
            "KKKKKKKKKKK",
            "KKKKKKKKKKK",
            "...........",
            "####.######",
            ".#.#.#.#.#.",
            "...........",
            ".#.#.#.#.#.",
            "...........",
            ".#.#.#.#.#.",
            "##.#####.##",
            "...........",
            "KKKKKKKKKKK",
            "KKKKKKKKKKK",
            "KKKKKKKKKKK",
            "KKKKKKKKKKK",
        ],
        door=(5, 18), leader_front=(5, 3), open_cells=[(4, 6), (5, 6), (6, 6), (4, 7), (5, 7), (6, 7)],
        objects={'Baily': (10, 11, 'MOVEMENT_TYPE_FACE_LEFT', 4), 'Tucker': (7, 8, 'MOVEMENT_TYPE_FACE_LEFT', 3)},
        barrier='crates'),
    # Aqua & Magma: the stage is ringed by lava; only the two currents in row 7 carry you across. The side
    # currents run downstream, so the way to them leads through a maze of currents and lava. A current below
    # the stage takes you back out (one way).
    'CELADON_CITY': dict(
        rows=[
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "K#LLL...LLL#K",
            "K.LLL...LLL.K",
            "K.EEEE.WWWW.K",
            "KSLLLLSLLLL.K",
            "KS.......LL.K",
            "KS.LLLL..LL.K",
            "KS.LWWW..NN.K",
            "KS.L...L.LL.K",
            "KS.L.L.L....K",
            "KS...L.LLLLLK",
            "K..LLL......K",
            "K...........K",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
            "KKKKKKKKKKKKK",
        ],
        door=(6, 17), leader_front=(6, 5), belt_tag=(64, 128, 232),
        objects={'Kay': (1, 16, 'MOVEMENT_TYPE_FACE_RIGHT', 3), 'Bridget': (8, 9, 'MOVEMENT_TYPE_FACE_LEFT', 3),
                 'Tina': (11, 13, 'MOVEMENT_TYPE_FACE_UP', 2), 'Mary': (8, 13, 'MOVEMENT_TYPE_FACE_UP', 2),
                 'Tamia': (9, 16, 'MOVEMENT_TYPE_FACE_LEFT', 3),
                 'CutTree': [(1, 15, 'MOVEMENT_TYPE_FACE_DOWN', 0), (11, 15, 'MOVEMENT_TYPE_FACE_DOWN', 0), (11, 16, 'MOVEMENT_TYPE_FACE_DOWN', 0)]},
        barrier='rock'),
}
