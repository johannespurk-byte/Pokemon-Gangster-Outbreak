#!/usr/bin/env python3
"""Gangster Outbreak: gang-themed texts on routes, in towns, caves, dungeons and on the Sevii Islands.

Trainer texts (intro, defeat, after the battle) are built from templates per trainer class group (kids, toughs,
swimmers, nerds, posh people, hikers & bird keepers, psychics & channelers, cool trainers) and per region: every
route, cave and dungeon belongs to the gang of the nearest city (GANG_OF; gyms and the league keep their own), so the trainers talk about that gang, its boss and town.
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
SEVII = ('TEAM ROCKET', 'ARCHER', 'the SEVII ISLANDS')   # the ROCKET remnants hide out on the islands

GANG_OF = {
    'Route1': ROCKET_V, 'Route2': ROCKET_V, 'Route22': ROCKET_V, 'Route23': ROCKET_V, 'ViridianForest': ROCKET_V,
    'Route3': YELL, 'Route4': YELL,
    'Route24': SKULL, 'Route25': SKULL, 'Route9': SKULL, 'Route10': SKULL,
    'Route5': ROCKET_A, 'Route6': ROCKET_A, 'Route11': ROCKET_A, 'Route12': ROCKET_A,
    'Route7': FLARE, 'Route8': FLARE,
    'Route13': PLASMA, 'Route14': PLASMA, 'Route15': PLASMA, 'Route19': PLASMA,
    'Route16': AQUA, 'Route17': AQUA, 'Route18': MAGMA,
    'Route20': GALACTIC, 'Route21_North': GALACTIC, 'Route21_South': GALACTIC,
    # caves, dungeons and other places with trainers
    'MtMoon': YELL, 'RockTunnel': SKULL, 'PowerPlant': SKULL, 'CeruleanCave': SKULL, 'PokemonTower': ROCKET_V,
    'SSAnne': ROCKET_A, 'DiglettsCave': ROCKET_A, 'RocketHideout': ROCKET_V, 'SilphCo': ROCKET_V,
    'UndergroundPath': FLARE, 'SeafoamIslands': PLASMA, 'PokemonMansion': GALACTIC, 'VictoryRoad': ROCKET_V,
    'OneIsland': SEVII, 'TwoIsland': SEVII, 'ThreeIsland': SEVII, 'FourIsland': SEVII, 'FiveIsland': SEVII,
    'SixIsland': SEVII, 'SevenIsland': SEVII, 'MtEmber': SEVII,
}
# GYMS: every trainer inside talks about the boss of the GYM ('gym' templates)
GYM_GANG = {
    'PewterCity_Gym': YELL, 'CeruleanCity_Gym': SKULL, 'VermilionCity_Gym': ROCKET_A,
    'CeladonCity_Gym': ('TEAM AQUA and TEAM MAGMA', 'ARCHIE', 'CELADON'),
    'FuchsiaCity_Gym': PLASMA, 'SaffronCity_Gym': FLARE, 'CinnabarIsland_Gym': GALACTIC, 'ViridianCity_Gym': ROCKET_V,
}
SKIP_CLASSES = ('leader', 'elite', 'champion', 'rival', 'boss', 'admin', 'prof', 'pkmn trainer')

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
    'grunt': {
        'intro': ["Halt! This is {G} business!", "{B} said nobody gets through. Nobody!",
                  "You're not one of us. Get lost or get hurt!", "Another crew poking around here? Big mistake!",
                  "I guard this place for {B}. Prepare yourself!", "Hand over your POKéMON. Orders from {B}!"],
        'defeat': ["Urgh! Not again!", "I'm gonna be in so much trouble…", "Retreat! Retreat!",
                   "How can a kid be this strong?!"],
        'post': ["{B} will deal with you himself.", "Fine, go ahead. You'll see what's waiting for you.",
                 "I only joined {G} for the money…", "You'd make a great grunt. Ever thought about it?",
                 "Don't tell {B} I lost to you.", "Word is you're GIOVANNI's new star. Is that true?"],
    },
    'gym': {
        'intro': ["Nobody reaches {B} without going through me!", "This is the GYM of {G}. Turn back while you can!",
                  "You want a RANK? Earn it the hard way!", "{B} doesn't waste time on rookies. Neither do I!",
                  "Another challenger? {B} will love this.", "Welcome to our turf. Hope you brought REVIVES!"],
        'defeat': ["Tch… You're the real thing.", "No way! {B} is gonna be mad!", "I lost?! In my own GYM?!",
                   "Fine… go on ahead."],
        'post': ["{B} is waiting. Don't keep the boss waiting.", "Beat {B} and the whole of {G} will respect you.",
                 "I trained for months under {B}. You still beat me.", "Go on. Show {B} what you've got.",
                 "Every gang in KANTO is watching this GYM.", "If you win, you're one step closer to the LEAGUE."],
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
    ('grunt', ('team ',)),
    ('water', ('swimmer', 'fisherman', 'sailor', 'tuber')),
    ('kid', ('youngster', 'bug catcher', 'lass', 'camper', 'picnicker', 'twins', 'sis and bro', 'young couple')),
    ('nerd', ('super nerd', 'gamer', 'engineer', 'scientist', 'juggler', 'maniac')),
    ('posh', ('beauty', 'gentleman', 'lady', 'painter', 'aroma', 'breeder', 'couple')),
    ('outdoor', ('hiker', 'bird keeper', 'ruin', 'ranger')),
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

# towns: gym signs, city signs and people in towns, POKéMON CENTERS and MARTS
MANUAL.update({
    ('ViridianCity', 'GymSign'): "VIRIDIAN CITY POKéMON GYM|BOSS: GIOVANNI - TEAM ROCKET HQ",
    ('ViridianCity', 'CitySign'): "VIRIDIAN CITY|GIOVANNI's City. Keep Walking.",
    ('ViridianCity', 'GymClosedWonderWhoLeaderIs'): "The GYM is TEAM ROCKET's HQ now.|Only GIOVANNI's people go in there.",
    ('ViridianCity', 'GoShoppingInPewterOccasionally'): "I go shopping in PEWTER sometimes.|TEAM YELL makes noise all night, but at least they don't steal.",
    ('ViridianCity', 'CatchMonsForEasierBattles'): "TRAINER TIPS|Catch POKéMON and build your crew. The bigger it is, the safer you are.",
    ('ViridianCity_Mart', 'ShopDoesGoodBusinessInAntidotes'): "This machine sells a lot of ANTIDOTES. The gangs love poison POKéMON.",
    ('ViridianCity_Mart', 'GotToBuySomePotions'): "I've got to buy POTIONS.|You never know when the next gang fight starts.",
    ('ViridianCity_PokemonCenter_1F', 'PokeCenterInEveryTown'): "There's a HEAL-BOT in every town ahead.|They're free, so heal up before every fight.",
    ('ViridianCity_PokemonCenter_1F', 'PokeCentersHealMons'): "The nurses fled, but the HEAL-BOTS stayed.|They fix any POKéMON. And they never ask questions.",
    ('PewterCity', 'GymSign'): "PEWTER CITY POKéMON GYM|BOSS: PIERS - TEAM YELL",
    ('PewterCity', 'CitySign'): "PEWTER CITY|A Stone Gray City. Loud All Night.",
    ('PewterCity', 'BrockOnlySeriousTrainerHere'): "TEAM YELL took over the GYM.|PIERS sings on the roof every night. Everybody has to cheer!",
    ('PewterCity', 'BrocksLookingForChallengersFollowMe'): "You're a TRAINER, right?|PIERS is looking for new challengers. Follow me!",
    ('PewterCity', 'GoTakeOnBrock'): "If you've got the guts, go take on PIERS!",
    ('PewterCity', 'CallPoliceIfInfoOnThieves'): "NOTICE!|Thieves have been stealing fossils from MT. MOON.|The PEWTER POLICE are gone. Good luck.",
    ('PewterCity', 'WerentThoseFossilsAmazing'): "The fossils in the MUSEUM are fake now. The real ones went to the black market.",
    ('PewterCity', 'ThisIsTheMuseum'): "This is the MUSEUM.|TEAM YELL wants money at the door, but it's worth it. See you!",
    ('PewterCity', 'SprayingRepelToKeepWildMonsOut'): "I'm spraying REPEL to keep wild POKéMON out of my garden. And TEAM YELL too!",
    ('PewterCity_Mart', 'GoodThingsIfRaiseMonsDiligently'): "Good things happen if you raise POKéMON well.|Even the weak ones can scare a gang if you train them.",
    ('PewterCity_PokemonCenter_1F', 'TeamRocketMtMoonImOnPhone'): "What!?|TEAM YELL is partying in MT. MOON again?|I'm on the phone! Scram!",
    ('CeruleanCity', 'GymSign'): "CERULEAN CITY POKéMON GYM|BOSS: GUZMA - TEAM SKULL",
    ('CeruleanCity', 'CitySign'): "CERULEAN CITY|A Mysterious Blue Aura… and TEAM SKULL",
    ('CeruleanCity', 'TrainerLifeIsToughIsntIt'): "You're a TRAINER, too?|Since TEAM SKULL came, it's a tough life, isn't it?",
    ('CeruleanCity', 'PeopleHereWereRobbed'): "The people here were robbed.|The ROCKETS broke in, and TEAM SKULL took the rest!|Even the POLICE gave up.",
    ('CeruleanCity', 'WantBrightRedBicycle'): "I want a bright red bicycle.|But TEAM SKULL would just steal it.",
    ('CeruleanCity', 'HardToControlMonsObedience'): "No! That's wrong!|SLOWBRO only listens to tough bosses. I guess I'm not one…",
    ('CeruleanCity_Mart', 'DoYouKnowAboutRareCandy'): "Do you know about RARE CANDY?|Loot bags have them sometimes. They make POKéMON grow fast!",
    ('CeruleanCity_PokemonCenter_1F', 'BillDoesWhateverForRareMons'): "That BILL!|He hides from the gangs in his cottage north of town. Smart guy.",
    ('CeruleanCity_PokemonCenter_1F', 'BillCollectsRareMons'): "BILL has lots of rare POKéMON. TEAM SKULL would love to steal them!",
    ('VermilionCity', 'GymSign'): "VERMILION CITY POKéMON GYM|BOSS: ARCHER - TEAM ROCKET",
    ('VermilionCity', 'CitySign'): "VERMILION CITY|ROCKET HARBOR",
    ('VermilionCity', 'GrimerMultipliesInSludge'): "ARCHER's ROCKETS dump sludge in the harbor.|They say GRIMER multiply in it.",
    ('VermilionCity', 'BuildingOnThisLand'): "I'm putting up a building on this plot.|ARCHER takes a cut, of course. Everybody pays him.",
    ('VermilionCity', 'SSAnneVisitsOnceAYear'): "The S.S. ANNE is a famous cruise ship.|The ROCKETS use it to smuggle POKéMON, they say.",
    ('VermilionCity', 'PokemonFanClubSign'): "POKéMON FAN CLUB|No gang colors inside!",
    ('VermilionCity_Mart', 'TeamRocketAreWickedPeople'): "TEAM ROCKET runs the harbor.|They ship rare POKéMON out every night. Don't ask me how I know.",
    ('VermilionCity_Mart', 'MonsGoodOrBadDependingOnTrainer'): "POKéMON aren't good or bad. It depends on the gang they run with.",
    ('VermilionCity_PokemonCenter_1F', 'PoisonedMonFaintedWhileWalking'): "My POKéMON got poisoned in a street fight!|It fainted on the way here!",
    ('LavenderTown', 'TownSign'): "LAVENDER TOWN|Too Quiet for the Gangs",
    ('LavenderTown', 'TownKnownAsMonGraveSite'): "This town is the grave site of POKéMON.|Even the gangs leave it alone. Mostly.",
    ('LavenderTown_Mart', 'DidYouBuyRevives'): "Did you buy REVIVES?|Out here, you'll need every single one.",
    ('CeladonCity', 'GymSign'): "CELADON CITY POKéMON GYM|BOSSES: ARCHIE and MAXIE - AQUA and MAGMA",
    ('CeladonCity', 'CitySign'): "CELADON CITY|Half Blue, Half Red. Pick a Side.",
    ('CeladonCity', 'GymIsGreatFullOfWomen'): "TEAM AQUA and TEAM MAGMA share the GYM.|They fight about it every single day!",
    ('CeladonCity', 'GameCornerIsBadForCitysImage'): "CELADON used to be beautiful.|Now it's AQUA on one side, MAGMA on the other, and ROCKET slots in the middle.",
    ('CeladonCity', 'KeepOutOfTeamRocketsWay'): "Keep out of everybody's way here. AQUA, MAGMA, ROCKET… pick one and pray.",
    ('CeladonCity', 'GetLostOrIllPunchYou'): "What are you staring at?|I'm MAGMA. Get lost, or I'll punch you.",
    ('CeladonCity', 'DeptStoreSign'): "Find what's left at the CELADON DEPT. STORE!",
    ('CeladonCity', 'GameCornerSign'): "ROCKET GAME CORNER|Everybody Loses. We Win.",
    ('CeladonCity_PokemonCenter_1F', 'RodeHereFromFuchsia'): "I rode up CYCLING ROAD from FUCHSIA.|TEAM AQUA chased me the whole way!",
    ('FuchsiaCity', 'GymSign'): "FUCHSIA CITY POKéMON GYM|BOSS: GHETSIS - TEAM PLASMA",
    ('FuchsiaCity', 'CitySign'): "FUCHSIA CITY|Behold! It's TEAM PLASMA's City!",
    ('FuchsiaCity', 'DidYouTrySafariGame'): "Did you try the SAFARI GAME?|Go before TEAM PLASMA “frees” all the POKéMON.",
    ('FuchsiaCity', 'SafariZoneSign'): "POKéMON PARADISE|SAFARI ZONE - STILL OPEN",
    ('FuchsiaCity', 'MyFatherIsGymLeader'): "My father ran this GYM before TEAM PLASMA came.|Now GHETSIS sits on his chair. I'll get it back one day!",
    ('FuchsiaCity_Mart', 'DidYouTryXSpeed'): "Did you try X SPEED?|Good for running away from TEAM PLASMA, too.",
    ('FuchsiaCity_PokemonCenter_1F', 'PokemonLeagueWestOfViridian'): "There's a narrow trail west of VIRIDIAN.|It leads to the POKéMON LEAGUE. Only the strongest boss can reach it.",
    ('SaffronCity', 'GymSign'): "SAFFRON CITY POKéMON GYM|BOSS: LYSANDRE - TEAM FLARE",
    ('SaffronCity', 'CitySign'): "SAFFRON CITY|TEAM FLARE's Shining Golden City",
    ('SaffronCity', 'BossTakeTownForTeamRocket'): "BOSS says SILPH belongs to TEAM ROCKET!|TEAM FLARE can keep the rest of the city.",
    ('SaffronCity', 'SaffronBelongsToTeamRocket'): "SILPH CO. belongs to TEAM ROCKET!|Tell those FLARE snobs!",
    ('SaffronCity', 'PeopleComingBackToSaffron'): "People fled when the ROCKETS came.|Now only TEAM FLARE's fashion is left to fear.",
    ('SaffronCity', 'HowCanClubNotRecognizeLance'): "This FAN CLUB…|They all love PIERS and LYSANDRE. Nobody has a clue!",
    ('SaffronCity_PokemonCenter_1F', 'GreatIfEliteFourCameBeatRockets'): "Sigh…|Wouldn't it be great if someone stomped every gang in KANTO?",
    ('SaffronCity_PokemonCenter_1F', 'SilphCoVictimOfFame'): "SILPH CO. is famous everywhere.|That's why every gang wants it.",
    ('CinnabarIsland', 'GymSign'): "CINNABAR ISLAND POKéMON GYM|BOSS: CYRUS - TEAM GALACTIC",
    ('CinnabarIsland', 'IslandSign'): "CINNABAR ISLAND|TEAM GALACTIC's Island of Science",
    ('CinnabarIsland', 'BlaineLivedHereSinceBeforeLab'): "TEAM GALACTIC took the GYM and the LAB.|CYRUS wants to build a new world from here.",
    ('CinnabarIsland', 'ScientistsExperimentInMansion'): "GALACTIC scientists run experiments in the burned-out MANSION.|Nobody who goes in comes out the same.",
    ('CinnabarIsland_Mart', 'ExtraItemsNeverHurt'): "Extra items never hurt.|Especially on an island full of GALACTIC grunts.",
})


# SEVII ISLANDS: the last places the gangs haven't taken (only TEAM ROCKET remnants and the bikers on THREE ISLAND)
MANUAL.update({
    ('OneIsland', 'IslandSign'): "ONE ISLAND|Friends Gather Here. Gangs Don't.",
    ('OneIsland', 'IsntWarmClimateHereGreat'): "Sight-seeing, are you?|The islands are the last place the gangs haven't taken. Yet.",
    ('TwoIsland', 'IslandSign'): "TWO ISLAND|A Quiet Island. For Now.",
    ('ThreeIsland', 'IslandSign'): "THREE ISLAND|Kin Island. Watch Out for Bikers.",
    ('FourIsland', 'IslandSign'): "FOUR ISLAND|The Warm, Blue, Floe Island",
    ('FiveIsland', 'IslandSign'): "FIVE ISLAND|Chrono Island. Strange Ships at Night.",
    ('FiveIsland', 'BeenGettingBusierAroundHere'): "It's been getting busier around here.|Strange ships land at night. ROCKET ships, if you ask me.",
    ('SixIsland', 'IslandSign'): "SIX ISLAND|Old and Wise. Nobody Bothers Us.",
    ('SixIsland', 'SkyAtNightIsFantastic'): "Far from the burning cities, the sky at night is fantastic.",
    ('SevenIsland', 'IslandSign'): "SEVEN ISLAND|The Last Island Before the End of the World",
    ('SevenIsland', 'IslandVirtuallyUntouched'): "This island is untouched. No gangs, no fires.|Let's keep it that way.",
})


# the rest: CELADON MANSION, SILPH CO., S.S. ANNE, POKéMON TOWER, SAFARI ZONE, paths and caves, SEVII houses
MANUAL.update({
    ('CeladonCity_Condominiums_1F', 'NothingBeatsThirstLikeTea'): "Nothing beats a hot TEA.|Even AQUA and MAGMA stop fighting for my TEA.",
    ('CeladonCity_Condominiums_1F', 'MyDearMonsKeepMeCompany'): "My POKéMON keep me company.|MEOWTH even brings money home. From the gangs, I think.",
    ('CeladonCity_Condominiums_3F', 'ImTheProgrammer'): "Me?|I'm the programmer! I make sure the gangs never crash the game.",
    ('CeladonCity_Condominiums_3F', 'ImTheGraphicArtist'): "I'm the graphic artist!|I drew the burning cities. Pretty, huh?",
    ('CeladonCity_Condominiums_3F', 'IWroteTheStory'): "I wrote the story!|Isn't GIOVANNI cool? And PIERS! And LYSANDRE's hair!",
    ('CeladonCity_Condominiums_3F', 'SomeonesPlayingGame'): "Someone's playing a game instead of working!|Probably a TEAM MAGMA grunt.",
    ('CeladonCity_Condominiums_Roof', 'IKnowEverything'): "I KNOW EVERYTHING! Even where the gangs hide their loot!",
    ('DiglettsCave_SouthEntrance', 'DiglettDugThisTunnel'): "DIGLETT dug this long tunnel!|It goes all the way to VIRIDIAN. The ROCKETS use it to smuggle things.",
    ('MtMoon_1F', 'ZubatIsABloodsucker'): "Beware! ZUBAT is a bloodsucker! So is TEAM YELL's toll.",
    ('MtMoon_1F', 'BrockHelpsExcavateFossils'): "I dig for fossils under MT. MOON.|TEAM YELL sells them in PEWTER. I just want to study them!",
    ('MtMoon_B2F', 'LabOnCinnabarRegeneratesFossils'): "Far away, on CINNABAR ISLAND, there's a LAB that brings fossils back to life.|TEAM GALACTIC runs it now, but it still works.",
    ('PokemonTower_1F', 'ErectedInMemoryOfDeadMons'): "POKéMON TOWER was built in memory of POKéMON that died.|Since the outbreak, it fills up fast.",
    ('PokemonTower_1F', 'CameToPrayForDepartedClefairy'): "I came to pray for my CLEFAIRY.|It was lost in the gang fights… Sniff!",
    ('PokemonTower_1F', 'GrowlitheWhyDidYouDie'): "My GROWLITHE… It protected me from the ROCKETS. Why did you have to die?",
    ('SSAnne_1F_Corridor', 'PassengersAreRestless'): "The passengers are restless.|Some of them are gangsters in disguise. Watch out!",
    ('SSAnne_1F_Room2', 'CruisingAroundWorld'): "We're sailing away from KANTO, my children and I. Far from the gangs.",
    ('SSAnne_1F_Room3', 'CruiseIsElegantAndCozy'): "A cruise is so elegant!|No burning cities, no gangs. Just the sea.",
    ('SSAnne_2F_Corridor', 'ThisShipIsLuxuryLiner'): "This ship is a luxury liner for TRAINERS!|ARCHER's ROCKETS check every cabin, though.",
    ('SSAnne_2F_Room6', 'ManyPeopleGetSeasick'): "Many people get seasick. Or ROCKET-sick.",
    ('SSAnne_Deck', 'ScrubbingDecksHardWork'): "Whew! Scrubbing decks is hard work!|The ROCKETS leave such a mess.",
    ('SSAnne_Kitchen', 'SoBusyImDizzy'): "I'm so busy I'm dizzy!|The ROCKETS eat like SNORLAX!",
    ('SafariZone_Center_RestHouse', 'CatchingMonsAsGifts'): "I'm catching POKéMON before TEAM PLASMA “frees” them all.",
    ('SafariZone_East_RestHouse', 'TiredFromAllTheFun'): "Whew! I'm tired! At least there are no gangs in here.",
    ('SafariZone_North_RestHouse', 'MyEeveeEvolvedIntoFlareon'): "My EEVEE evolved into FLAREON.|TEAM PLASMA says that's cruel. Whatever.",
    ('SafariZone_West_RestHouse', 'KogaPatrolsSafariEverySoOften'): "The SAFARI ZONE is huge.|TEAM PLASMA patrols it now, looking for POKéMON to “free.”",
    ('SafariZone_West_RestHouse', 'HikedLotsDidntSeeMonIWanted'): "I hiked a lot, but TEAM PLASMA scared all the POKéMON away.",
    ('SilphCo_1F', 'WelcomePresidentInBoardroom'): "Welcome to SILPH CO.|The PRESIDENT is in the boardroom on 11F. If the ROCKETS let him go.",
    ('SilphCo_6F', 'RocketsTryingToConquerWorld'): "TEAM ROCKET wants to conquer the world with our products.|TEAM FLARE wants them too!",
    ('SilphCo_6F', 'TargetedSilphForOurMonProducts'): "Every gang in KANTO wants SILPH's POKéMON products.",
    ('SilphCo_6F', 'ComeWorkForSilphWhenYoureOlder'): "Come work for SILPH when this is over. If it's ever over.",
    ('SilphCo_8F', 'WonderIfSilphIsFinished'): "I wonder if SILPH is finished… ROCKET or FLARE, somebody will take it.",
    ('UndergroundPath_EastEntrance', 'DoYouGoToCeladonDeptStore'): "The DEPT. STORE in CELADON still has stuff.|If AQUA and MAGMA let you in.",
    ('UndergroundPath_SouthEntrance', 'PeopleLoseThingsInTheDarkness'): "People lose things down there. Or the gangs take them.",
    ('FiveIsland_House1', 'HusbandWentOffFishing'): "My husband went fishing.|He says the sea is the only place without gangs.",
    ('FiveIsland_House2', 'MeadowBelongedToFamily'): "That MEADOW belonged to our family.|Then TEAM ROCKET built a warehouse on it!",
    ('FiveIsland_PokemonCenter_1F', 'OnMyWayToPrepareMeal'): "I'm cooking for a rich lady in the RESORT.|She pays well. Better than the gangs.",
    ('FourIsland_Mart', 'IcefallCaveIsFrigid'): "The SEVII ISLANDS are warm, except ICEFALL CAVE.|And poachers hide in there!",
    ('FourIsland_PokemonCenter_1F', 'NewPokemonJournalHasntArrived'): "The new POKéMON JOURNAL hasn't arrived?|The gangs must have stolen the delivery.",
    ('OneIsland_House1', 'GazeUponVolcanoOnSunnyDays'): "On sunny days, I gaze at the volcano.|At least it's not a burning city.",
    ('OneIsland_House2', 'IWantToStayHereForever'): "Young people want to go to the big cities.|With all the gangs? I'm staying here!",
    ('OneIsland_PokemonCenter_1F', 'EnormousVolcanoOnIsland'): "There's a huge volcano on this island.|Strange people have been digging there lately.",
    ('OneIsland_TreasureBeach', 'GoodThingsWashUpOnBeach'): "Good things wash up on this beach.|Lately, mostly loot bags from KANTO.",
    ('OneIsland_KindleRoad_EmberSpa', 'HotSpringIsTherapeutic'): "A hot spring is good for the soul. Especially after a gang fight.",
    ('SevenIsland_Mart', 'PreparationsCompleteForRuins'): "Preparations complete! I'll explore the RUINS before TEAM ROCKET loots them.",
    ('SevenIsland_PokemonCenter_1F', 'CheckOutTrainerTower'): "If you're confident, check out TRAINER TOWER. No gangs, just strong TRAINERS!",
    ('SixIsland_House', 'GoodPlaceForNatureAndHistory'): "Nothing exciting here for young people.|No gangs either. That's good enough for me.",
    ('SixIsland_Mart', 'ShouldBuyMailAndWriteLetter'): "I should write my son in KANTO and tell him to come home.",
    ('ThreeIsland_House3', 'WantedToLiveSomewhereQuiet'): "I moved here to get away from the gangs.|Then the BIKERS came. Sigh.",
    ('ThreeIsland_Mart', 'TrueThatCeldadonDeptStoreBigger'): "Is it true? The CELADON DEPT. STORE is huge?|And AQUA and MAGMA fight over it?",
    ('ThreeIsland_PokemonCenter_1F', 'ImpossibleToSurfBetweenIslands'): "You can't SURF between the islands. The currents are too fast.|That's why the gangs need ships.",
    ('TwoIsland_PokemonCenter_1F', 'OldLadyLivesOutOnCape'): "Not many people live on TWO ISLAND.|An old lady lives out on the cape. Even the gangs are scared of her.",
    ('TrainerTower_Lobby', 'StairsTougherThanAnyBattle'): "Gasp, gasp…|These stairs are tougher than any gang!",
})


# GYMS: badge explanations (field move + new level cap), boss defeats and the GYM advisor's tips
MANUAL.update({
    ('PewterCity_Gym', 'LetMeTakeYouToTheTop'): "Hiya! Do you dream big?|Do you dare to dream of becoming the KINGPIN of KANTO?|I'm no fighter, but I know every boss. Let me take you to the top!",
    ('PewterCity_Gym', 'YoureChampMaterial'): "Just as I thought! You're KINGPIN material!",
    ('CeruleanCity_Gym', 'MistyDefeat'): "GUZMA: What?! Beat down by a rookie?|…Fine. Take the RUNNER RANK. You earned it.",
    ('CeruleanCity_Gym', 'ExplainCascadeBadge'): "The RUNNER RANK raises your level cap to Lv. 32.|With the HM in your BAG, your first POKéMON can CUT small trees, even if it never learned the move.|Here, take my favorite TM too.",
    ('CeruleanCity_Gym', 'GymGuyAdvice'): "Yo! Boss in the making!|GUZMA of TEAM SKULL uses BUG and WATER POKéMON.|FIRE, FLYING and ROCK moves crush his bugs. ELECTRIC zaps the water ones!",
    ('CeruleanCity_Gym', 'WeMakePrettyGoodTeam'): "You beat GUZMA! See, what'd I tell ya?|You and me, kid, we make a pretty good crew!",
    ('VermilionCity_Gym', 'LtSurgeDefeat'): "ARCHER: Impossible…|You're the real deal, kid. Take the SOLDIER RANK!",
    ('VermilionCity_Gym', 'ExplainThunderBadgeTakeThis'): "The SOLDIER RANK raises your level cap to Lv. 38.|With the HM in your BAG, any POKéMON in your crew can FLY.|Take this too, kid!",
    ('VermilionCity_Gym', 'GymGuyAdvice'): "Yo! Boss in the making!|ARCHER of the NEW TEAM ROCKET uses POISON and DARK POKéMON.|GROUND moves wreck the poison ones, FIGHTING moves the dark ones.|ARCHER is careful. He locked himself in, so it won't be easy to reach him.",
    ('VermilionCity_Gym', 'GymGuyPostVictory'): "Whew! You took down a ROCKET executive!",
    ('CeladonCity_Gym', 'ErikaDefeat'): "ARCHIE: Bwahahaha! Beaten, and by a kid!|MAXIE: Logic says you deserve the ENFORCER RANK.",
    ('CeladonCity_Gym', 'ExplainRainbowBadgeTakeThis'): "The ENFORCER RANK raises your level cap to Lv. 44.|With the HM in your BAG, your first POKéMON can use STRENGTH outside of battle.|Take this with you too.",
    ('CeladonCity_Gym', 'KayIntro'): "This GYM belongs to TEAM AQUA and TEAM MAGMA. Pick a side or get lost!",
    ('FuchsiaCity_Gym', 'KogaDefeat'): "GHETSIS: Impossible! A mere pawn…|Take the HITMAN RANK. For now.",
    ('FuchsiaCity_Gym', 'KogaExplainSoulBadge'): "The HITMAN RANK raises your level cap to Lv. 49.|With the HM in your BAG, your first POKéMON can SURF outside of battle.|Take this too.",
    ('FuchsiaCity_Gym', 'GymGuyAdvice'): "Yo! Boss in the making!|This GYM is full of invisible walls! GHETSIS looks close, but he's blocked off.|Find the gaps in the walls to reach him.|His HYDREIGON is DARK and DRAGON. FAIRY and FIGHTING moves hit it hard!",
    ('FuchsiaCity_Gym', 'GymGuyPostVictory'): "You beat GHETSIS! TEAM PLASMA is shaking!",
    ('SaffronCity_Gym', 'SabrinaDefeat'): "LYSANDRE: This loss… is beautiful in its own way.|You have earned the CAPO RANK.",
    ('SaffronCity_Gym', 'ExplainMarshBadgeTakeThis'): "The CAPO RANK raises your level cap to Lv. 54.|With the HM in your BAG, your first POKéMON can use ROCK SMASH outside of battle.|Please take this TM as well.",
    ('SaffronCity_Gym', 'GymGuyAdvice'): "Yo! Boss in the making!|LYSANDRE of TEAM FLARE uses FIRE, DARK and FLYING POKéMON.|WATER, ROCK and ELECTRIC moves will burn him out!",
    ('SaffronCity_Gym', 'GymGuyPostVictory'): "You beat LYSANDRE! TEAM FLARE's fashion show is over!",
    ('CinnabarIsland_Gym', 'BlaineDefeat'): "CYRUS: An error in my calculation…|You have earned the UNDERBOSS RANK.",
    ('CinnabarIsland_Gym', 'ExplainVolcanoBadge'): "The UNDERBOSS RANK raises your level cap to Lv. 58.|With the HM in your BAG, your first POKéMON can climb WATERFALLS.|Take this as well.",
    ('CinnabarIsland_Gym', 'GymGuyAdvice'): "Yo! Boss in the making!|CYRUS of TEAM GALACTIC uses DARK, ICE and FLYING POKéMON.|FIGHTING, ROCK and ELECTRIC moves break his perfect logic!",
    ('CinnabarIsland_Gym', 'GymGuyPostVictory'): "You beat CYRUS! His new world will have to wait!",
    ('ViridianCity_Gym', 'GiovanniPostBattle'): "You surpassed me, recruit. No… boss.|TEAM ROCKET follows you now.|Go to the POKéMON LEAGUE and take all of KANTO for us.|I'll be watching.",
    ('ViridianCity_Gym', 'ExplainEarthBadgeTakeThis'): "The KINGPIN RANK raises your level cap to Lv. 66.|It is the mark of a true boss. With it, you can challenge the POKéMON LEAGUE.|Take this TM too. A gift for your LEAGUE challenge.",
    ('ViridianCity_Gym', 'GymGuyAdvice'): "Yo! Boss in the making!|This is GIOVANNI's own GYM. He is the toughest boss of all.|His team is all GROUND. WATER, GRASS and ICE moves are your best bet!",
    ('ViridianCity_Gym', 'GymGuyPostVictory'): "Blow me away! You beat GIOVANNI himself!",
})

# texts with control codes (badge fanfare, music) keep those lines as they are
RAW = {
    ('PewterCity_Gym', 'BrockDefeat'): [
        "I took you for granted, and so\\n", "I lost.\\p",
        "Word on the street is you're GIOVANNI's\\n", "new blood. Fine. You earned your\\l", "first stripe.\\p",
        "{FONT_NORMAL}{PLAYER} was promoted to\\n",
        "PROSPECT!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}\\p",
        "{FONT_MALE}The PROSPECT RANK raises your\\n", "level cap to Lv. 27.\\p",
        "With the HM in your BAG, your first\\n", "POKéMON can use FLASH, even if it\\l", "never learned the move.$"],
    ('ViridianCity_Gym', 'GiovanniIntro'): [
        "Fwahahaha!\\n", "So, my recruit has come home.\\p",
        "You took every RANK in KANTO.\\n", "Now try to take mine!\\p",
        "Once more, you shall face\\n", "GIOVANNI, the greatest TRAINER!{PLAY_BGM}{MUS_RG_ENCOUNTER_ROCKET}$"],
    ('ViridianCity_Gym', 'GiovanniDefeat'): [
        "Ha!\\n", "That was a truly intense fight.\\l", "You have won!\\p",
        "Take my place - the KINGPIN RANK!\\n",
        "{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"],
}


# POKéMON LEAGUE: SILVER is the CHAMPION, GIOVANNI comes instead of PROF. OAK
MANUAL.update({
    ('PokemonLeague_ChampionsRoom', 'Intro'): "{RIVAL}: So you made it, {PLAYER}.|While you ran errands for my father, I beat all four of them.|Now I'm the CHAMPION. Not GIOVANNI's son. Not his errand boy. Me.|This decides who takes over the family. Let's go!",
    ('PokemonLeague_ChampionsRoom', 'RematchIntro'): "{RIVAL}: Back again, {PLAYER}?|I've trained every day since you took my title.|This time the family is mine!",
    ('PokemonLeague_ChampionsRoom', 'Defeat'): "NO! Not again!|You beat me at my very best…",
    ('PokemonLeague_ChampionsRoom', 'Victory'): "{RIVAL}: Hah! I knew it!|Father picked the wrong recruit!",
    ('PokemonLeague_ChampionsRoom', 'PostBattle'): "{RIVAL}: Why?! I never made a single mistake…|…Fine. You're the CHAMPION now. And the new head of the family.|Don't you dare make father regret it.",
    ('PokemonLeague_ChampionsRoom', 'OakPlayer'): "GIOVANNI: {PLAYER}!",
    ('PokemonLeague_ChampionsRoom', 'OakCongratulations'): "GIOVANNI: So, you've won. Well done, boss.|You came to me with nothing but {STR_VAR_1}.|Now all of KANTO bows to you. Fwahahaha!",
    ('PokemonLeague_ChampionsRoom', 'OakImDisappointedRival'): "GIOVANNI: SILVER…|You beat the ELITE FOUR, then lost to my recruit.|You fight only for yourself. A boss fights for his crew.|Learn that, and one day you will lead TEAM ROCKET.",
    ('PokemonLeague_ChampionsRoom', 'OakPlayerComeWithMe'): "GIOVANNI: {PLAYER}.|You didn't win alone. Your crew made you strong.|Come with me!",
    ('PokemonLeague_HallOfFame', 'OakCongratulations'): "GIOVANNI: Ahem! Congratulations, {PLAYER}!|This is the HALL OF FAME. The greatest TRAINERS of all time are honored here.|Today a gangster joins them. Fwahahaha!|You and your crew are HALL OF FAMERS!",
})


# SEVII ISLANDS, the rest: islanders, LORELEI (a retired ELITE FOUR member now), SILVER, the BIKERS and the ROCKET remnants
MANUAL.update({
    ('OneIsland', 'LuckyToHaveCelioHere'): "We're on an island in the middle of nowhere.|Lucky for us, the gangs think so too.",
    ('OneIsland', 'PokemonNetCenterSign'): "Connecting the Islands to KANTO!|POKéMON NET CENTER",
    ('OneIsland_House1', 'LastTimeMtEmberErupted'): "When did MT. EMBER last erupt?|…Before the cities started burning, at least.",
    ('OneIsland_KindleRoad', 'RouteSign'): "This is KINDLE ROAD.|Go straight for MT. EMBER. Beware of diggers!",
    ('OneIsland_KindleRoad_EmberSpa', 'WaterWarmsMeToCore'): "Haaaah! This is heavenly!|The water melts away every gang war. You should jump in too!",
    ('OneIsland_KindleRoad_EmberSpa', 'SeeHowSmoothMySkinIs'): "See how smooth my skin is?|No stress here. No gangs, no fires. Just hot water.",
    ('OneIsland_KindleRoad_EmberSpa', 'BrunoVisitsSpaOnOccasion'): "After a day of training, nothing beats a hot soak.|BRUNO used to come here before the gangs took the LEAGUE from him.",
    ('TwoIsland', 'JoyfulGameCornerSign'): "Skip & Chomp!|Joyful GAME CORNER - No Gangsters!",
    ('TwoIsland', 'FellowMovedFromCeladonCity'): "That fellow moved here from CELADON.|He couldn't stand AQUA and MAGMA anymore.",
    ('TwoIsland', 'BrunoCameToIslandWhileBack'): "Did you know? A famous TRAINER came here a while back.|BRUNO! He lost the LEAGUE to new faces and now trains out here.",
    ('TwoIsland', 'OldWomanLivesOutOnCape'): "An old woman lives out on the cape.|She judges POKéMON. Even the gangs are scared of her!",
    ('TwoIsland', 'BuyRareItemsHere'): "Smart TRAINERS buy rare items here. Far from KANTO's gang prices!",
    ('TwoIsland_PokemonCenter_1F', 'HaveYouVisitedGameCorner'): "Have you been to the GAME CORNER?|It's honest, unlike the ROCKET one in CELADON!",
    ('TwoIsland_JoyfulGameCorner', 'IsThisOnlyThreeIsland'): "Hah? What is this GAME CORNER?|Our boss said we'd take over an island. This one's too sad!",
    ('ThreeIsland', 'WouldntWantToSeeBikersHereAgain'): "THREE ISLAND has the most people of all the islands.|We fled from KANTO's gangs. Then those BIKERS came too!",
    ('ThreeIsland', 'YouveGotImpressiveMons'): "You've got seriously impressive POKéMON. Like a real gang boss!",
    ('ThreeIsland_House2', 'IAdmireSabrina'): "Have you heard of LYSANDRE in SAFFRON?|He took over the GYM with TEAM FLARE. Scary, but so stylish!",
    ('ThreeIsland_House4', 'GhostsInBerryForest'): "Never go to the BERRY FOREST alone.|There are ghosts there. And worse, BIKERS.",
    ('ThreeIsland_Mart', 'PeopleHealWithBerriesFromForest'): "I buy medicine here sometimes.|But BERRIES from the BERRY FOREST are free. No gang taxes!",
    ('ThreeIsland_PokemonCenter_1F', 'PCNetworkCanLinkWithKanto'): "Did you hear? The PCs here can link with KANTO now.|Let's hope the gangs can't hack them.",
    ('ThreeIsland_Port', 'WereKantoRiderFederation'): "We're the KANTO RIDER FEDERATION!|Vroom! KANTO got too hot with all the gangs, so this island is ours now!",
    ('FourIsland', 'MadeSlushieUsingIcefallCaveIce'): "Ooh, chilly!|I made a slushie with ice from ICEFALL CAVE. Watch out for poachers in there!",
    ('FourIsland', 'LoreleiMetLaprasAsChild'): "LORELEI has gone back to KANTO.|She wants to take the LEAGUE back from the newcomers.",
    ('FourIsland', 'RivalAlreadyGotEggBeSmellingYa'): "{RIVAL}: Hey! {PLAYER}!|What are you doing on the SEVII ISLANDS? Following me?|Father sent me to scout the islands. The DAY-CARE already gave me an EGG.|Stay out of my way!",
    ('FourIsland_Mart', 'LoreleiGrewUpOnThisIsland'): "A great TRAINER grew up on this island.|LORELEI! She was in the ELITE FOUR, before the new heroes took over.",
    ('FourIsland_PokemonCenter_1F', 'RareIceMonsInCave'): "A rare ICE POKéMON lives in the cave.|Poachers sell it to the gangs in KANTO. Terrible!",
    ('FourIsland_LoreleisHouse', 'IllReturnToLeagueInShortWhile'): "LORELEI: You solved all the problems here?|Wonderful. Then I can go back to KANTO.|The LEAGUE is in the hands of strangers now. I want my place back.",
    ('FiveIsland', 'WeirdBuildingInMeadow'): "Oh, this stinks.|TEAM ROCKET built a warehouse on the MEADOW. I can't play there anymore!",
    ('FiveIsland_PokemonCenter_1F', 'YoureOnQuestToCatchEveryMon'): "You're catching every kind of POKéMON for your crew?|How incredible. Please show me sometime.",
    ('FiveIsland_RocketWarehouse', 'GiovannisKidHasRedHair'): "What, you again?|You're GIOVANNI's new recruit, aren't you?|The boss's real kid has red hair… SILVER. Where is he?|Tell GIOVANNI the islands are ours now!",
    ('FiveIsland_WaterLabyrinth', 'LetMeTakeLookAtMons'): "You travel a lot, don't you?|Gang life is hard. You're not driving your POKéMON too harshly, are you?",
    ('SixIsland', 'ThatWayToWaterPathRuinValley'): "Keep going this way to reach the WATER PATH. It leads to RUIN VALLEY.|TEAM ROCKET has been sniffing around there too.",
    ('SixIsland_GreenPath', 'RightRouteSign'): "GREEN PATH|Think Green, Keep Gangs Out!",
    ('SixIsland_Mart', 'AgathaOldestEverEliteFourMember'): "Play with POKéMON and you'll live long!|Take AGATHA. She was the oldest ELITE FOUR member, and she still trains every day.",
    ('SixIsland_PokemonCenter_1F', 'SomethingHiddenOnThisIsland'): "I've searched for RUINS for twenty years.|Something is hidden on this island. I have to find it before TEAM ROCKET does!",
    ('SixIsland_PokemonCenter_1F', 'RivalImpossibleToGetAllMonsAroundHere'): "{RIVAL}: Hey, {PLAYER}!|How's your crew coming along?|I've caught every strong POKéMON on these islands. Father will see who the real heir is!",
    ('SixIsland_WaterPath', 'WantedUltimateHorn'): "Wanted! The Ultimate Horn!|Reward paid in cash. No gangsters.",
    ('SevenIsland', 'IslandsMadeInSevenDays'): "They're called the SEVII ISLANDS because there are seven.|…At least that's what they say. Plenty of places to hide.",
    ('SevenIsland_Mart', 'NeedToFishOnSevenIsland'): "I need to fish on SEVEN ISLAND to finish my fishing tour.|Better than fishing in KANTO's polluted harbors!",
    ('SevenIsland_PokemonCenter_1F', 'DidYouRideSeagallopHere'): "Did you ride a SEAGALLOP ferry here?|They're fast. The gangs can't catch them!",
    ('SevenIsland_SevaultCanyon', 'BrunoTrainedWithBrawly'): "Training alone isn't bad. But it's good to aim for the top with a rival.|BRUNO trains here now, since he lost his place in the LEAGUE.",
    ('SevenIsland_SevaultCanyon_Entrance', 'RouteSign'): "SEVAULT CANYON ENTRANCE|Please don't damage the plants. Or the RUINS. Looking at you, ROCKETS.",
    ('SevenIsland_TrainerTower', 'TrainerTowerSign'): "TRAINER TOWER ENTRANCE|Rise to the Challenge! Gang Bosses Welcome!",
})


# CAVES and DUNGEONS: GIOVANNI tests his recruit, SILVER as his son, gang flavor everywhere (hints stay as they are)
MANUAL.update({
    ('RocketHideout_B4F', 'GiovanniIntro'): "So! You found my secret hideout. Good.|A boss has to beat anyone who stands in his way. Even me.|Show me what my recruit has learned!",
    ('RocketHideout_B4F', 'GiovanniDefeat'): "Fwahaha! Well done!",
    ('RocketHideout_B4F', 'GiovanniPostBattle'): "You raise your POKéMON with care, and you fight like a boss.|Take the SILPH SCOPE. LAVENDER's ghosts won't stop you.|We'll meet again, {PLAYER}.",
    ('SilphCo_11F', 'GiovanniIntro'): "Ah, {PLAYER}! So we meet again!|I'm making the PRESIDENT an offer he can't refuse.|TEAM FLARE wants SILPH too. Prove you're strong enough to keep them away!",
    ('SilphCo_11F', 'GiovanniDefeat'): "Hah! You did it again!",
    ('SilphCo_11F', 'GiovanniPostBattle'): "Fine. With you around, nobody will dare touch SILPH.|The MASTER BALL stays here. For now.|{PLAYER}! Never forget who made you a boss!",
    ('SilphCo_11F', 'ThanksForRescuingUs'): "SECRETARY: Thank you.|You chased off TEAM FLARE AND made GIOVANNI back down. Who ARE you?",
    ('SilphCo_5F', 'RocketBossLookingForStrongMons'): "TEAM ROCKET took the building, then TEAM FLARE tried too.|Everyone wants SILPH's POKéMON. I hope our PRESIDENT is okay.",
    ('SilphCo_6F', 'RocketsTookOverBuilding'): "The ROCKETS came and took over! And TEAM FLARE waits outside!",
    ('SilphCo_8F', 'ToRocketBossMonsAreTools'): "The gang bosses are all cruel.|To them, POKéMON are just tools. I hope you're different.",
    ('SilphCo_10F', 'WaaaImScared'): "Waaaaa! Two gangs in one building! I'm scared!",
    ('SilphCo_7F', 'RivalIntro'): "{RIVAL}: Hahaha! I knew you'd turn up!|Father sent you to SILPH too? He doesn't trust me.|I'll show him who's the better heir!",
    ('SilphCo_7F', 'RivalDefeat'): "Tch! Fine, go see Father in the boardroom!",
    ('SilphCo_7F', 'RivalPostBattle'): "{RIVAL}: Don't think this is over.|One day I'll lead TEAM ROCKET, not you!|Good luck in there. You'll need it.",
    ('PokemonTower_2F', 'RivalIntro'): "{RIVAL}: Hey, {PLAYER}!|What, scared of ghosts? Ha!|I'll show you how a real ROCKET fights!",
    ('PokemonTower_2F', 'RivalDefeat'): "What?! You got lucky!",
    ('PokemonTower_2F', 'RivalPostBattle'): "The ROCKETS upstairs are Father's men, but they don't listen to me.|The ghosts don't either. Get a SILPH SCOPE if you want to see them.|Later, loser!",
    ('PokemonTower_1F', 'SenseSpiritsUpToMischief'): "I am a CHANNELER.|Since the outbreak, the spirits here are restless.|I sense them high in the TOWER.",
    ('PokemonTower_5F', 'RestHereInPurifiedSpace'): "Come, child! This space is pure. No gangs, no ghosts. Rest here.",
    ('SSAnne_2F_Corridor', 'RivalIntro'): "{RIVAL}: Well, well, {PLAYER}!|Father sent you to the S.S. ANNE too?|ARCHER's ROCKETS run this ship. Let's see who impresses them more!",
    ('SSAnne_2F_Corridor', 'RivalDefeat'): "Humph! At least you're not totally useless.",
    ('SSAnne_2F_Corridor', 'RivalPostBattle'): "{RIVAL}: I heard there's a CUT master on board.|Just a seasick old man! Go rub his back if you want.|Smell ya later!",
    ('SSAnne_1F_Room1', 'ImAGlobalPoliceAgent'): "Ssh…! I'm a GLOBAL POLICE agent.|I'm watching ARCHER's ROCKETS. They smuggle POKéMON on this ship!",
    ('SSAnne_1F_Corridor', 'LeStrongSilentType'): "Bonjour! I am le waiter!|I serve everybody. Even le gangsters. Especially le gangsters, they tip well!",
    ('SSAnne_2F_Room5', 'HaveYouGoneToSafariZone'): "Have you been to the SAFARI ZONE in FUCHSIA?|Go before TEAM PLASMA “frees” all the POKéMON!",
    ('SSAnne_Deck', 'FeelSeasick'): "Urf… I feel ill…|Seasick, or ROCKET-sick, I don't know anymore…",
    ('SSAnne_Kitchen', 'BusyOutOfTheWay'): "You, mon petit! We're busy cooking for le ROCKETS! Out of the way!",
    ('SSAnne_Kitchen', 'SawOddBerryInTrash'): "I saw an odd BERRY in the trash. Probably smuggled. Don't tell anyone!",
    ('MtMoon_B2F', 'MiguelIntro'): "Hey, stop! I found these fossils first!|TEAM YELL pays good money for them!",
    ('MtMoon_B2F', 'MiguelDefeat'): "Okay! I'll share!",
    ('MtMoon_B2F', 'Grunt2Intro'): "We, TEAM ROCKET, still dig in MT. MOON!|TEAM YELL can scream all they want!",
    ('PokemonMansion_2F', 'ArnieIntro'): "I can't get out! TEAM GALACTIC turned this place into one big puzzle!",
    ('PokemonMansion_2F', 'ArnieDefeat'): "Oh, no! My bag of loot!",
    ('PokemonMansion_3F', 'SimonIntro'): "This place is huge! I'm looting it before TEAM GALACTIC does!",
    ('PokemonMansion_3F', 'SimonPostBattle'): "I wonder where my partner went. Maybe the GALACTIC scientists got him…",
    ('PokemonMansion_3F', 'BraydonIntro'): "My mentor lived here, before TEAM GALACTIC came.",
    ('RockTunnel_1F', 'RouteSign'): "ROCK TUNNEL|CERULEAN CITY - LAVENDER TOWN. No Lights, No Laws.",
    ('RockTunnel_1F', 'DanaPostBattle'): "You looked cute and harmless. Was I ever wrong! A real gang boss!",
    ('VictoryRoad_3F', 'ColbyIntro'): "Only the strongest boss can pass here!",
    ('FourIsland_IcefallCave_Back', 'GruntIntro'): "W-what?! Who says we can't sell the POKéMON we catch?|The gangs in KANTO pay a fortune!",
    ('FourIsland_IcefallCave_Back', 'GruntDefeat'): "We didn't plan on this!",
    ('FourIsland_IcefallCave_Back', 'ShutItLadyLeaveUsBe'): "Aww, shut it, lady. You're not ELITE FOUR anymore.|Don't let your glasses get all steamed up!",
    ('FourIsland_IcefallCave_Back', 'ThankYouThisIsAwful'): "{PLAYER}, thank you. But this is awful…|I was born and raised on these islands.|The gangs from KANTO have reached even here. I have to stop them.",
    ('FiveIsland_RocketWarehouse', 'GideonIntro'): "I almost sold the SAPPHIRE to TEAM ROCKET for huge money…|Then GIOVANNI's new pet had to show up!",
    ('FiveIsland_RocketWarehouse', 'ManyMonsLockedInPen'): "Many POKéMON are locked in the pen… Ready to be shipped to KANTO's gangs.",
    ('FiveIsland_RocketWarehouse', 'GetLostLeaveMeBe'): "What do you want now? Tell GIOVANNI we're done. Get lost!",
    ('MtEmber_Exterior', 'Grunt1Intro'): "You've been eavesdropping on us! You're GIOVANNI's spy, aren't you?",
    ('MtEmber_Exterior', 'Grunt2Intro'): "Trying to take our treasure for GIOVANNI? Don't bet on it!",
    ('MtEmber_Exterior', 'Grunt2PostBattle'): "Forget everything you heard! The islands belong to us now!",
    ('ThreeIsland_DunsparceTunnel', 'ProspectingForGold'): "Hey there! I'm prospecting for gold!|When I strike it rich, I'll buy my own GYM. Like the gang bosses!",
    ('UndergroundPath_WestEntrance', 'SleepyMonNearCeladon'): "A sleepy POKéMON blocks a road near CELADON too. Even the gangs can't move it!",
    ('SafariZone_Center_RestHouse', 'WhereDidErikGo'): "SARA: Where did my boyfriend ERIK go?|I hope TEAM PLASMA didn't “free” him too…",
    ('SafariZone_East_RestHouse', 'CaughtChanseyAllWorthwhile'): "I caught a CHANSEY! Before TEAM PLASMA could set it “free”!",
})


from route_texts_rest import REST   # noqa: E402  remaining town / house / story texts
MANUAL.update(REST)


# texts with hints the player needs (keys, passwords, teleport tiles, the SILPH SCOPE …) keep their wording
ORIG_REF = 'f93f63af'   # last commit before this tool touched any texts
HINTS = re.compile(r'KEY|SCOPE|ELEVATOR|LIFT|PASSWORD|TELEPORT|HINT|TILE|SWITCH|BASEMENT|SECRET|FLUTE|GOLD TEETH|'
                   r'DOOR|LOCK|WALL|GAP|TRASH|QUIZ|ARROW|PANEL|FLOOR|SPIN|'
                   r'STRENGTH|SURF|CUT|FLASH|BOULDER|HOLE|STAIRS|LADDER|TICKET|PASS\b', re.I)
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


def original_texts(path):
    import subprocess
    rel = os.path.relpath(path, ROOT)
    src = subprocess.run(['git', 'show', '%s:%s' % (ORIG_REF, rel)], cwd=ROOT, capture_output=True, text=True).stdout
    return {l: ''.join(re.findall(r'\.string "(.*)"', b)) for l, b in re.findall(r'^(\w+)::?\n((?:\t\.string .*\n)+)', src, re.M)}


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
    files = sorted(glob.glob(os.path.join(ROOT, 'data/maps/*_Frlg/scripts.inc')))
    for path in files:
        mapname = path.split('/')[-2].replace('_Frlg', '')
        s = open(path).read()
        orig = original_texts(path)
        changed = 0
        gang = GANG_OF.get(mapname) or next((g for k, g in GANG_OF.items() if mapname.startswith(k + '_')), None)
        gym = GYM_GANG.get(mapname)
        if gym:
            gang = gym
        elif 'Gym' in mapname or mapname.startswith('PokemonLeague'):
            gang = None   # the league has its own texts
        if gang:
            used = {}
            prefix = mapname + '_Text_'
            for trainer, intro, defeat in all_battles:
                if not intro.startswith(prefix) or 'Rematch' in intro or not intro.endswith('Intro'):
                    continue
                if 'RIVAL' in trainer or trainer.startswith(('TRAINER_LEADER', 'TRAINER_GANG_')) \
                        or any(k in classes.get(trainer, '') for k in SKIP_CLASSES):
                    continue
                grp = 'gym' if gym else group_of(classes.get(trainer, ''))
                base = intro[:-len('Intro')]
                post = base + 'PostBattle' if re.search(r'^%sPostBattle:' % re.escape(base), s, re.M) else base + 'Post'
                h = int(hashlib.md5((mapname + grp).encode()).hexdigest(), 16)   # one rotation per route and group
                n = used.get(grp, 0)
                used[grp] = n + 1
                g, b, t = gang
                for kind, label, shift in (('intro', intro, 0), ('defeat', defeat, 0), ('post', post, 0),
                                           ('intro', base + 'RematchIntro', 1)):
                    if HINTS.search(orig.get(label, '')):
                        continue   # keep gameplay hints
                    pool = TEMPLATES[grp][kind]
                    text = pool[(h + n + shift) % len(pool)].format(G=g, B=b, T=t)
                    s, ok = replace(s, label, wrap(text))
                    changed += ok
        for (mp, suffix), text in MANUAL.items():
            if mp == mapname:
                s, ok = replace(s, '%s_Text_%s' % (mapname, suffix), wrap(text))
                if not ok:   # some maps use another map's prefix for their texts
                    m = re.search(r'^(\w+_Text_%s)::?$' % re.escape(suffix), s, re.M)
                    if m:
                        s, ok = replace(s, m.group(1), wrap(text))
                assert ok, (mp, suffix)
                changed += ok
        for (mp, suffix), lines in RAW.items():
            if mp == mapname:
                s, ok = replace(s, '%s_Text_%s' % (mapname, suffix), lines)
                assert ok, (mp, suffix)
                changed += ok
        if changed:
            open(path, 'w').write(s)
            total += changed
    print('%d texts written' % total)


if __name__ == '__main__':
    main()
