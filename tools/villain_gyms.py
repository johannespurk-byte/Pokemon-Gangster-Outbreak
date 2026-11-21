#!/usr/bin/env python3
"""Gangster Outbreak: the Kanto gyms are taken over by the villain teams of generations 2-8,
from the weakest to the most dangerous (Giovanni keeps gym 8). One-off edit, refuses to run twice.

Gym 1 Pewter  Team Yell (Gen 8)       PIERS
Gym 2 Cerulean Team Skull (Gen 7)     GUZMA
Gym 3 Vermilion Neo Rocket (Gen 2)    ARCHER
Gym 4 Celadon Team Aqua & Magma (G3)  ARCHIE + MAXIE (double battle)
Gym 5 Fuchsia Team Plasma (Gen 5)     GHETSIS
Gym 6 Saffron Team Flare (Gen 6)      LYSANDRE
Gym 7 Cinnabar Team Galactic (Gen 4)  CYRUS
"""
import json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def rd(p):
    return open(os.path.join(ROOT, p)).read()


def wr(p, s):
    open(os.path.join(ROOT, p), 'w').write(s)


# leader trainer, map, old name(s) in texts, new name, class, pic, gender, overworld gfx (None = keep), team
BOSSES = [
    ('TRAINER_LEADER_BROCK', 'PewterCity', ['BROCK'], 'PIERS', 'Leader Frlg', 'Leader Brock Frlg', 'Male', None,
     [('Zigzagoon-Galar', 12), ('Scraggy', 12), ('Inkay', 13), ('Linoone-Galar', 14)]),
    ('TRAINER_LEADER_MISTY', 'CeruleanCity', ['MISTY'], 'GUZMA', 'Leader Frlg', 'Leader Misty Frlg', 'Male',
     'OBJ_EVENT_GFX_SKULL_GRUNT',
     [('Spinarak', 18), ('Surskit', 18), ('Dewpider', 19), ('Wimpod', 21)]),
    ('TRAINER_LEADER_LT_SURGE', 'VermilionCity', ['LT. SURGE', 'LT.SURGE'], 'ARCHER', 'Team Rocket Frlg',
     'Rocket Grunt M Frlg', 'Male', 'OBJ_EVENT_GFX_ROCKET_M',
     [('Zubat', 21), ('Koffing', 22), ('Houndour', 23), ('Golbat', 24)]),
    ('TRAINER_LEADER_ERIKA', 'CeladonCity', ['ERIKA'], 'ARCHIE', 'Aqua Leader', 'Aqua Leader Archie', 'Male',
     'OBJ_EVENT_GFX_ARCHIE',
     [('Carvanha', 27), ('Mightyena', 28), ('Sharpedo', 29)]),
    ('TRAINER_LEADER_KOGA', 'FuchsiaCity', ['KOGA'], 'GHETSIS', 'Leader Frlg', 'Ghetsis Bw2', 'Male', None,
     [('Cofagrigus', 38), ('Bouffalant', 38), ('Seismitoad', 39), ('Bisharp', 40), ('Eelektross', 41), ('Hydreigon', 43)]),
    ('TRAINER_LEADER_SABRINA', 'SaffronCity', ['SABRINA'], 'LYSANDRE', 'Leader Frlg', 'Leader Sabrina Frlg', 'Male', None,
     [('Mienshao', 38), ('Honchkrow', 39), ('Pyroar', 40), ('Gyarados', 43)]),
    ('TRAINER_LEADER_BLAINE', 'CinnabarIsland', ['BLAINE'], 'CYRUS', 'Leader Frlg', 'Cyrus', 'Male', 'OBJ_EVENT_GFX_CYRUS',
     [('Houndoom', 42), ('Crobat', 43), ('Honchkrow', 44), ('Gyarados', 45), ('Weavile', 47)]),
]
MAXIE = ('TRAINER_GANG_MAXIE', 'MAXIE', 'Magma Leader', 'Magma Leader Maxie', 'Male',
         [('Numel', 27), ('Mightyena', 28), ('Camerupt', 29)])

