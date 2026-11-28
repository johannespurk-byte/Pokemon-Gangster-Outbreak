Route18_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route18_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Route18_OnWarp
	.byte 0

Route18_OnTransition::
	call_if_eq VAR_MAP_SCENE_ROUTE16, 1, Route18_OnTransitionCyclingRoad
	end

Route18_OnTransitionCyclingRoad::
	setflag FLAG_SYS_ON_CYCLING_ROAD
	return

Route18_OnWarp::
	map_script_2 VAR_MAP_SCENE_ROUTE16, 1, Route18_OnWarpCyclingRoad
	.2byte 0

Route18_OnWarpCyclingRoad::
	special ForcePlayerOntoBike
	end

Route18_EventScript_RouteSign::
	msgbox Route18_Text_RouteSign, MSGBOX_SIGN
	end

Route18_EventScript_CyclingRoadSign::
	msgbox Route18_Text_CyclingRoadSign, MSGBOX_SIGN
	end

Route18_Text_WiltonIntro::
	.string "I live out here, far from TEAM\n"
	.string "MAGMA.\p"
	.string "Leave me be! …Or battle!$"

Route18_Text_WiltonDefeat::
	.string "Whoa! Rolled right over me!$"

Route18_Text_WiltonPostBattle::
	.string "From up here I watch TEAM MAGMA\n"
	.string "come and go.$"

Route18_Text_RamiroIntro::
	.string "My birds see every gang that\n"
	.string "moves.\p"
	.string "And they saw you!$"

Route18_Text_RamiroDefeat::
	.string "My birds flew off…$"

Route18_Text_RamiroPostBattle::
	.string "The wild POKéMON are the only\n"
	.string "honest ones left.$"

Route18_Text_JacobIntro::
	.string "These hills are older than any\n"
	.string "gang.\p"
	.string "So am I!$"

Route18_Text_JacobDefeat::
	.string "Like a rockslide!$"

Route18_Text_JacobPostBattle::
	.string "Head to CELADON if you want\n"
	.string "trouble.$"

Route18_Text_RouteSign::
	.string "ROUTE 18\n"
	.string "CELADON CITY - FUCHSIA CITY$"

Route18_Text_CyclingRoadSign::
	.string "CYCLING ROAD\p"
	.string "No pedestrians permitted! - TEAM\n"
	.string "MAGMA$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route18_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE18_1, Route18_Text_GangTrainer1Intro, Route18_Text_GangTrainer1Defeat
	msgbox Route18_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route18_Text_GangTrainer1Intro:
	.string "MAXIE wants your money.\p"
	.string "And I want a fight!$"

Route18_Text_GangTrainer1Defeat:
	.string "Who are you, anyway?!$"

Route18_Text_GangTrainer1Post:
	.string "Next time I'll bring the whole of\n"
	.string "TEAM MAGMA!$"

Route18_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE18_2, Route18_Text_GangTrainer2Intro, Route18_Text_GangTrainer2Defeat
	msgbox Route18_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route18_Text_GangTrainer2Intro:
	.string "Out here, nobody pays the tolls of\n"
	.string "MAXIE!$"

Route18_Text_GangTrainer2Defeat:
	.string "Heh, you're tough as stone.$"

Route18_Text_GangTrainer2Post:
	.string "Rest a bit. The road ahead is\n"
	.string "rough.$"

