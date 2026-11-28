SilphCo_9F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_9F_OnLoad
	.byte 0

SilphCo_9F_OnLoad::
	call_if_unset FLAG_SILPH_9F_DOOR_1, EventScript_Close9FDoor1
	call_if_unset FLAG_SILPH_9F_DOOR_2, EventScript_Close9FDoor2
	call_if_unset FLAG_SILPH_9F_DOOR_3, EventScript_Close9FDoor3
	call_if_unset FLAG_SILPH_9F_DOOR_4, EventScript_Close9FDoor4
	end

SilphCo_9F_EventScript_HealWoman::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_9F_EventScript_HealWomanRocketsGone
	msgbox SilphCo_9F_Text_YouShouldTakeQuickNap
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox SilphCo_9F_Text_DontGiveUp
	release
	end

SilphCo_9F_EventScript_HealWomanRocketsGone::
	msgbox SilphCo_9F_Text_ThankYouSoMuch
	release
	end

SilphCo_9F_EventScript_FloorSign::
	msgbox SilphCo_9F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_9F_EventScript_Ed::
	trainerbattle_single TRAINER_SCIENTIST_ED, SilphCo_9F_Text_EdIntro, SilphCo_9F_Text_EdDefeat
	msgbox SilphCo_9F_Text_EdPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_9F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_37, SilphCo_9F_Text_Grunt1Intro, SilphCo_9F_Text_Grunt1Defeat
	msgbox SilphCo_9F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_9F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_38, SilphCo_9F_Text_Grunt2Intro, SilphCo_9F_Text_Grunt2Defeat
	msgbox SilphCo_9F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_9F_Text_YouShouldTakeQuickNap::
	.string "You look tired.\n"
	.string "You should take a quick nap.$"

SilphCo_9F_Text_DontGiveUp::
	.string "Don't give up!$"

SilphCo_9F_Text_ThankYouSoMuch::
	.string "Thank you so much.$"

SilphCo_9F_Text_Grunt1Intro::
	.string "Hand over your POKéMON. Orders\n"
	.string "from GIOVANNI!$"

SilphCo_9F_Text_Grunt1Defeat::
	.string "I'm gonna be in so much trouble…$"

SilphCo_9F_Text_Grunt1PostBattle::
	.string "Word is you're GIOVANNI's new\n"
	.string "star. Is that true?$"

SilphCo_9F_Text_EdIntro::
	.string "I sell info to every gang.\p"
	.string "Info on you is worth a lot!$"

SilphCo_9F_Text_EdDefeat::
	.string "Error! That wasn't in my data!$"

SilphCo_9F_Text_EdPostBattle::
	.string "Data says you'll beat GIOVANNI.\p"
	.string "Probably.$"

SilphCo_9F_Text_Grunt2Intro::
	.string "Halt! This is TEAM ROCKET\n"
	.string "business!$"

SilphCo_9F_Text_Grunt2Defeat::
	.string "Retreat! Retreat!$"

SilphCo_9F_Text_Grunt2PostBattle::
	.string "GIOVANNI will deal with you\n"
	.string "himself.$"

SilphCo_9F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "9F$"

