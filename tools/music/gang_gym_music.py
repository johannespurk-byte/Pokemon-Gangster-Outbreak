#!/usr/bin/env python3
"""Gangster Outbreak: one theme per GYM, in the style of the gang that holds it.

Writes sound/songs/midi/mus_go_gym_*.mid (registered in include/constants/songs.h, sound/song_table.inc and
midi.cfg, all with the voicegroup rg_rocket_hideout). The building blocks (Song, chord, make_melody, instrument
and drum numbers) come from gang_music.py.

  PEWTER    TEAM YELL      punk rock: 184 BPM, two-beat drums, fuzz chords, gang-shout stabs
  CERULEAN  TEAM SKULL     boom-bap: 88 BPM, heavy swung beat, sub bass, square "rap" lead
  VERMILION NEO ROCKET     military march: 116 BPM, snare rolls, brass fanfare, timpani
  CELADON   AQUA + MAGMA   tribal: 108 BPM, taiko and toms, organ against overdrive guitar
  FUCHSIA   PLASMA         cathedral: 92 BPM, church organ, strings, slow tolling bells
  SAFFRON   FLARE          electro disco: 124 BPM, four-on-the-floor, octave bass, glittering arps
  CINNABAR  GALACTIC       cold space synth: 112 BPM, wave lead, sparse beat, echoing bells
  VIRIDIAN  ROCKET         mafia noir: 96 BPM, strings, muted trumpet, timpani, walking bass

Usage: python3 tools/music/gang_gym_music.py
"""
import os
import random
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gang_music import (Song, chord, make_melody, MIDI_DIR, PPQ, BAR,  # noqa: E402
                        DRUMS, PIANO, BELL, ORGAN, OVERDRIVE, DIST_HI, BASS, TIMPANI, STRINGS, TRUMPET, HORN,
                        DIST_LO, WHISTLE, LEAD, LEAD2, WAVE, HAT,
                        KICK, SNARE, SNARE2, CLAP, TOM_LO, TOM_MID, TOM_HI, CRASH, CRASH2, TAIKO)

# per style: (bpm, channel programs, lead channel program, drum pattern [(beat, key, vel)], bass pattern
# [(8th, offset, length8)], chord comp positions in beats (None = held), extras)
STYLES = {
    'punk': dict(chords=OVERDRIVE, pad=DIST_HI, lead=LEAD, stab=TRUMPET,
                 drums=[(0, KICK, 124), (0.5, KICK, 96), (1, SNARE2, 122), (1.5, KICK, 100),
                        (2, KICK, 118), (2.5, KICK, 96), (3, SNARE2, 124), (3.5, SNARE2, 90)],
                 bass=[(i, 0, 1) for i in range(8)], comp=[i * 0.5 for i in range(8)], hat=8),
    'boombap': dict(chords=PIANO, pad=STRINGS, lead=LEAD2, stab=HORN,
                    drums=[(0, KICK, 124), (0.75, KICK, 100), (1, SNARE, 120), (2.25, KICK, 112),
                           (2.5, KICK, 104), (3, SNARE, 124), (3.75, CLAP, 80)],
                    bass=[(0, 0, 3), (3, 0, 1), (5, 7, 1), (6, 0, 2)], comp=[0.5, 2.75], hat=8),
    'march': dict(chords=HORN, pad=STRINGS, lead=TRUMPET, stab=TRUMPET,
                  drums=[(0, KICK, 120), (1, SNARE2, 110), (1.5, SNARE2, 80), (1.75, SNARE2, 86),
                         (2, KICK, 118), (3, SNARE2, 112), (3.5, SNARE2, 90)],
                  bass=[(0, 0, 2), (2, 7, 2), (4, 0, 2), (6, 7, 2)], comp=[0, 1, 2, 3], hat=0, timpani=True),
    'tribal': dict(chords=ORGAN, pad=OVERDRIVE, lead=WHISTLE, stab=DIST_LO,
                   drums=[(0, TAIKO, 124), (0.75, TOM_LO, 100), (1.5, TAIKO, 110), (2, TOM_MID, 104),
                          (2.5, TAIKO, 118), (3, TOM_HI, 100), (3.5, TOM_LO, 96)],
                   bass=[(0, 0, 3), (3, 0, 1), (4, 0, 2), (6, 10, 2)], comp=[0, 1.5, 2.5], hat=0),
    'cathedral': dict(chords=ORGAN, pad=STRINGS, lead=WAVE, stab=BELL,
                      drums=[(0, KICK, 100), (2, SNARE, 86)],
                      bass=[(0, 0, 8)], comp=None, hat=0, bells=True),
    'disco': dict(chords=PIANO, pad=STRINGS, lead=LEAD, stab=TRUMPET,
                  drums=[(0, KICK, 124), (1, KICK, 120), (1, CLAP, 104), (2, KICK, 122), (3, KICK, 120), (3, CLAP, 108)],
                  bass=[(i, 0 if i % 2 == 0 else 12, 1) for i in range(8)], comp=[0.5, 1.5, 2.5, 3.5], hat=16, arp=True),
    'space': dict(chords=STRINGS, pad=WAVE, lead=WAVE, stab=BELL,
                  drums=[(0, KICK, 110), (1.5, KICK, 90), (2, CLAP, 96), (3.75, KICK, 84)],
                  bass=[(0, 0, 2), (3, 0, 1), (4, -2, 2), (7, 0, 1)], comp=None, hat=4, arp=True, bells=True),
    'noir': dict(chords=PIANO, pad=STRINGS, lead=TRUMPET, stab=HORN,
                 drums=[(0, KICK, 110), (1, SNARE, 90), (2, KICK, 100), (2.5, KICK, 84), (3, SNARE, 100)],
                 bass=[(0, 0, 2), (2, 3, 2), (4, 7, 2), (6, 10, 2)], comp=[1, 3], hat=8, timpani=True),
}


