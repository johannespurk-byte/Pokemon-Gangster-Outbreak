SixIsland_PatternBush_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_PatternBush_OnTransition
	.byte 0

SixIsland_PatternBush_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND_PATTERN_BUSH
	getplayerxy VAR_TEMP_1, VAR_TEMP_2
	call_if_ge VAR_TEMP_1, 50, SixIsland_PatternBush_EventScript_SetEscapeRightExit
	call_if_le VAR_TEMP_1, 49, SixIsland_PatternBush_EventScript_SetEscapeLeftExit
	end

SixIsland_PatternBush_EventScript_SetEscapeRightExit::
	setescapewarp MAP_SIX_ISLAND_GREEN_PATH, 255, 64, 10
	return

SixIsland_PatternBush_EventScript_SetEscapeLeftExit::
	setescapewarp MAP_SIX_ISLAND_GREEN_PATH, 255, 45, 10
	return

SixIsland_PatternBush_EventScript_Bethany::
	trainerbattle_single TRAINER_PKMN_BREEDER_BETHANY, SixIsland_PatternBush_Text_BethanyIntro, SixIsland_PatternBush_Text_BethanyDefeat
	msgbox SixIsland_PatternBush_Text_BethanyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Allison::
	trainerbattle_single TRAINER_PKMN_BREEDER_ALLISON, SixIsland_PatternBush_Text_AllisonIntro, SixIsland_PatternBush_Text_AllisonDefeat
	msgbox SixIsland_PatternBush_Text_AllisonPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Garret::
	trainerbattle_single TRAINER_BUG_CATCHER_GARRET, SixIsland_PatternBush_Text_GarretIntro, SixIsland_PatternBush_Text_GarretDefeat
	msgbox SixIsland_PatternBush_Text_GarretPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Jonah::
	trainerbattle_single TRAINER_BUG_CATCHER_JONAH, SixIsland_PatternBush_Text_JonahIntro, SixIsland_PatternBush_Text_JonahDefeat
	msgbox SixIsland_PatternBush_Text_JonahPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Vance::
	trainerbattle_single TRAINER_BUG_CATCHER_VANCE, SixIsland_PatternBush_Text_VanceIntro, SixIsland_PatternBush_Text_VanceDefeat
	msgbox SixIsland_PatternBush_Text_VancePostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Nash::
	trainerbattle_single TRAINER_YOUNGSTER_NASH, SixIsland_PatternBush_Text_NashIntro, SixIsland_PatternBush_Text_NashDefeat
	msgbox SixIsland_PatternBush_Text_NashPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Cordell::
	trainerbattle_single TRAINER_YOUNGSTER_CORDELL, SixIsland_PatternBush_Text_CordellIntro, SixIsland_PatternBush_Text_CordellDefeat
	msgbox SixIsland_PatternBush_Text_CordellPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Dalia::
	trainerbattle_single TRAINER_LASS_DALIA, SixIsland_PatternBush_Text_DaliaIntro, SixIsland_PatternBush_Text_DaliaDefeat
	msgbox SixIsland_PatternBush_Text_DaliaPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Joana::
	trainerbattle_single TRAINER_LASS_JOANA, SixIsland_PatternBush_Text_JoanaIntro, SixIsland_PatternBush_Text_JoanaDefeat
	msgbox SixIsland_PatternBush_Text_JoanaPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Riley::
	trainerbattle_single TRAINER_CAMPER_RILEY, SixIsland_PatternBush_Text_RileyIntro, SixIsland_PatternBush_Text_RileyDefeat
	msgbox SixIsland_PatternBush_Text_RileyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Marcy::
	trainerbattle_single TRAINER_PICNICKER_MARCY, SixIsland_PatternBush_Text_MarcyIntro, SixIsland_PatternBush_Text_MarcyDefeat
	msgbox SixIsland_PatternBush_Text_MarcyPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_EventScript_Layton::
	trainerbattle_single TRAINER_RUIN_MANIAC_LAYTON, SixIsland_PatternBush_Text_LaytonIntro, SixIsland_PatternBush_Text_LaytonDefeat
	msgbox SixIsland_PatternBush_Text_LaytonPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_PatternBush_Text_BethanyIntro::
	.string "I only battle people with style.\n"
	.string "You'll do.$"

SixIsland_PatternBush_Text_BethanyDefeat::
	.string "You raise your POKéMON with a\n"
	.string "whole lot of love!$"

