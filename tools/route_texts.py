#!/usr/bin/env python3
"""Gangster Outbreak: gang-themed texts on the Kanto routes and in Viridian Forest.

Trainer texts (intro, defeat, after the battle) are built from templates per trainer class group (kids, toughs,
swimmers, nerds, posh people, hikers & bird keepers, psychics & channelers, cool trainers) and per region: every
route belongs to the gang of the nearest city (GANG_OF), so the trainers talk about that gang, its boss and town.
The choice of variant is fixed per trainer, so the texts stay the same on every run.
Signs and other people on the routes get hand-written texts (MANUAL). Functional texts (gifts, tutors, trades,
day care, gate guards, Bill, the Fishing Guru's brother, the Pokédex aides) are left alone.

Usage: python3 tools/route_texts.py
"""
import glob, hashlib, os, re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# gang, boss, town
ROCKET_V = ('TEAM ROCKET', 'GIOVANNI', 'VIRIDIAN')
YELL = ('TEAM YELL', 'PIERS', 'PEWTER')
SKULL = ('TEAM SKULL', 'GUZMA', 'CERULEAN')
ROCKET_A = ('TEAM ROCKET', 'ARCHER', 'VERMILION')
FLARE = ('TEAM FLARE', 'LYSANDRE', 'SAFFRON')
PLASMA = ('TEAM PLASMA', 'GHETSIS', 'FUCHSIA')
AQUA = ('TEAM AQUA', 'ARCHIE', 'CELADON')
MAGMA = ('TEAM MAGMA', 'MAXIE', 'CELADON')
GALACTIC = ('TEAM GALACTIC', 'CYRUS', 'CINNABAR')

GANG_OF = {
    'Route1': ROCKET_V, 'Route2': ROCKET_V, 'Route22': ROCKET_V, 'Route23': ROCKET_V, 'ViridianForest': ROCKET_V,
    'Route3': YELL, 'Route4': YELL,
    'Route24': SKULL, 'Route25': SKULL, 'Route9': SKULL, 'Route10': SKULL,
    'Route5': ROCKET_A, 'Route6': ROCKET_A, 'Route11': ROCKET_A, 'Route12': ROCKET_A,
    'Route7': FLARE, 'Route8': FLARE,
    'Route13': PLASMA, 'Route14': PLASMA, 'Route15': PLASMA, 'Route19': PLASMA,
    'Route16': AQUA, 'Route17': AQUA, 'Route18': MAGMA,
    'Route20': GALACTIC, 'Route21_North': GALACTIC, 'Route21_South': GALACTIC,
}

