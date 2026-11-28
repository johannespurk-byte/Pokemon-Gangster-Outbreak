SilphCo_10F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_10F_OnLoad
	.byte 0

SilphCo_10F_OnLoad::
	call_if_unset FLAG_SILPH_10F_DOOR, EventScript_Close10FDoor
	end

SilphCo_10F_EventScript_WorkerF::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_10F_EventScript_WorkerFRocketsGone
	msgbox SilphCo_10F_Text_WaaaImScared
	release
	end

SilphCo_10F_EventScript_WorkerFRocketsGone::
	msgbox SilphCo_10F_Text_KeepMeCryingASecret
	release
	end

SilphCo_10F_EventScript_FloorSign::
	msgbox SilphCo_10F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_10F_EventScript_Travis::
	trainerbattle_single TRAINER_SCIENTIST_TRAVIS, SilphCo_10F_Text_TravisIntro, SilphCo_10F_Text_TravisDefeat
	msgbox SilphCo_10F_Text_TravisPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_10F_EventScript_Grunt::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_39, SilphCo_10F_Text_GruntIntro, SilphCo_10F_Text_GruntDefeat
	msgbox SilphCo_10F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_10F_Text_GruntIntro::
	.string "I guard this place for GIOVANNI.\n"
	.string "Prepare yourself!$"

SilphCo_10F_Text_GruntDefeat::
	.string "Urgh! Not again!$"

SilphCo_10F_Text_GruntPostBattle::
	.string "Don't tell GIOVANNI I lost to you.$"

SilphCo_10F_Text_TravisIntro::
	.string "I hacked a PC of TEAM ROCKET once.\p"
	.string "Now I hide out here!$"

SilphCo_10F_Text_TravisDefeat::
	.string "Error! That wasn't in my data!$"

SilphCo_10F_Text_TravisPostBattle::
	.string "GIOVANNI pays well for smart\n"
	.string "people.\p"
	.string "Just saying.$"

SilphCo_10F_Text_WaaaImScared::
	.string "Waaaaa! Two gangs in one building!\n"
	.string "I'm scared!$"

SilphCo_10F_Text_KeepMeCryingASecret::
	.string "About me crying…\n"
	.string "Please keep it a secret.$"

SilphCo_10F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "10F$"