INTROS = {
    'PewterCity': ["I'm PIERS, boss of TEAM YELL.\\n", "Our gang used to cheer for the\\l", "GYM challengers in GALAR.\\p",
                   "Now we run this GYM. Show me some\\n", "noise, GIOVANNI's new blood!"],
    'CeruleanCity': ["GUZMA! That's my name, the guy who\\n", "beats you down and keeps on going!\\p",
                     "TEAM SKULL owns this GYM now.\\n", "Let's see if you're worth it!"],
    'VermilionCity': ["I'm ARCHER, executive of the NEW\\n", "TEAM ROCKET from JOHTO.\\p",
                      "GIOVANNI left us behind. I'll see\\n", "if his new recruit is any better!"],
    'CeladonCity': ["ARCHIE: Bwahahaha! TEAM AQUA wants\\n", "the sea, TEAM MAGMA wants the land.\\p",
                    "MAXIE: For once we fight together.\\n", "Two bosses, one GYM. Ready?"],
    'FuchsiaCity': ["I am GHETSIS of TEAM PLASMA.\\p", "POKéMON exist to serve the ones\\n",
                    "who rule. Kneel before my power!"],
    'SaffronCity': ["I am LYSANDRE of TEAM FLARE.\\p", "This world is ugly. Only the chosen\\n",
                    "deserve to see its beautiful end.\\p", "Prove that you are one of them!"],
    'CinnabarIsland': ["I am CYRUS, leader of TEAM\\n", "GALACTIC.\\p", "Spirit and emotion are flaws.\\n",
                       "I will erase them and remake the\\l", "universe. Starting with you."],
}
POSTS = {
    'PewterCity': ["Heh. TEAM YELL will cheer for you\\n", "from now on. Don't let us down."],
    'VermilionCity': ["The NEW TEAM ROCKET bows to you.\\n", "Tell GIOVANNI we're waiting."],
    'CeladonCity': ["ARCHIE: Land or sea, you rule both\\n", "now. Bwahahaha!"],
    'FuchsiaCity': ["Hmph. Even I can use a pawn as\\n", "strong as you."],
    'SaffronCity': ["You are beautiful in your own way.\\n", "TEAM FLARE salutes you."],
    'CinnabarIsland': ["Your spirit beat my logic.\\n", "Fascinating… and unacceptable."],
}
MAXIE_DEFEAT = ["MAXIE: Fuhahaha… Remarkable.$"]
NEED_TWO = ["ARCHIE: Come back with at least two\\n", "POKéMON that can still fight!$"]

# gym trainers: map -> (new class, pic or None, overworld gfx list or None)
GRUNTS = {
    'PewterCity': ('Team Yell', None, None),
    'CeruleanCity': ('Team Skull', None, ['OBJ_EVENT_GFX_SKULL_GRUNT']),
    'VermilionCity': ('Team Rocket Frlg', 'Rocket Grunt M Frlg', ['OBJ_EVENT_GFX_ROCKET_M']),
    'CeladonCity': ('AQUA_MAGMA', None, ['OBJ_EVENT_GFX_AQUA_MEMBER_F', 'OBJ_EVENT_GFX_MAGMA_MEMBER_F']),
    'FuchsiaCity': ('Team Plasma', 'Plasma Grunt M Bw2', None),
    'SaffronCity': ('Team Flare', None, None),
    'CinnabarIsland': ('Team Galactic', None, None),
}
NEW_CLASSES = [('TEAM_PLASMA', 'TEAM PLASMA'), ('TEAM_GALACTIC', 'GALACTIC'), ('TEAM_SKULL', 'TEAM SKULL'),
               ('TEAM_YELL', 'TEAM YELL'), ('TEAM_FLARE', 'TEAM FLARE')]


def party_block(party, const):
    m = re.search(r'^=== %s ===\n.*?(?=^=== |\Z)' % re.escape(const), party, re.M | re.S)
    assert m, const
    return m


def boss_entry(const, name, cls, pic, gender, mons, double=False):
    out = ['=== %s ===' % const, 'Name: %s' % name, 'Class: %s' % cls, 'Pic: %s' % pic, 'Gender: %s' % gender,
           'Music: Serious', 'Double Battle: %s' % ('Yes' if double else 'No'),
           'AI: Check Bad Move / Try To Faint / Check Viability', '']
    for species, level in mons:
        out += [species, 'Level: %d' % level, '']
    return '\n'.join(out) + '\n'


