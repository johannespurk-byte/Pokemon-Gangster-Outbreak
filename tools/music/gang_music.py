#!/usr/bin/env python3
"""Gangster Outbreak: the soundtrack for battles, routes and towns.

Writes new MIDI files over the FRLG originals (sound/songs/midi/mus_rg_*.mid); all of them play with the
voicegroup rg_rocket_hideout (set in midi.cfg), like the title theme (tools/music/gang_title.py).

Three styles, all in minor keys:
  battle  150-172 BPM: breakbeat drums, driving synth bass, distorted power chords, square-wave lead,
          trumpet stabs; gym bosses and the champion get timpani, strings and a heavier groove
  route   92-100 BPM: boom-bap beat, bouncing bass, whistle or square lead over piano chords
  town    76-88 BPM: slow G-funk / noir: lazy beat, organ or nylon guitar, strings; Lavender is a ghostly
          version with tubular bells and no drums

Each song has its own key, chords, tempo and melody (the melodies are built from a seeded random walk over the
chord tones, with an A A' B A'' phrase structure, so every song is different but stays the same on every run).
The part between the [ and ] markers loops.

Usage: python3 tools/music/gang_music.py
"""
import os
import random
import struct

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
MIDI_DIR = os.path.join(ROOT, 'sound/songs/midi')

PPQ = 96
BAR = PPQ * 4

# programs of voicegroup rg_rocket_hideout
DRUMS, PIANO, BELL, ORGAN, NYLON, OVERDRIVE, DIST_HI, BASS, SQUARE_DS = 0, 1, 14, 17, 24, 29, 30, 38, 39
TIMPANI, STRINGS, TRUMPET, HORN, DIST_LO, WHISTLE, LEAD, LEAD2, WAVE, HAT = 47, 48, 56, 60, 62, 78, 80, 81, 87, 127
# drum keys of the FRLG drum set
KICK, SNARE, SNARE2, CLAP, TOM_LO, TOM_MID, TOM_HI, CRASH, RIDE, TAMB, CRASH2, TAIKO = 36, 37, 38, 39, 41, 45, 48, 49, 53, 54, 55, 87

MINOR = [0, 2, 3, 5, 7, 8, 10]   # natural minor scale steps


class Song:
    def __init__(self, bpm):
        self.bpm = bpm
        self.events = []

    def add(self, tick, data, order=1):
        self.events.append((tick, order, bytes(data)))

    def note(self, ch, tick, key, length, vel):
        vel = max(1, min(127, int(vel)))
        self.add(tick, (0x90 | ch, key, vel), 2)
        self.add(tick + max(1, length - 2), (0x80 | ch, key, 0), 0)

    def program(self, ch, prog, vol=100, pan=64):
        self.add(0, (0xC0 | ch, prog))
        self.add(0, (0xB0 | ch, 7, vol))
        self.add(0, (0xB0 | ch, 10, pan))

    def marker(self, tick, text):
        t = text.encode()
        self.add(tick, (0xFF, 0x06, len(t)) + tuple(t), 0)

    def write(self, path):
        def vlq(n):
            out = [n & 0x7F]
            n >>= 7
            while n:
                out.insert(0, (n & 0x7F) | 0x80)
                n >>= 7
            return bytes(out)
        tempo = 60000000 // self.bpm
        track = vlq(0) + bytes((0xFF, 0x51, 3)) + tempo.to_bytes(3, 'big')
        track += vlq(0) + bytes((0xFF, 0x58, 4, 4, 2, 24, 8))
        last = 0
        for tick, _, data in sorted(self.events, key=lambda e: (e[0], e[1])):
            track += vlq(tick - last) + data
            last = tick
        track += vlq(0) + bytes((0xFF, 0x2F, 0))
        with open(path, 'wb') as f:
            f.write(b'MThd' + struct.pack('>IHHH', 6, 0, 1, PPQ))
            f.write(b'MTrk' + struct.pack('>I', len(track)) + track)


# ------------------------------------------------------------------ harmony

