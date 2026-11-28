SilphCo_2F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_2F_OnLoad
	.byte 0

SilphCo_2F_OnLoad::
	call_if_unset FLAG_SILPH_2F_DOOR_1, EventScript_Close2FDoor1
	call_if_unset FLAG_SILPH_2F_DOOR_2, EventScript_Close2FDoor2
	end

SilphCo_2F_EventScript_ThunderWaveTutor::
	goto EventScript_ThunderWaveTutor
	end

SilphCo_2F_EventScript_FloorSign::
	msgbox SilphCo_2F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_2F_EventScript_Connor::
	trainerbattle_single TRAINER_SCIENTIST_CONNOR, SilphCo_2F_Text_ConnorIntro, SilphCo_2F_Text_ConnorDefeat
	msgbox SilphCo_2F_Text_ConnorPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_EventScript_Jerry::
	trainerbattle_single TRAINER_SCIENTIST_JERRY, SilphCo_2F_Text_JerryIntro, SilphCo_2F_Text_JerryDefeat
	msgbox SilphCo_2F_Text_JerryPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_23, SilphCo_2F_Text_Grunt1Intro, SilphCo_2F_Text_Grunt1Defeat
	msgbox SilphCo_2F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_24, SilphCo_2F_Text_Grunt2Intro, SilphCo_2F_Text_Grunt2Defeat
	msgbox SilphCo_2F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_2F_Text_ConnorIntro::
	.string "Science doesn't stop for a gang\n"
	.string "war!$"

SilphCo_2F_Text_ConnorDefeat::
	.string "Fine… I'll recalculate.$"

SilphCo_2F_Text_ConnorPostBattle::
	.string "Knowledge is the only thing they\n"
	.string "can't steal.$"

SilphCo_2F_Text_JerryIntro::
	.string "I built a radar for gang\n"
	.string "movements. It beeped at you!$"

SilphCo_2F_Text_JerryDefeat::
	.string "Error! That wasn't in my data!$"

SilphCo_2F_Text_JerryPostBattle::
	.string "Every gang uses my gadgets.\n"
	.string "Nobody pays.$"

SilphCo_2F_Text_Grunt1Intro::
	.string "You're not one of us. Get lost or\n"
	.string "get hurt!$"

SilphCo_2F_Text_Grunt1Defeat::
	.string "Retreat! Retreat!$"

SilphCo_2F_Text_Grunt1PostBattle::
	.string "Diamond-shaped tiles are teleport\n"
	.string "blocks.\p"
	.string "They're transporters for this\n"
	.string "high-tech building.$"

SilphCo_2F_Text_Grunt2Intro::
	.string "Another crew poking around here?\n"
	.string "Big mistake!$"

SilphCo_2F_Text_Grunt2Defeat::
	.string "How can a kid be this strong?!$"

SilphCo_2F_Text_Grunt2PostBattle::
	.string "You'd make a great grunt. Ever\n"
	.string "thought about it?$"

Text_ThunderWaveTeach::
	.string "Eeek!\n"
	.string "No! Stop! Help!\p"
	.string "Oh, you're not with TEAM ROCKET.\n"
	.string "I'm sorry, I thought…\p"
	.string "Will you forgive me if I teach\n"
	.string "you the THUNDER WAVE technique?$"

Text_ThunderWaveDeclined::
	.string "Oh…\n"
	.string "But THUNDER WAVE is convenient…$"

Text_ThunderWaveWhichMon::
	.string "Which POKéMON should I teach\n"
	.string "THUNDER WAVE?$"

Text_ThunderWaveTaught::
	.string "THUNDER WAVE causes paralysis in\n"
	.string "the target POKéMON.\p"
	.string "A paralyzed POKéMON may be unable\n"
	.string "to move in battle.\p"
	.string "It's an ELECTRIC-type move, so it\n"
	.string "won't work on some POKéMON.$"

SilphCo_2F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "2F$"