# '|' starts a new text box; {G} gang, {B} boss, {T} town
TEMPLATES = {
    'kid': {
        'intro': ["My big brother rides with {G}! I'm gonna be just like him!",
                  "{B} says kids can't fight. I'll prove him wrong!",
                  "Mom says stay inside.|But battles are way more fun!",
                  "I'm the toughest kid on this road. Ask anyone!",
                  "I found a loot bag yesterday!|Wanna fight for it?",
                  "My POKéMON is the scariest on the block!"],
        'defeat': ["Waaah! I'm telling my brother!", "No fair! You're way too strong!",
                   "Aww… There goes my lunch money.", "Okay, okay! You win!"],
        'post': ["When I grow up, I'm joining {G}.|Or maybe your crew!",
                 "Everybody's scared of {B}.|Are you scared?",
                 "I hide when the gangs come by.|Not from you, though!",
                 "The road to {T} isn't safe.|Watch out!",
                  "Don't tell my mom I battled you.",
                  "Can I join your crew when I'm older?"],
    },
    'tough': {
        'intro': ["This road belongs to {G}.|Pay up or fight!",
                  "{B} wants your money.|And I want a fight!",
                  "Wrong turf, rookie. Nobody passes without a fight!",
                  "You walk like you own the place.|Big mistake!",
                  "Hey! You're in the wrong part of town!",
                  "{B} said nobody passes. That means you!"],
        'defeat': ["Tch… You hit hard.", "Fine! Keep your cash!", "{B} is gonna kill me…", "Who are you, anyway?!"],
        'post': ["Word travels fast.|{B} will hear about you.",
                 "Next time I'll bring the whole of {G}!",
                 "Respect. Not many beat me on my own road.",
                 "Go on. This road's yours…|for now.",
                  "You're making a name for yourself, boss.",
                  "Tell your crew I fought fair. Mostly."],
    },
    'water': {
        'intro': ["{G} took the docks, so I swim my own routes!",
                  "The sea doesn't care about gangs.|But I do!",
                  "You'll sink like the last punk who came here!",
                  "Out here, only the strong stay afloat!",
                  "Splash! Nobody swims past me!",
                  "I patrol these waves for {G}!"],
        'defeat': ["Glub… I'm all washed up.", "Sunk! Totally sunk!", "The tide turned on me…",
                   "Gah! Swallowed water!"],
        'post': ["Even {G} can't control the sea.",
                 "Smugglers use these waters at night. Stay sharp.",
                 "Swim fast if you see a boat from {G}.",
                 "The water's the only free place left.",
                  "The currents carry loot bags. Keep your eyes open.",
                  "Ships from {T} pass here every night."],
    },
    'nerd': {
        'intro': ["I hacked a PC of {G} once.|Now I hide out here!",
                  "My calculations say you lose.|Let's test it!",
                  "I sell info to every gang.|Info on you is worth a lot!",
                  "Science doesn't stop for a gang war!",
                  "I built a radar for gang movements. It beeped at you!",
                  "Your POKéMON stats look… interesting!"],
        'defeat': ["Error! That wasn't in my data!", "My theory… failed!",
                   "Impossible! I calculated everything!", "Fine… I'll recalculate."],
        'post': ["{B} pays well for smart people.|Just saying.",
                 "I'll sell your battle data to {G}.|Kidding! Mostly.",
                 "Data says you'll beat {B}.|Probably.",
                 "Knowledge is the only thing they can't steal.",
                  "Every gang uses my gadgets. Nobody pays.",
                  "I'm writing a paper on gang warfare. You're in it now."],
    },
    'posh': {
        'intro': ["Even in a gang war, one must look fabulous.",
                  "{B} invited me to his parties.|Jealous?",
                  "Ugh, a street kid.|Let's get this over with.",
                  "Style is a weapon too, darling!",
                  "Darling, this road is far too dirty for you.",
                  "I only battle people with style. You'll do."],
        'defeat': ["My outfit! It's ruined!", "How… vulgar of you to win.", "Oh my. You're rather strong.",
                   "I broke a nail! Unforgivable!"],
        'post': ["Money buys safety from {G}.|Usually.",
                 "You fight like a true boss.|How charming.",
                 "Do tell {B} I said hello.",
                 "Dirty roads, dirty fights.|Such is life.",
                  "The parties in {T} are wild these days.",
                  "One day this mess will end. I'll be fabulous then, too."],
    },
    'outdoor': {
        'intro': ["I live out here, far from {G}.|Leave me be! …Or battle!",
                  "My birds see every gang that moves.|And they saw you!",
                  "These hills are older than any gang.|So am I!",
                  "Out here, nobody pays the tolls of {B}!",
                  "Gangs come and go. The mountains stay!",
                  "My POKéMON is as tough as the rocks here!"],
        'defeat': ["Whoa! Rolled right over me!", "My birds flew off…", "Like a rockslide!",
                   "Heh, you're tough as stone."],
        'post': ["From up here I watch {G} come and go.",
                 "The wild POKéMON are the only honest ones left.",
                 "Head to {T} if you want trouble.",
                 "Rest a bit. The road ahead is rough.",
                  "Gangs never climb this far. Too lazy.",
                  "Watch the skies. Even the birds avoid {G}."],
    },
    'mystic': {
        'intro': ["I sense… a boss in the making.",
                  "The spirits whisper of {B}.|They are afraid.",
                  "Your future is dark.|Let me show you!",
                  "The outbreak woke the ghosts.|And me!",
                  "The ghosts told me you'd come.",
                  "I feel a strange power around you…"],
        'defeat': ["The visions… were wrong!", "My mind… is spinning!", "The spirits abandoned me…",
                   "I did not foresee this!"],
        'post': ["I see you facing {B}.|And winning.",
                 "Even ghosts avoid {G}.",
                 "Your aura is strong.|Like a true boss.",
                 "Beware the road to {T}.",
                  "The spirits say {B} fears you already.",
                  "Leave this place before night falls."],
    },
    'cool': {
        'intro': ["I trained through the whole outbreak.|Show me what you've got!",
                  "{G} tried to recruit me. I said no.|Wanna know why?",
                  "Only real TRAINERS survive out here!",
                  "A boss with a crew like yours?|Let's see if you're for real.",
                  "I don't care about gangs. Only about strong opponents!",
                  "Let's see what a real boss can do!"],
        'defeat': ["Impressive. Truly.", "You're the real deal.", "I underestimated you.",
                   "That was a great battle!"],
        'post': ["Take down {B}.|Somebody has to.",
                 "With skills like that, every gang will want you.",
                 "Keep training.|The bosses only get stronger.",
                 "KANTO needs someone like you.",
                  "One day, I'll challenge {B} too.",
                  "Strong TRAINERS are rare these days. Respect."],
    },
}