def set_text(s, label, lines):
    m = re.search(r'^%s::\n((?:\t\.string [^\n]*\n)+)' % re.escape(label), s, re.M)
    assert m, label
    body = ''.join('\t.string "%s"\n' % l for l in lines)
    return s[:m.start(1)] + body + s[m.end(1):]


def main():
    party = rd('src/data/trainers_frlg.party')
    if 'TRAINER_GANG_MAXIE' in party:
        sys.exit('villain gyms already applied')

    # trainer classes
    tr = rd('include/constants/trainers.h')
    tr = tr.replace('    TRAINER_CLASS_PAINTER_FRLG,\n', '    TRAINER_CLASS_PAINTER_FRLG,\n'
                    + ''.join('    TRAINER_CLASS_%s, // Gangster Outbreak\n' % c for c, _ in NEW_CLASSES), 1)
    wr('include/constants/trainers.h', tr)
    bm = rd('src/battle_main.c')
    bm = bm.replace('    [TRAINER_CLASS_PAINTER_FRLG] =         { _("PAINTER"), 4 },\n',
                    '    [TRAINER_CLASS_PAINTER_FRLG] =         { _("PAINTER"), 4 },\n'
                    + ''.join('    [TRAINER_CLASS_%s] = { _("%s"), 8 },\n' % (c, n) for c, n in NEW_CLASSES), 1)
    wr('src/battle_main.c', bm)

    # bosses
    for const, gym, old_names, name, cls, pic, gender, gfx, mons in BOSSES:
        m = party_block(party, const)
        party = party[:m.start()] + boss_entry(const, name, cls, pic, gender, mons) + '\n' + party[m.end():]
    party = party.rstrip('\n') + '\n\n' + boss_entry(*MAXIE[:5], MAXIE[5])
    opp = rd('include/constants/opponents_frlg.h')
    count = int(re.search(r'#define TRAINERS_COUNT_FRLG\s+(\d+)', opp).group(1))
    opp = opp.replace('#define TRAINERS_COUNT_FRLG                      %d' % count,
                      '#define TRAINER_GANG_MAXIE                         %d // Gangster Outbreak: Celadon gym partner\n\n'
                      '#define TRAINERS_COUNT_FRLG                      %d' % (count, count + 1))
    wr('include/constants/opponents_frlg.h', opp)

    # gym trainers
    for gym, (cls, pic, gfxs) in GRUNTS.items():
        sc = rd('data/maps/%s_Gym_Frlg/scripts.inc' % gym)
        consts = re.findall(r'trainerbattle_single (TRAINER_\w+),', sc)
        n = 0
        for c in consts:
            if c.startswith('TRAINER_LEADER'):
                continue
            m = party_block(party, c)
            block = m.group(0)
            klass = cls
            if cls == 'AQUA_MAGMA':
                klass = 'Team Aqua' if n % 2 == 0 else 'Team Magma'
                p = 'Aqua Grunt F' if n % 2 == 0 else 'Magma Grunt F'
            else:
                p = pic
            block = re.sub(r'^Class: .*$', 'Class: %s' % klass, block, flags=re.M)
            if p:
                block = re.sub(r'^Pic: .*$', 'Pic: %s' % p, block, flags=re.M)
            party = party[:m.start()] + block + party[m.end():]
            n += 1
        if gfxs:
            mj = 'data/maps/%s_Gym_Frlg/map.json' % gym
            m = json.loads(rd(mj))
            k = 0
            for o in m['object_events']:
                sc_name = o.get('script', '')
                if o.get('trainer_type', 'TRAINER_TYPE_NONE') != 'TRAINER_TYPE_NONE':
                    o['graphics_id'] = gfxs[k % len(gfxs)]
                    k += 1
            wr(mj, json.dumps(m, indent=2, ensure_ascii=False).replace(': []', ': [\n\n  ]') + '\n')
    wr('src/data/trainers_frlg.party', party)

    # leader overworld sprites, names and texts
    for const, gym, old_names, name, cls, pic, gender, gfx, mons in BOSSES:
        mj = 'data/maps/%s_Gym_Frlg/map.json' % gym
        m = json.loads(rd(mj))
        script_name = None
        sc = rd('data/maps/%s_Gym_Frlg/scripts.inc' % gym)
        leader_label = re.search(r'^(\w+)::\n(?:\t[^\n]*\n)*?\ttrainerbattle_single %s,' % const, sc, re.M).group(1)
        for o in m['object_events']:
            if o.get('script') == leader_label:
                if gfx:
                    o['graphics_id'] = gfx
                leader_obj = o
        if gym == 'CeladonCity':
            maxie = dict(leader_obj)
            maxie['graphics_id'] = 'OBJ_EVENT_GFX_MAXIE'
            maxie['x'] = leader_obj['x'] + 1
            maxie['script'] = leader_label
            m['object_events'].append(maxie)
        wr(mj, json.dumps(m, indent=2, ensure_ascii=False).replace(': []', ': [\n\n  ]') + '\n')

        def rename(mt):
            line = mt.group(0)
            for old in old_names:
                line = line.replace(old, name)
            return line
        sc = re.sub(r'\t\.string "[^\n]*"', rename, sc)
        intro = re.search(r'trainerbattle_single %s, (\w+), (\w+),' % const, sc)
        intro_label, defeat_label = intro.group(1), intro.group(2)
        sc = set_text(sc, intro_label, INTROS[gym][:-1] + [INTROS[gym][-1] + '{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$'])
        post = re.search(r'^(%s_Gym_Text_\w*PostBattle)::' % gym, sc, re.M)
        if gym in POSTS and post and not re.search(r'%s_Gym_Text_(Liam|Diana|Luis|Tucker|Baily|Dwayne|Kay|Bridget|Tina|Tamia|Lisa|Lori|Mary|Kayden|Kirk|Nate|Phil|Edgar|Shawn|Amanda|Johan|Stacy|Tyron|Tasha|Cameron|Preston|Erik|Quinn|Avery|Ramon|Derek|Dusty|Zac)PostBattle' % gym, post.group(1)):
            sc = set_text(sc, post.group(1), POSTS[gym][:-1] + [POSTS[gym][-1] + '$'])
        if gym == 'CeladonCity':
            old = '\ttrainerbattle_single %s, %s, %s, CeladonCity_Gym_EventScript_DefeatedErika, NO_MUSIC\n' % (const, intro_label, defeat_label)
            assert old in sc
            new = ('\tlock\n\tfaceplayer\n'
                   '\tgoto_if_set FLAG_DEFEATED_ERIKA, CeladonCity_Gym_EventScript_ArchieMaxieBeaten\n'
                   '\tmsgbox %s\n' % intro_label +
                   '\tspecial HasEnoughMonsForDoubleBattle\n'
                   '\tgoto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, CeladonCity_Gym_EventScript_ArchieMaxieNeedTwo\n'
                   '\ttrainerbattle_two_trainers %s, %s, TRAINER_GANG_MAXIE, CeladonCity_Gym_Text_MaxieDefeat\n' % (const, defeat_label) +
                   '\tgoto CeladonCity_Gym_EventScript_DefeatedErika\n'
                   'CeladonCity_Gym_EventScript_ArchieMaxieBeaten::\n')
            sc = sc.replace(old, new)
            sc = sc.replace('\tmsgbox CeladonCity_Gym_Text_ErikaIntro\n', '\tmsgbox CeladonCity_Gym_Text_ErikaIntro\n')
            sc += ('\nCeladonCity_Gym_EventScript_ArchieMaxieNeedTwo::\n\tmsgbox CeladonCity_Gym_Text_NeedTwo\n\trelease\n\tend\n'
                   '\nCeladonCity_Gym_Text_MaxieDefeat::\n' + ''.join('\t.string "%s"\n' % l for l in MAXIE_DEFEAT) +
                   '\nCeladonCity_Gym_Text_NeedTwo::\n' + ''.join('\t.string "%s"\n' % l for l in NEED_TWO))
            # the intro is shown by msgbox now: drop the battle music command from it
            sc = sc.replace('{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"\n\nCeladonCity_Gym_Text_ErikaDefeat', '$"\n\nCeladonCity_Gym_Text_ErikaDefeat')
        wr('data/maps/%s_Gym_Frlg/scripts.inc' % gym, sc)
        print('gym', gym, '->', name)


if __name__ == '__main__':
    main()