def gym(tonic, prog_a, prog_b, bpm, seed, style):
    st = STYLES[style]
    rng = random.Random(seed)
    s = Song(bpm)
    s.program(0, st['chords'], 86, 46)
    s.program(1, BASS, 124, 64)
    s.program(2, st['lead'], 100, 76)
    s.program(3, st['pad'], 70, 84)
    s.program(4, st['stab'], 96, 60)
    s.program(5, HAT, 54, 70)
    s.program(6, LEAD2, 52, 40)     # arps
    s.program(7, TIMPANI, 112, 64)
    s.program(9, DRUMS, 124, 64)

    prog = [chord(tonic, d) for d in prog_a] * 2 + [chord(tonic, d) for d in prog_b] * 2   # 16 bars
    intro = 2
    for bar in range(intro):
        t = bar * BAR
        c = prog[0]
        for k in c:
            s.note(3, t, k + 12, BAR, 72)
        s.note(1, t, c[0] - 24, BAR, 110)
        if st.get('timpani') or style in ('punk', 'tribal'):
            for i in range(4 if bar else 2):
                s.note(7 if st.get('timpani') else 9, t + i * PPQ, tonic - 24 if st.get('timpani') else TAIKO, PPQ // 2, 90 + i * 8)
    s.note(9, BAR + 3 * PPQ, SNARE2, PPQ // 4, 100)
    s.note(9, BAR + 3 * PPQ + PPQ // 2, SNARE2, PPQ // 4, 110)
    start = intro * BAR
    s.marker(start, '[')
    mel = make_melody(rng, tonic, prog, tonic + 12, tonic + 26 if style != 'cathedral' else tonic + 22, 16)
    for bar in range(16):
        t = start + bar * BAR
        c = prog[bar]
        root = c[0] - 24
        for beat, key, vel in st['drums']:
            s.note(9, t + int(beat * PPQ), key, PPQ // 2, vel)
        if bar % 8 == 0:
            s.note(9, t, CRASH if style in ('punk', 'march', 'tribal') else CRASH2, BAR, 100)
        if bar % 4 == 3 and style in ('punk', 'march', 'noir'):
            for i in range(4):
                s.note(9, t + 3 * PPQ + i * PPQ // 4, (TOM_HI, TOM_MID, TOM_LO, SNARE2)[i], PPQ // 4, 96)
        if st['hat']:
            step = BAR // st['hat']
            for i in range(st['hat']):
                s.note(5, t + i * step, 60, step // 2, 92 if i % 2 == 0 else 62)
        for pos, off, ln in st['bass']:
            s.note(1, t + pos * PPQ // 2, root + off, ln * PPQ // 2, 118 if pos == 0 else 98)
        if st['comp'] is None:
            for k in c:
                s.note(0, t, k + 12, BAR, 70)
        else:
            for pos in st['comp']:
                for k in c:
                    s.note(0, t + int(pos * PPQ), k + 12, PPQ // 3 if style != 'march' else PPQ // 2, 80)
        for k in c[1:]:
            s.note(3, t, k + 12, BAR, 58)
        if st.get('arp'):
            arp = [c[0] + 24, c[1] + 24, c[2] + 24, c[1] + 36]
            for i in range(8):
                s.note(6, t + i * PPQ // 2, arp[i % 4], PPQ // 3, 64)
        if st.get('bells') and bar % 2 == 0:
            s.note(4, t, c[0] + 24, BAR, 70)
        elif bar % 4 == 3:
            for pos in (2, 2.75, 3.5):
                for k in (c[1] + 12, c[2] + 12):
                    s.note(4, t + int(pos * PPQ), k, PPQ // 3, 100)
        if st.get('timpani') and bar % 2 == 0:
            s.note(7, t, tonic - 24, PPQ // 2, 116)
            s.note(7, t + 2 * PPQ, tonic - 17, PPQ // 2, 100)
    for pos, key, ln in mel:
        s.note(2, start + pos * PPQ // 2, key, ln * PPQ // 2 - 4, 100)
    s.marker(start + 16 * BAR, ']')
    return s


# degrees: 0=i 1=ii° 2=III 3=iv 4=V 5=VI 6=VII
GYMS = {
    'mus_go_gym_yell':     lambda: gym(52, [0, 6, 5, 6], [3, 5, 6, 4], 184, 301, 'punk'),       # E minor
    'mus_go_gym_skull':    lambda: gym(50, [0, 3, 0, 4], [5, 3, 6, 4], 88, 303, 'boombap'),     # D minor
    'mus_go_gym_rocket_a': lambda: gym(48, [0, 4, 0, 4], [5, 3, 4, 4], 116, 307, 'march'),      # C minor
    'mus_go_gym_aquamagma': lambda: gym(53, [0, 6, 0, 5], [3, 6, 4, 4], 108, 311, 'tribal'),    # F minor
    'mus_go_gym_plasma':   lambda: gym(47, [0, 3, 5, 4], [0, 1, 5, 4], 92, 313, 'cathedral'),   # B minor
    'mus_go_gym_flare':    lambda: gym(55, [0, 5, 2, 6], [3, 5, 6, 4], 124, 317, 'disco'),      # G minor
    'mus_go_gym_galactic': lambda: gym(54, [0, 5, 3, 6], [0, 2, 5, 4], 112, 331, 'space'),      # F# minor
    'mus_go_gym_rocket_v': lambda: gym(45, [0, 3, 4, 0], [5, 1, 4, 4], 96, 337, 'noir'),        # A minor
}

# map -> song (the map.json "music" field)
GYM_MAPS = {
    'PewterCity_Gym': 'mus_go_gym_yell', 'CeruleanCity_Gym': 'mus_go_gym_skull',
    'VermilionCity_Gym': 'mus_go_gym_rocket_a', 'CeladonCity_Gym': 'mus_go_gym_aquamagma',
    'FuchsiaCity_Gym': 'mus_go_gym_plasma', 'SaffronCity_Gym': 'mus_go_gym_flare',
    'CinnabarIsland_Gym': 'mus_go_gym_galactic', 'ViridianCity_Gym': 'mus_go_gym_rocket_v',
}


if __name__ == '__main__':
    for name, make in GYMS.items():
        make().write(os.path.join(MIDI_DIR, name + '.mid'))
        print('wrote', name)