GROUPS = [  # first match on the trainer class wins
    ('water', ('swimmer', 'fisherman', 'sailor', 'tuber')),
    ('kid', ('youngster', 'bug catcher', 'lass', 'camper', 'picnicker', 'twins', 'sis and bro', 'young couple')),
    ('nerd', ('super nerd', 'gamer', 'engineer', 'scientist', 'juggler', 'maniac')),
    ('posh', ('beauty', 'gentleman', 'lady', 'painter', 'aroma')),
    ('outdoor', ('hiker', 'bird keeper', 'ruin')),
    ('mystic', ('psychic', 'channeler')),
    ('cool', ('cool', 'ace')),
    ('tough', ('',)),
]

# hand-written texts for signs and people: (map folder, label suffix) -> text ('|' = new box)
MANUAL = {
    ('Route1', 'CanJumpFromLedges'): "See those ledges? Jump down them and you're back in PALLET in no time.|Handy when SILVER's crew is after you.",
    ('Route1', 'WorkAtPokeMartTakeSample'): "Hi! The MARTS are all vending machines now.|I hand out free samples so people keep coming. Here!",
    ('Route1', 'ComeSeeUsIfYouNeedPokeBalls'): "The machine in VIRIDIAN sells POKé BALLS. If TEAM ROCKET left any.",
    ('Route2_House', 'FaintedMonsCanUseFieldMoves'): "A fainted POKéMON can't fight, but it can still use field moves.|Good thing, since the gangs fight dirty.",
    ('Route2_ViridianForest_SouthEntrance', 'ForestIsMaze'): "Going into VIRIDIAN FOREST?|TEAM ROCKET hides stolen goods in there. Don't get lost!",
    ('Route2_ViridianForest_SouthEntrance', 'RattataHasWickedBite'): "RATTATA may be small, but they bite like a gangster.|Have you caught one already?",
    ('Route2_ViridianForest_NorthEntrance', 'ManyMonsOnlyInForests'): "Many POKéMON hide in forests and caves since the outbreak.|Look everywhere to find them all.",
    ('Route3', 'TunnelFromCeruleanTiring'): "Whew… I better take a rest…|I ran all the way from TEAM SKULL in CERULEAN!",
    ('Route3', 'RouteSign'): "ROUTE 3|MT. MOON AHEAD - TEAM YELL TURF",
    ('Route4', 'TrippedOverGeodude'): "Ouch! I tripped over a GEODUDE!|TEAM YELL put it there as a trap, I bet!",
    ('Route4', 'PeopleLikeAndRespectBrock'): "Whoa, a real gang BADGE! You took it from PIERS?|Even TEAM YELL respects that. Keep it safe!",
    ('Route4_PokemonCenter_1F', 'TeamRocketAttacksCerulean'): "TEAM SKULL took over CERULEAN…|Not a day goes by without the gangs in the news.",
    ('Route4_PokemonCenter_1F', 'ItsANewspaper'): "It's a newspaper.|“GANG WAR SPREADS ACROSS KANTO.”",
    ('Route10_PokemonCenter_1F', 'NuggetUselessSoldFor5000'): "I found a NUGGET in a loot bag.|Sold it and bought REVIVES. Smart, right?",
    ('Route10_PokemonCenter_1F', 'HeardGhostsHauntLavender'): "They say ghosts haunt LAVENDER TOWN.|Even the gangs stay away from it.",
    ('Route11_EastEntrance_1F', 'RockTunnelToReachLavender'): "If you want to reach LAVENDER TOWN, take ROCK TUNNEL.|It starts north of CERULEAN, in TEAM SKULL land.",
    ('Route12', 'RouteSign'): "ROUTE 12|North to LAVENDER - ROCKET CHECKPOINT",
    ('Route12', 'SportfishingArea'): "SPORTFISHING AREA|Closed by order of TEAM ROCKET.",
    ('Route12_NorthEntrance_1F', 'LookoutSpotUpstairs'): "There's a lookout spot upstairs.|The ROCKETS use it to watch the road.",
    ('Route13', 'LookToLeftOfThatPost'): "TRAINER TIPS|Look to the left of that post!|…TEAM PLASMA hid something there.",
    ('Route13', 'RouteSign'): "ROUTE 13|North to SILENCE BRIDGE",
    ('Route14', 'RouteSign'): "ROUTE 14|West to FUCHSIA CITY - PLASMA TERRITORY",
    ('Route15', 'RouteSign'): "ROUTE 15|West to FUCHSIA CITY - PLASMA TERRITORY",
    ('Route15_WestEntrance_1F', 'OaksAideCameByHere'): "Are you the kid who's building a crew?|PROF. OAK's AIDE came through here, hiding from TEAM PLASMA.",
    ('Route16', 'CyclingRoadSign'): "Enjoy the slope!|CYCLING ROAD - TEAM AQUA TOLL",
    ('Route16', 'RouteSign'): "ROUTE 16|CELADON CITY - FUCHSIA CITY",
    ('Route16_NorthEntrance_1F', 'CyclingRoadIsDownhillCourse'): "CYCLING ROAD is a downhill course by the sea.|TEAM AQUA races down there every night.",
    ('Route16_NorthEntrance_2F', 'OnBikeRideWithGirlfriend'): "I'm riding with my girlfriend.|If we ride fast, the gangs can't catch us!",
    ('Route16_NorthEntrance_2F', 'RidingTogetherOnNewBikes'): "We stole these bikes back from TEAM SKULL!",
    ('Route17', 'WatchOutForDiscardedItems'): "It's a notice.|Watch out for loot bags. And for whoever dropped them.",
    ('Route17', 'DontThrowGameThrowBalls'): "It's a notice!|Don't throw the game, throw POKé BALLS instead!|- TEAM AQUA",
    ('Route18', 'CyclingRoadSign'): "CYCLING ROAD|No pedestrians permitted! - TEAM MAGMA",
    ('Route18_EastEntrance_1F', 'CyclingRoadAllUphillFromHere'): "CYCLING ROAD is all uphill from here.|TEAM MAGMA watches the top.",
    ('Route19', 'RouteSign'): "SEA ROUTE 19|FUCHSIA CITY - SEAFOAM ISLANDS",
    ('Route20', 'MistyTrainsHere'): "Strong TRAINERS and WATER POKéMON are common here.|They say TEAM GALACTIC ships pass by at night.",
    ('Route22', 'LeagueGateSign'): "POKéMON LEAGUE|Front Gate - BOSSES ONLY",
    ('Route23', 'VictoryRoadGateSign'): "VICTORY ROAD GATE -|POKéMON LEAGUE",
    ('Route24', 'JoinTeamRocket'): "By the way, how would you like to join TEAM ROCKET?|Oh wait, you're already in GIOVANNI's crew.|Then how about switching to ARCHER's side?|Are you sure?|Fine. Then I'll take you down!",
    ('Route24', 'YoudBecomeTopRocketLeader'): "With your ability, you'd be a top boss anywhere.|Come on, think about it!",
    ('Route25', 'MistyHighHopesAboutThisPlace'): "This cape used to be a famous date spot.|Now TEAM SKULL throws parties here.",
    ('Route25', 'AreYouHereAlone'): "Are you here alone?|Out on CERULEAN's cape, you'd better bring your crew.",
    ('ViridianForest', 'FriendsItchingToBattle'): "I came here with friends to catch BUG POKéMON!|We're hiding from TEAM ROCKET. And itching to battle!",
    ('ViridianForest', 'RanOutOfPokeBalls'): "I ran out of POKé BALLS.|A ROCKET took the rest. Never carry too few!",
    ('ViridianForest', 'LeavingViridianForest'): "LEAVING VIRIDIAN FOREST|PEWTER CITY AHEAD - TEAM YELL TURF",
    ('Route5_SouthEntrance', 'ThirstyOnGuardDuty'): "I guard this gate for TEAM FLARE.|Gee, I'm thirsty, though!|Oh, wait there, the road's closed.",
    ('Route6_NorthEntrance', 'ThirstyOnGuardDuty'): "I guard this gate for TEAM FLARE.|Gee, I'm thirsty, though!|Oh, wait there, the road's closed.",
    ('Route7_EastEntrance', 'ThirstyOnGuardDuty'): "I guard this gate for TEAM FLARE.|Gee, I'm thirsty, though!|Oh, wait there, the road's closed.",
    ('Route8_WestEntrance', 'ThirstyOnGuardDuty'): "I guard this gate for TEAM FLARE.|Gee, I'm thirsty, though!|Oh, wait there, the road's closed.",
}