def chord(tonic, degree, harmonic_v=True):
    """triad on a scale degree (0-6) of the minor key with tonic `tonic` (MIDI key); V is major if harmonic_v"""
    notes = []
    for step in (0, 2, 4):
        d = degree + step
        key = tonic + MINOR[d % 7] + 12 * (d // 7)
        if harmonic_v and degree == 4 and step == 2:
            key += 1   # raised leading tone
        notes.append(key)
    return notes


def scale_keys(tonic, low, high):
    return [k for k in range(low, high + 1) if (k - tonic) % 12 in MINOR]


# ------------------------------------------------------------------ melody

RHYTHMS = [  # 2-bar rhythms in 8ths: (position, length)
    [(0, 3), (3, 1), (4, 2), (6, 2), (8, 4), (12, 2), (14, 2)],
    [(0, 2), (2, 1), (3, 1), (4, 2), (6, 2), (8, 3), (11, 1), (12, 4)],
    [(0, 1), (1, 1), (2, 2), (4, 1), (5, 1), (6, 2), (8, 6), (14, 2)],
    [(0, 4), (4, 2), (6, 1), (7, 1), (8, 2), (10, 2), (12, 4)],
    [(1, 1), (2, 2), (4, 3), (7, 1), (8, 2), (10, 1), (11, 1), (12, 4)],
    [(0, 2), (2, 2), (4, 2), (6, 2), (8, 2), (10, 2), (12, 2), (14, 2)],
]


def make_phrase(rng, tonic, chords2, low, high, start_key, rhythm, end_on_root=False):
    """2 bars of melody over two chords; returns [(pos8, key, len8)]"""
    keys = scale_keys(tonic, low, high)
    out, cur = [], start_key
    for i, (pos, ln) in enumerate(rhythm):
        ch = chords2[0] if pos < 8 else chords2[1]
        strong = pos % 4 == 0 or ln >= 3
        if strong:
            cands = [k for k in keys if (k - ch[0]) % 12 in [(c - ch[0]) % 12 for c in ch]]
        else:
            cands = keys
        cands = sorted(cands, key=lambda k: abs(k - cur))[:4]
        nxt = rng.choice(cands[:3] if len(cands) >= 3 else cands)
        if i == len(rhythm) - 1 and end_on_root:
            nxt = min((k for k in keys if (k - tonic) % 12 == 0), key=lambda k: abs(k - cur))
        out.append((pos, nxt, ln))
        cur = nxt
    return out


def make_melody(rng, tonic, prog, low, high, bars):
    """A A' B A'' over `bars` bars (prog: one chord per bar, repeating)"""
    mel = []
    rA, rB = rng.choice(RHYTHMS), rng.choice(RHYTHMS)
    mid = (low + high) // 2
    phrase_a = make_phrase(rng, tonic, [prog[0], prog[1 % len(prog)]], low, high, mid, rA)
    for p in range(bars // 2):
        c2 = [prog[(2 * p) % len(prog)], prog[(2 * p + 1) % len(prog)]]
        kind = p % 4
        if kind == 0:
            ph = phrase_a
        elif kind == 1:   # A': same rhythm, answer that ends on the tonic
            ph = make_phrase(rng, tonic, c2, low, high, phrase_a[-1][1], rA, end_on_root=(p % 8 == 7))
        elif kind == 2:   # B: new rhythm, a bit higher
            ph = make_phrase(rng, tonic, c2, low + 3, high, high - 3, rB)
        else:             # A'': first half of A, new ending
            head = [n for n in phrase_a if n[0] < 8]
            tail_rhythm = [(pos, ln) for pos, ln in rA if pos >= 8] or [(8, 8)]
            tail = make_phrase(rng, tonic, c2, low, high, head[-1][1] if head else low, tail_rhythm, end_on_root=True)
            ph = head + tail
        for pos, key, ln in ph:
            mel.append((p * 16 + pos, key, ln))
    return mel


# ------------------------------------------------------------------ styles

def battle(name, tonic, prog_a, prog_b, bpm, seed, heavy=False, champion=False):
    rng = random.Random(seed)
    s = Song(bpm)
    s.program(0, DIST_LO, 92, 50)       # power chords
    s.program(1, BASS, 124, 64)
    s.program(2, LEAD, 92, 74)          # square lead (CGB)
    s.program(3, LEAD2, 60, 44)         # harmony / arps (CGB)
    s.program(4, TRUMPET if not heavy else HORN, 100, 80)
    s.program(5, HAT, 54, 70)
    s.program(6, STRINGS if heavy else DIST_HI, 80, 60)
    s.program(7, TIMPANI, 110, 64)
    s.program(9, DRUMS, 127, 64)

    prog = [chord(tonic, d) for d in prog_a] * 2 + [chord(tonic, d) for d in prog_b] * 2   # 16 bars
    intro = 2
    # intro: power-chord hits and a tom run
    for bar in range(intro):
        t = bar * BAR
        c = prog[0]
        for pos in (0, 3, 6) if bar == 0 else (0, 3):
            for k in (c[0] - 12, c[0] - 5):
                s.note(0, t + pos * PPQ // 2, k, PPQ // 2, 110)
            s.note(1, t + pos * PPQ // 2, c[0] - 24, PPQ // 2, 120)
            s.note(9, t + pos * PPQ // 2, KICK, PPQ // 2, 120)
        s.note(9, t, CRASH, BAR, 100 if bar == 0 else 0)
        if heavy:
            s.note(7, t, tonic - 24, PPQ, 120)
    for i, k in enumerate((TOM_HI, TOM_HI, TOM_MID, TOM_MID, TOM_LO, TOM_LO, SNARE2, SNARE2)):
        s.note(9, (intro - 1) * BAR + BAR // 2 + i * PPQ // 4, k, PPQ // 4, 80 + i * 6)
    start = intro * BAR
    s.marker(start, '[')
    mel = make_melody(rng, tonic, prog, tonic + 12, tonic + 26, 16)
    for bar in range(16):
        t = start + bar * BAR
        c = prog[bar]
        root = c[0] - 24
        # drums: driving rock/breakbeat
        pattern = [(0, KICK, 122), (1, SNARE2, 116), (1.75, KICK, 100), (2, KICK, 112), (3, SNARE2, 118)]
        if heavy:
            pattern += [(2.5, KICK, 96), (0.5, KICK, 90)]
        for beat, key, vel in pattern:
            s.note(9, t + int(beat * PPQ), key, PPQ // 2, vel)
        if bar % 4 == 3:
            for i in range(4):
                s.note(9, t + 3 * PPQ + i * PPQ // 4, (TOM_HI, TOM_MID, TOM_LO, SNARE2)[i], PPQ // 4, 96)
        if bar % 8 == 0:
            s.note(9, t, CRASH, BAR, 104)
        for i in range(8):
            s.note(5, t + i * PPQ // 2, 60, PPQ // 4, 96 if i % 2 == 0 else 64)
        # bass: 8ths with octave jumps
        for i in range(8):
            k = root + (12 if i in (3, 7) else 0)
            if i == 7 and bar % 2:
                k = prog[(bar + 1) % 16][0] - 24 + (-1 if prog[(bar + 1) % 16][0] > c[0] else 1)
            s.note(1, t + i * PPQ // 2, k, PPQ // 2, 118 if i % 2 == 0 else 98)
        # power chords: palm-muted 8ths, open hits on 1 and the "and" of 2
        for i in range(8):
            open_hit = i in (0, 3)
            for k in (c[0] - 12, c[0] - 5) + ((c[0],) if open_hit else ()):
                s.note(0, t + i * PPQ // 2, k, PPQ // 2 if open_hit else PPQ // 4, 112 if open_hit else 76)
        # arpeggio / counter line
        arp = [c[0], c[1], c[2], c[1] + 12 if c[1] + 12 < 90 else c[1]]
        for i in range(8):
            s.note(3, t + i * PPQ // 2, arp[i % 4], PPQ // 3, 70)
        # stabs on the turnaround
        if bar % 4 == 3:
            for pos in (2, 2.75, 3.5):
                for k in (c[1], c[2]):
                    s.note(4, t + int(pos * PPQ), k, PPQ // 3, 104)
        if heavy:
            for k in c[1:]:
                s.note(6, t, k, BAR, 66)
            if bar % 2 == 0:
                s.note(7, t, tonic - 24 + (7 if bar % 4 == 2 else 0), PPQ // 2, 118)
                s.note(7, t + int(2.5 * PPQ), tonic - 24, PPQ // 2, 100)
        elif bar % 2 == 0:
            s.note(6, t + int(1.5 * PPQ), c[2], PPQ, 70)
    for pos, key, ln in mel:
        s.note(2, start + pos * PPQ // 2, key, ln * PPQ // 2 - 4, 104)
    if champion:   # the lead is doubled by the brass an octave down in the B part
        for pos, key, ln in mel:
            if pos >= 64:
                s.note(4, start + pos * PPQ // 2, key - 12, ln * PPQ // 2 - 4, 92)
    s.marker(start + 16 * BAR, ']')
    return s


def route(name, tonic, prog_a, prog_b, bpm, seed, lead=WHISTLE):
    rng = random.Random(seed)
    s = Song(bpm)
    s.program(0, STRINGS, 78, 64)
    s.program(1, PIANO, 108, 50)
    s.program(2, BASS, 124, 64)
    s.program(3, lead, 104, 78)
    s.program(4, TRUMPET, 92, 58)
    s.program(5, HAT, 58, 70)
    s.program(9, DRUMS, 124, 64)
    prog = [chord(tonic, d) for d in prog_a] * 2 + [chord(tonic, d) for d in prog_b] * 2
    intro = 2
    for bar in range(intro):
        t = bar * BAR
        c = prog[bar]
        for i in range(8):
            s.note(1, t + i * PPQ // 2, [c[0], c[1], c[2], c[1]][i % 4] + 12, PPQ // 2, 74 if i % 2 == 0 else 58)
        for k in c[1:]:
            s.note(0, t, k + 12, BAR, 62)
    start = intro * BAR
    s.marker(start, '[')
    mel = make_melody(rng, tonic, prog, tonic + 12, tonic + 24, 16)
    for bar in range(16):
        t = start + bar * BAR
        c = prog[bar]
        root = c[0] - 24
        for beat, key, vel in ((0, KICK, 120), (1, SNARE, 110), (1.75, KICK, 90), (2.5, KICK, 108), (3, SNARE, 114)):
            s.note(9, t + int(beat * PPQ), key, PPQ // 2, vel)
        if bar % 2:
            s.note(9, t + 3 * PPQ, CLAP, PPQ // 2, 88)
        if bar % 8 == 0:
            s.note(9, t, CRASH2, BAR, 90)
        for i in range(8):
            s.note(5, t + i * PPQ // 2 + (8 if i % 2 else 0), 60, PPQ // 4, 96 if i % 2 == 0 else 66)
        nxt = prog[(bar + 1) % 16][0] - 24
        for pos, key, ln in ((0, root, 2), (2, root + 12, 1), (3, root, 1), (5, root + 7, 1), (6, root, 1), (7, nxt - 1 if nxt > root else nxt + 1, 1)):
            s.note(2, t + pos * PPQ // 2, key, ln * PPQ // 2, 118 if pos == 0 else 94)
        for pos in (1, 2.5, 3.5):   # piano comp
            for k in c:
                s.note(1, t + int(pos * PPQ) + 4, k + 12, PPQ // 3, 66)
        for k in c[1:]:
            s.note(0, t, k + 12, BAR, 56)
        if bar % 8 == 7:
            for pos in (2, 3):
                for k in (c[0] + 12, c[2] + 12):
                    s.note(4, t + pos * PPQ, k, PPQ // 3, 96)
    for pos, key, ln in mel:
        s.note(3, start + pos * PPQ // 2, key, ln * PPQ // 2 - 6, 98)
    s.marker(start + 16 * BAR, ']')
    return s


def town(name, tonic, prog_a, prog_b, bpm, seed, keys=ORGAN, lead=WHISTLE, ghost=False):
    rng = random.Random(seed)
    s = Song(bpm)
    s.program(0, STRINGS, 84, 64)
    s.program(1, keys, 104, 48)
    s.program(2, BASS, 122, 64)
    s.program(3, lead, 100, 80)
    s.program(4, BELL if ghost else NYLON, 86, 36)
    s.program(5, HAT, 50, 70)
    s.program(9, DRUMS, 116, 64)
    prog = [chord(tonic, d) for d in prog_a] * 2 + [chord(tonic, d) for d in prog_b] * 2
    intro = 2
    for bar in range(intro):
        for k in prog[bar]:
            s.note(0, bar * BAR, k + 12, BAR, 66)
        s.note(4, bar * BAR, prog[bar][0] + 24, BAR // 2, 70)
    start = intro * BAR
    s.marker(start, '[')
    mel = make_melody(rng, tonic, prog, tonic + 12, tonic + 24, 16)
    for bar in range(16):
        t = start + bar * BAR
        c = prog[bar]
        root = c[0] - 24
        if not ghost:
            for beat, key, vel in ((0, KICK, 112), (1, SNARE, 100), (2.25, KICK, 92), (2.5, KICK, 100), (3, SNARE, 104)):
                s.note(9, t + int(beat * PPQ), key, PPQ // 2, vel)
            for i in range(4):
                s.note(5, t + i * PPQ + PPQ // 2, 60, PPQ // 4, 70)
            if bar % 8 == 0:
                s.note(9, t, CRASH2, BAR, 76)
            for pos, key, ln in ((0, root, 3), (3, root, 1), (4, root + 7, 2), (6, root + 10 if bar % 2 else root + 12, 2)):
                s.note(2, t + pos * PPQ // 2, key, ln * PPQ // 2, 112 if pos == 0 else 90)
        else:
            s.note(2, t, root, BAR, 80)
            if bar % 2 == 0:
                s.note(9, t, TAIKO, PPQ, 70)
        # keys: lazy off-beat chords
        for pos in (0.5, 1.5, 2.5, 3.25) if not ghost else (0,):
            for k in c:
                s.note(1, t + int(pos * PPQ), k + 12, PPQ // 2 if not ghost else BAR, 70 if not ghost else 60)
        for k in c[1:]:
            s.note(0, t, k + 12, BAR, 58)
        # plucked arpeggio / bells
        arp = [c[0] + 24, c[2] + 12, c[1] + 24, c[2] + 12]
        for i in range(4):
            s.note(4, t + i * PPQ, arp[i], PPQ, 62 if not ghost else 74)
    for pos, key, ln in mel:
        s.note(3, start + pos * PPQ // 2, key, ln * PPQ // 2 - 6, 92)
    s.marker(start + 16 * BAR, ']')
    return s


# degrees: 0=i 1=ii° 2=III 3=iv 4=V 5=VI 6=VII
SONGS = {
    # battles
    'mus_rg_vs_wild':       lambda: battle('wild', 52, [0, 5, 6, 0], [3, 5, 4, 4], 156, 11),              # E minor
    'mus_rg_vs_trainer':    lambda: battle('trainer', 50, [0, 6, 5, 4], [3, 0, 5, 4], 162, 23),           # D minor
    'mus_rg_vs_gym_leader': lambda: battle('boss', 45, [0, 3, 5, 4], [5, 6, 0, 4], 168, 37, heavy=True),  # A minor
    'mus_rg_vs_champion':   lambda: battle('champ', 48, [0, 5, 2, 6], [3, 4, 0, 4], 172, 41, heavy=True, champion=True),  # C minor
    # routes
    'mus_rg_route1':  lambda: route('route1', 55, [0, 5, 2, 6], [3, 0, 5, 4], 94, 101),                  # G minor
    'mus_rg_route3':  lambda: route('route3', 53, [0, 3, 6, 2], [5, 6, 0, 4], 98, 103, lead=LEAD),       # F minor
    'mus_rg_route11': lambda: route('route11', 57, [0, 6, 5, 6], [3, 5, 0, 4], 96, 107),                 # A minor
    'mus_rg_route24': lambda: route('route24', 52, [0, 2, 5, 4], [3, 6, 2, 4], 100, 109, lead=LEAD),     # E minor
    # towns
    'mus_rg_pallet':     lambda: town('pallet', 50, [0, 5, 3, 4], [5, 6, 0, 4], 82, 201),                         # D minor
    'mus_rg_pewter':     lambda: town('pewter', 52, [0, 3, 0, 4], [5, 2, 6, 4], 84, 203, keys=PIANO),             # E minor
    'mus_rg_fuchsia':    lambda: town('cerulean', 55, [0, 6, 5, 4], [3, 2, 5, 4], 80, 205, keys=NYLON, lead=LEAD),  # G minor
    'mus_rg_vermillion': lambda: town('vermilion', 53, [0, 5, 6, 2], [3, 4, 0, 4], 86, 207),                      # F minor
    'mus_rg_celadon':    lambda: town('celadon', 57, [0, 3, 6, 2], [5, 3, 4, 4], 88, 209, keys=PIANO, lead=LEAD), # A minor
    'mus_rg_cinnabar':   lambda: town('cinnabar', 48, [0, 5, 0, 4], [3, 5, 6, 4], 84, 211),                       # C minor
    'mus_rg_lavender':   lambda: town('lavender', 54, [0, 5, 3, 4], [0, 1, 5, 4], 72, 213, keys=STRINGS, lead=WAVE, ghost=True),  # F# minor
}


if __name__ == '__main__':
    for name, make in SONGS.items():
        make().write(os.path.join(MIDI_DIR, name + '.mid'))
        print('wrote', name)
