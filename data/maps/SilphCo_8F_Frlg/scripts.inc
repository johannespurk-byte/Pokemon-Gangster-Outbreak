SilphCo_8F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_8F_OnLoad
	.byte 0

SilphCo_8F_OnLoad::
	call_if_unset FLAG_SILPH_8F_DOOR, EventScript_Close8FDoor
	end

SilphCo_8F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_8F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_8F_Text_WonderIfSilphIsFinished
	release
	end

SilphCo_8F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_8F_Text_ThanksForSavingUs
	release
	end

SilphCo_8F_EventScript_FloorSign::
	msgbox SilphCo_8F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_8F_EventScript_Parker::
	trainerbattle_single TRAINER_SCIENTIST_PARKER, SilphCo_8F_Text_ParkerIntro, SilphCo_8F_Text_ParkerDefeat
	msgbox SilphCo_8F_Text_ParkerPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_8F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_32, SilphCo_8F_Text_Grunt1Intro, SilphCo_8F_Text_Grunt1Defeat
	msgbox SilphCo_8F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_8F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_36, SilphCo_8F_Text_Grunt2Intro, SilphCo_8F_Text_Grunt2Defeat
	msgbox SilphCo_8F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_8F_Text_WonderIfSilphIsFinished::
	.string "I wonder if SILPH is finished…$"

SilphCo_8F_Text_ThanksForSavingUs::
	.string "Thanks for saving us!$"

SilphCo_8F_Text_Grunt1Intro::
	.string "I guard this place for GIOVANNI.\n"
	.string "Prepare yourself!$"

SilphCo_8F_Text_Grunt1Defeat::
	.string "Retreat! Retreat!$"

SilphCo_8F_Text_Grunt1PostBattle::
	.string "Don't tell GIOVANNI I lost to you.$"

SilphCo_8F_Text_ParkerIntro::
	.string "My calculations say you lose.\p"
	.string "Let's test it!$"

SilphCo_8F_Text_ParkerDefeat::
	.string "Fine… I'll recalculate.$"

SilphCo_8F_Text_ParkerPostBattle::
	.string "I'll sell your battle data to TEAM\n"
	.string "ROCKET.\p"
	.string "Kidding! Mostly.$"

SilphCo_8F_Text_Grunt2Intro::
	.string "Hand over your POKéMON. Orders\n"
	.string "from GIOVANNI!$"

SilphCo_8F_Text_Grunt2Defeat::
	.string "How can a kid be this strong?!$"

SilphCo_8F_Text_Grunt2PostBattle::
	.string "Word is you're GIOVANNI's new\n"
	.string "star. Is that true?$"

SilphCo_8F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "8F$"

SilphCo_8F_Text_ToRocketBossMonsAreTools::
	.string "TEAM ROCKET's BOSS is terribly\n"
	.string "cruel!\p"
	.string "To him, POKéMON are just tools to\n"
	.string "be used.\p"
	.string "What will happen if that tyrant\n"
	.string "takes over our company…$"