MAX_WIDTH = 180   # pixels per line (the message box fits about 200)


def font():
    src = open(os.path.join(ROOT, 'src/fonts.c')).read()
    m = re.search(r'gFontNormalLatinGlyphWidths\[\] = \{(.*?)\};', src, re.S)
    widths = [int(x) for x in re.findall(r'\d+', re.sub(r'//.*', '', m.group(1)))]
    cmap = {}
    for line in open(os.path.join(ROOT, 'charmap.txt'), encoding='utf-8'):
        mm = re.match(r"^'(.)'\s*=\s*([0-9A-F]{2})\s*$", line.strip())
        if mm:
            cmap[mm.group(1)] = int(mm.group(2), 16)
    return lambda t: sum(widths[cmap[c]] if c in cmap and cmap[c] < len(widths) else 6 for c in t)


WIDTH = font()


def wrap(text):
    """text with '|' box breaks -> .string lines (2 lines per box)"""
    lines_out = []
    for box in text.split('|'):
        lines, cur = [], ''
        for word in box.split():
            cand = (cur + ' ' + word).strip()
            if cur and WIDTH(cand) > MAX_WIDTH:
                lines.append(cur)
                cur = word
            else:
                cur = cand
        lines.append(cur)
        for i in range(0, len(lines), 2):
            lines_out.append(lines[i:i + 2])
    out = []
    for bi, pair in enumerate(lines_out):
        last_box = bi == len(lines_out) - 1
        if len(pair) == 2:
            out += [pair[0] + '\\n', pair[1] + ('$' if last_box else '\\p')]
        else:
            out.append(pair[0] + ('$' if last_box else '\\p'))
    return out


