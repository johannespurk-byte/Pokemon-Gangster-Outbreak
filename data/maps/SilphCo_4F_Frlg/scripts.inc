SilphCo_4F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_4F_OnLoad
	.byte 0

SilphCo_4F_OnLoad::
	call_if_unset FLAG_SILPH_4F_DOOR_1, EventScript_Close4FDoor1
	call_if_unset FLAG_SILPH_4F_DOOR_2, EventScript_Close4FDoor2
	end

SilphCo_4F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_4F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_4F_Text_CantYouSeeImHiding
	release
	end

SilphCo_4F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_4F_Text_TeamRocketIsGone
	release
	end

SilphCo_4F_EventScript_FloorSign::
	msgbox SilphCo_4F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_4F_EventScript_Rodney::
	trainerbattle_single TRAINER_SCIENTIST_RODNEY, SilphCo_4F_Text_RodneyIntro, SilphCo_4F_Text_RodneyDefeat
	msgbox SilphCo_4F_Text_RodneyPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_4F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_26, SilphCo_4F_Text_Grunt1Intro, SilphCo_4F_Text_Grunt1Defeat
	msgbox SilphCo_4F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_4F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_27, SilphCo_4F_Text_Grunt2Intro, SilphCo_4F_Text_Grunt2Defeat
	msgbox SilphCo_4F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_4F_Text_CantYouSeeImHiding::
	.string "Sssh!\n"
	.string "Can't you see I'm hiding?$"

SilphCo_4F_Text_TeamRocketIsGone::
	.string "Huh?\n"
	.string "TEAM ROCKET is gone?$"

SilphCo_4F_Text_Grunt1Intro::
	.string "Another crew poking around here?\n"
	.string "Big mistake!$"

SilphCo_4F_Text_Grunt1Defeat::
	.string "How can a kid be this strong?!$"

SilphCo_4F_Text_Grunt1PostBattle::
	.string "You'd make a great grunt. Ever\n"
	.string "thought about it?$"

SilphCo_4F_Text_RodneyIntro::
	.string "I built a radar for gang\n"
	.string "movements. It beeped at you!$"

SilphCo_4F_Text_RodneyDefeat::
	.string "Impossible! I calculated\n"
	.string "everything!$"

SilphCo_4F_Text_RodneyPostBattle::
	.string "The doors are electronically\n"
	.string "locked.\p"
	.string "It takes a CARD KEY to open the\n"
	.string "doors here.$"

SilphCo_4F_Text_Grunt2Intro::
	.string "I guard this place for GIOVANNI.\n"
	.string "Prepare yourself!$"

SilphCo_4F_Text_Grunt2Defeat::
	.string "Urgh! Not again!$"

SilphCo_4F_Text_Grunt2PostBattle::
	.string "Don't tell GIOVANNI I lost to you.$"

SilphCo_4F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "4F$"