SixIsland_PatternBush_Text_BethanyPostBattle::
	.string "One day this mess will end. I'll be\n"
	.string "fabulous then, too.$"

SixIsland_PatternBush_Text_AllisonIntro::
	.string "Even in a gang war, one must look\n"
	.string "fabulous.$"

SixIsland_PatternBush_Text_AllisonDefeat::
	.string "My outfit! It's ruined!$"

SixIsland_PatternBush_Text_AllisonPostBattle::
	.string "Money buys safety from TEAM\n"
	.string "ROCKET.\p"
	.string "Usually.$"

SixIsland_PatternBush_Text_GarretIntro::
	.string "My big brother rides with TEAM\n"
	.string "ROCKET! I'm gonna be just like\p"
	.string "him!$"

SixIsland_PatternBush_Text_GarretDefeat::
	.string "Aww… There goes my lunch money.$"

SixIsland_PatternBush_Text_GarretPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "ROCKET.\p"
	.string "Or maybe your crew!$"

SixIsland_PatternBush_Text_JonahIntro::
	.string "ARCHER says kids can't fight. I'll\n"
	.string "prove him wrong!$"

SixIsland_PatternBush_Text_JonahDefeat::
	.string "Okay, okay! You win!$"

SixIsland_PatternBush_Text_JonahPostBattle::
	.string "Everybody's scared of ARCHER.\p"
	.string "Are you scared?$"

SixIsland_PatternBush_Text_VanceIntro::
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

SixIsland_PatternBush_Text_VanceDefeat::
	.string "Waaah! I'm telling my brother!$"

SixIsland_PatternBush_Text_VancePostBattle::
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

SixIsland_PatternBush_Text_NashIntro::
	.string "I'm the toughest kid on this road.\n"
	.string "Ask anyone!$"

SixIsland_PatternBush_Text_NashDefeat::
	.string "No fair! You're way too strong!$"

SixIsland_PatternBush_Text_NashPostBattle::
	.string "The road to the SEVII ISLANDS\n"
	.string "isn't safe.\p"
	.string "Watch out!$"

SixIsland_PatternBush_Text_CordellIntro::
	.string "I found a loot bag yesterday!\p"
	.string "Wanna fight for it?$"

SixIsland_PatternBush_Text_CordellDefeat::
	.string "Aww… There goes my lunch money.$"

SixIsland_PatternBush_Text_CordellPostBattle::
	.string "Ayup, you townies come around all\n"
	.string "so dandy with a hat that flashy…\p"
	.string "How about giving me that?$"

SixIsland_PatternBush_Text_DaliaIntro::
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

SixIsland_PatternBush_Text_DaliaDefeat::
	.string "Okay, okay! You win!$"

SixIsland_PatternBush_Text_DaliaPostBattle::
	.string "Can I join your crew when I'm\n"
	.string "older?$"

SixIsland_PatternBush_Text_JoanaIntro::
	.string "My big brother rides with TEAM\n"
	.string "ROCKET! I'm gonna be just like\p"
	.string "him!$"

SixIsland_PatternBush_Text_JoanaDefeat::
	.string "Waaah! I'm telling my brother!$"

SixIsland_PatternBush_Text_JoanaPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "ROCKET.\p"
	.string "Or maybe your crew!$"

SixIsland_PatternBush_Text_RileyIntro::
	.string "ARCHER says kids can't fight. I'll\n"
	.string "prove him wrong!$"

SixIsland_PatternBush_Text_RileyDefeat::
	.string "No fair! You're way too strong!$"

SixIsland_PatternBush_Text_RileyPostBattle::
	.string "Everybody's scared of ARCHER.\p"
	.string "Are you scared?$"

SixIsland_PatternBush_Text_MarcyIntro::
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

SixIsland_PatternBush_Text_MarcyDefeat::
	.string "It wasn't a bug.\n"
	.string "I cut my shin on some grass.$"

SixIsland_PatternBush_Text_MarcyPostBattle::
	.string "A little cut like that…\n"
	.string "A little spit's enough to cure it!$"

SixIsland_PatternBush_Text_LaytonIntro::
	.string "I sell info to every gang.\p"
	.string "Info on you is worth a lot!$"

SixIsland_PatternBush_Text_LaytonDefeat::
	.string "Error! That wasn't in my data!$"

SixIsland_PatternBush_Text_LaytonPostBattle::
	.string "Data says you'll beat ARCHER.\p"
	.string "Probably.$"