def replace(s, label, lines):
    m = re.search(r'(^%s::?\n)((?:\t\.string .*\n)+)' % re.escape(label), s, re.M)
    if not m:
        return s, False
    return s[:m.start(2)] + ''.join('\t.string "%s"\n' % l for l in lines) + s[m.end(2):], True


def trainer_classes():
    party = open(os.path.join(ROOT, 'src/data/trainers_frlg.party')).read()
    return {m.group(1): m.group(2).lower() for m in re.finditer(r'^=== (TRAINER_\w+) ===\n(?:.*\n)*?Class: (.*)$', party, re.M)}


def group_of(cls):
    for name, keys in GROUPS:
        if any(k in cls for k in keys):
            return name
    return 'tough'


def battles():
    """(trainer, intro label, defeat label) of every trainer battle, from the shared trainer scripts and the maps"""
    srcs = [os.path.join(ROOT, 'data/scripts/trainers_frlg.inc')] + glob.glob(os.path.join(ROOT, 'data/maps/*/scripts.inc'))
    out = []
    for f in srcs:
        out += re.findall(r'trainerbattle_\w+ (TRAINER_\w+), (\w+), (\w+)', open(f).read())
    return out


def main():
    classes = trainer_classes()
    all_battles = battles()
    total = 0
    files = sorted(glob.glob(os.path.join(ROOT, 'data/maps/Route*_Frlg/scripts.inc')) +
                   [os.path.join(ROOT, 'data/maps/ViridianForest_Frlg/scripts.inc')])
    for path in files:
        mapname = path.split('/')[-2].replace('_Frlg', '')
        s = open(path).read()
        changed = 0
        gang = GANG_OF.get(mapname)
        if gang:
            used = {}
            prefix = mapname + '_Text_'
            for trainer, intro, defeat in all_battles:
                if not intro.startswith(prefix) or 'Rematch' in intro or not intro.endswith('Intro'):
                    continue
                if 'RIVAL' in trainer:
                    continue
                grp = group_of(classes.get(trainer, ''))
                base = intro[:-len('Intro')]
                post = base + 'PostBattle' if re.search(r'^%sPostBattle:' % re.escape(base), s, re.M) else base + 'Post'
                h = int(hashlib.md5((mapname + grp).encode()).hexdigest(), 16)   # one rotation per route and group
                n = used.get(grp, 0)
                used[grp] = n + 1
                g, b, t = gang
                for kind, label, shift in (('intro', intro, 0), ('defeat', defeat, 0), ('post', post, 0),
                                           ('intro', base + 'RematchIntro', 1)):
                    pool = TEMPLATES[grp][kind]
                    text = pool[(h + n + shift) % len(pool)].format(G=g, B=b, T=t)
                    s, ok = replace(s, label, wrap(text))
                    changed += ok
        for (mp, suffix), text in MANUAL.items():
            if mp == mapname:
                s, ok = replace(s, '%s_Text_%s' % (mapname, suffix), wrap(text))
                assert ok, (mp, suffix)
                changed += ok
        if changed:
            open(path, 'w').write(s)
            total += changed
    print('%d texts written' % total)


if __name__ == '__main__':
    main()
