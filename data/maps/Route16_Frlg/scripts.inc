Route16_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route16_OnResume
	map_script MAP_SCRIPT_ON_TRANSITION, Route16_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route16_OnWarp
	.byte 0

Route16_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, Route16_EventScript_RemoveSnorlax
	end

Route16_EventScript_RemoveSnorlax::
	removeobject VAR_LAST_TALKED
	return

Route16_OnTransition::
	call_if_eq VAR_MAP_SCENE_ROUTE16, 1, Route16_OnTransitionCyclingRoad
	end

Route16_OnTransitionCyclingRoad::
	setflag FLAG_SYS_ON_CYCLING_ROAD
	return

Route16_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route16_OnWarpCyclingRoad
	.2byte 0

Route16_OnWarpCyclingRoad::
	special ForcePlayerOntoBike
	end

Route16_EventScript_Snorlax::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_POKE_FLUTE, Route16_EventScript_SnorlaxNoPokeFlute
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route16_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
	setwildbattle SPECIES_SNORLAX, 30
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_16_SNORLAX
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route16_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route16_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route16_EventScript_FoughtSnorlax
	release
	end

Route16_EventScript_DontUsePokeFlute::
	release
	end

Route16_EventScript_FoughtSnorlax::
	msgbox Text_SnorlaxReturnedToMountains
	release
	end

Route16_EventScript_SnorlaxNoPokeFlute::
	msgbox Route16_Text_MonSprawledOutInSlumber
	release
	end

Route16_EventScript_CyclingRoadSign::
	msgbox Route16_Text_CyclingRoadSign, MSGBOX_SIGN
	end

Route16_EventScript_RouteSign::
	msgbox Route16_Text_RouteSign, MSGBOX_SIGN
	end

Route16_Text_LaoIntro::
	.string "Wrong turf, rookie. Nobody passes\n"
	.string "without a fight!$"

Route16_Text_LaoDefeat::
	.string "Tch… You hit hard.$"

Route16_Text_LaoPostBattle::
	.string "Respect. Not many beat me on my\n"
	.string "own road.$"

Route16_Text_KojiIntro::
	.string "ARCHIE said nobody passes. That\n"
	.string "means you!$"

Route16_Text_KojiDefeat::
	.string "Who are you, anyway?!$"

Route16_Text_KojiPostBattle::
	.string "Tell your crew I fought fair.\n"
	.string "Mostly.$"

Route16_Text_LukeIntro::
	.string "This road belongs to TEAM AQUA.\p"
	.string "Pay up or fight!$"

Route16_Text_LukeDefeat::
	.string "Tch… You hit hard.$"

Route16_Text_LukePostBattle::
	.string "Word travels fast.\p"
	.string "ARCHIE will hear about you.$"

Route16_Text_HideoIntro::
	.string "You walk like you own the place.\p"
	.string "Big mistake!$"

Route16_Text_HideoDefeat::
	.string "Fine! Keep your cash!$"

Route16_Text_HideoPostBattle::
	.string "Go on. This road's yours…\p"
	.string "for now.$"

Route16_Text_CamronIntro::
	.string "ARCHIE wants your money.\p"
	.string "And I want a fight!$"

Route16_Text_CamronDefeat::
	.string "Fine! Keep your cash!$"

Route16_Text_CamronPostBattle::
	.string "Next time I'll bring the whole of\n"
	.string "TEAM AQUA!$"

Route16_Text_RubenIntro::
	.string "Hey! You're in the wrong part of\n"
	.string "town!$"

Route16_Text_RubenDefeat::
	.string "ARCHIE is gonna kill me…$"

Route16_Text_RubenPostBattle::
	.string "You're making a name for yourself,\n"
	.string "boss.$"

Route16_Text_MonSprawledOutInSlumber::
	.string "A POKéMON is sprawled out in a\n"
	.string "deep, comfortable slumber. It\p"
	.string "blocks the whole road.$"

Route16_Text_CyclingRoadSign::
	.string "Enjoy the slope!\p"
	.string "CYCLING ROAD - TEAM AQUA TOLL$"

Route16_Text_RouteSign::
	.string "ROUTE 16\p"
	.string "CELADON CITY - FUCHSIA CITY$"

Route16_Text_JedIntro::
	.string "I found a loot bag yesterday!\p"
	.string "Wanna fight for it?$"

Route16_Text_JedDefeat::
	.string "Waaah! I'm telling my brother!$"

Route16_Text_JedPostBattle::
	.string "Don't tell my mom I battled you.$"

Route16_Text_JedNotEnoughMons::
	.string "JED: Just one POKéMON? We only\n"
	.string "fight two-on-two!$"

Route16_Text_LeaIntro::
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

Route16_Text_LeaDefeat::
	.string "No fair! You're way too strong!$"

Route16_Text_LeaPostBattle::
	.string "Can I join your crew when I'm\n"
	.string "older?$"

Route16_Text_LeaNotEnoughMons::
	.string "LEA: You don't have two POKéMON?\p"
	.string "Out here alone with one? That's\n"
	.string "brave. Or dumb.$"
