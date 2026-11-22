#!/usr/bin/env python3
"""Gangster Outbreak: title theme (sound/songs/midi/mus_title.mid, voicegroup rg_rocket_hideout).

A dark boom-bap / G-funk loop in D minor, 88 BPM: string pad and a lonely piano open it, then the beat
drops with synth bass, the West-Coast whistle carries the hook and trumpet stabs hit the accents.
The part between the [ and ] markers loops.

Usage: python3 tools/music/gang_title.py
"""
import os
import struct

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, 'sound/songs/midi/mus_title.mid')

PPQ = 96
BAR = PPQ * 4
BPM = 88

# programs of voicegroup rg_rocket_hideout
DRUMS, PIANO, STRINGS, BASS, TRUMPET, WHISTLE, LEAD, HAT = 0, 1, 48, 38, 56, 78, 80, 127
KICK, SNARE, CLAP, CRASH = 36, 38, 39, 55

# chord loop: Dm - Bb - Gm - A (harmonic minor turn back to Dm)
CHORDS = [(50, 53, 57), (46, 50, 53), (43, 46, 50), (45, 49, 52)]   # D F A / Bb D F / G Bb D / A C# E
ROOTS = [38, 34, 31, 33]                                             # D2 Bb1 G1 A1


class Song:
    def __init__(self):
        self.events = []   # (tick, order, bytes)

    def add(self, tick, data, order=1):
        self.events.append((tick, order, bytes(data)))

    def note(self, ch, tick, key, length, vel):
        self.add(tick, (0x90 | ch, key, vel), 2)
        self.add(tick + max(1, length - 2), (0x80 | ch, key, 0), 0)

    def program(self, ch, prog, vol=100, pan=64, tick=0):
        self.add(tick, (0xC0 | ch, prog))
        self.add(tick, (0xB0 | ch, 7, vol))
        self.add(tick, (0xB0 | ch, 10, pan))

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
        tempo = 60000000 // BPM
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


def compose():
    s = Song()
    s.program(0, STRINGS, 72, 64)
    s.program(1, PIANO, 92, 52)
    s.program(2, BASS, 112, 64)
    s.program(3, WHISTLE, 84, 76)
    s.program(4, TRUMPET, 80, 58)
    s.program(5, HAT, 46, 70)
    s.program(9, DRUMS, 108, 64)

    intro, loop_bars = 4, 16
    # ---- intro: pad + piano, a reversed-crash style swell into the drop
    for bar in range(intro):
        t = bar * BAR
        chord = CHORDS[bar % 4]
        for k in chord[:2]:
            s.note(0, t, k + 12, BAR, 70)
        arp = [chord[0] + 12, chord[1] + 12, chord[2] + 12, chord[1] + 12]
        for i in range(8):
            s.note(1, t + i * PPQ // 2, arp[i % 4] + (12 if i == 6 else 0), PPQ // 2, 78 if i % 2 == 0 else 60)
    for i in range(8):  # snare roll in the last bar of the intro
        s.note(9, (intro - 1) * BAR + BAR // 2 + i * PPQ // 4, SNARE, PPQ // 4, 50 + i * 8)

    start = intro * BAR
    s.marker(start, '[')
    # whistle hook (8 bars, played twice; second time an octave higher at the end)
    hook = [  # (beat offset in 8ths, key, length in 8ths)
        (0, 74, 3), (3, 77, 1), (4, 76, 2), (6, 74, 2),
        (8, 70, 4), (12, 69, 2), (14, 70, 2),
        (16, 67, 3), (19, 70, 1), (20, 69, 2), (22, 67, 2),
        (24, 69, 6), (30, 73, 2),
        (32, 74, 3), (35, 77, 1), (36, 81, 2), (38, 79, 2),
        (40, 77, 4), (44, 74, 2), (46, 77, 2),
        (48, 79, 3), (51, 77, 1), (52, 76, 2), (54, 74, 2),
        (56, 73, 4), (60, 76, 2), (62, 69, 2),
    ]
    for bar in range(loop_bars):
        t = start + bar * BAR
        ci = bar % 4
        chord, root = CHORDS[ci], ROOTS[ci]
        # drums: boom-bap with a swung ghost kick
        for beat, key, vel in ((0, KICK, 120), (1, SNARE, 112), (2.5, KICK, 104), (3, SNARE, 116)):
            s.note(9, t + int(beat * PPQ), key, PPQ // 2, vel)
        if bar % 2:
            s.note(9, t + int(3.75 * PPQ), KICK, PPQ // 4, 84)
            s.note(9, t + 3 * PPQ, CLAP, PPQ // 2, 90)
        if bar % 8 == 0:
            s.note(9, t, CRASH, BAR, 96)
        for i in range(8):  # hats, 8ths with accents and a 16th fill every 4th bar
            s.note(5, t + i * PPQ // 2 + (6 if i % 2 else 0), 60, PPQ // 4, 100 if i % 2 == 0 else 70)
        if bar % 4 == 3:
            for i in range(4):
                s.note(5, t + 3 * PPQ + i * PPQ // 4, 60, PPQ // 8, 80)
        # bass: root, octave bounce, walk into the next chord
        nxt = ROOTS[(ci + 1) % 4]
        line = [(0, root, 3), (3, root + 12, 1), (4, root, 2), (6, root + 7, 1), (7, nxt + (1 if nxt < root else -1), 1)]
        for pos, key, ln in line:
            s.note(2, t + pos * PPQ // 2, key, ln * PPQ // 2, 118 if pos == 0 else 96)
        # pad: two notes, sustained
        for k in chord[1:]:
            s.note(0, t, k + 12, BAR, 60)
        # piano: eerie off-beat stabs
        for pos in (1, 3, 5):
            for k in chord[1:]:
                s.note(1, t + pos * PPQ // 2 + 4, k + 24, PPQ // 3, 64)
        # trumpet stabs on the turnaround
        if ci == 3:
            for pos in (4, 5.5):
                for k in (chord[0] + 12, chord[2] + 12):
                    s.note(4, t + int(pos * PPQ), k, PPQ // 3, 100)
    for rep in range(2):
        for pos, key, ln in hook:
            s.note(3, start + rep * 8 * BAR + pos * PPQ // 2, key + (12 if rep and pos >= 56 else 0), ln * PPQ // 2 - 6, 96)
    end = start + loop_bars * BAR
    s.marker(end, ']')
    return s


if __name__ == '__main__':
    compose().write(OUT)
    print('wrote', OUT)
