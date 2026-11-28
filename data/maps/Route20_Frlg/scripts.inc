Route20_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route20_OnTransition
	.byte 0

Route20_OnTransition::
	call_if_unset FLAG_STOPPED_SEAFOAM_B3F_CURRENT, Route20_EventScript_ResetSeafoamBouldersForB3F
	call_if_unset FLAG_STOPPED_SEAFOAM_B4F_CURRENT, Route20_EventScript_ResetSeafoamBouldersForB4F
	end

Route20_EventScript_ResetSeafoamBouldersForB3F::
	clearflag FLAG_HIDE_SEAFOAM_1F_BOULDER_1
	clearflag FLAG_HIDE_SEAFOAM_1F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B1F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B1F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B2F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B2F_BOULDER_2
	setflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_2
	return

Route20_EventScript_ResetSeafoamBouldersForB4F::
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_3
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_4
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_5
	clearflag FLAG_HIDE_SEAFOAM_B3F_BOULDER_6
	setflag FLAG_HIDE_SEAFOAM_B4F_BOULDER_1
	setflag FLAG_HIDE_SEAFOAM_B4F_BOULDER_2
	return

Route20_EventScript_SeafoamIslandsSign::
	msgbox Route20_Text_SeafoamIslands, MSGBOX_SIGN
	end

Route20_Text_BarryIntro::
	.string "I patrol these waves for TEAM\n"
	.string "GALACTIC!$"

Route20_Text_BarryDefeat::
	.string "Gah! Swallowed water!$"

Route20_Text_BarryPostBattle::
	.string "Ships from CINNABAR pass here\n"
	.string "every night.$"

Route20_Text_ShirleyIntro::
	.string "I patrol these waves for TEAM\n"
	.string "GALACTIC!$"

Route20_Text_ShirleyDefeat::
	.string "Sunk! Totally sunk!$"

Route20_Text_ShirleyPostBattle::
	.string "Ships from CINNABAR pass here\n"
	.string "every night.$"

Route20_Text_TiffanyIntro::
	.string "You'll sink like the last punk who\n"
	.string "came here!$"

Route20_Text_TiffanyDefeat::
	.string "The tide turned on me…$"

Route20_Text_TiffanyPostBattle::
	.string "Swim fast if you see a boat from\n"
	.string "TEAM GALACTIC.$"

Route20_Text_IreneIntro::
	.string "I found a loot bag yesterday!\p"
	.string "Wanna fight for it?$"

Route20_Text_IreneDefeat::
	.string "Waaah! I'm telling my brother!$"

Route20_Text_IrenePostBattle::
	.string "Don't tell my mom I battled you.$"

Route20_Text_DeanIntro::
	.string "TEAM GALACTIC took the docks, so\n"
	.string "I swim my own routes!$"

Route20_Text_DeanDefeat::
	.string "Glub… I'm all washed up.$"

Route20_Text_DeanPostBattle::
	.string "Even TEAM GALACTIC can't control\n"
	.string "the sea.$"

Route20_Text_DarrinIntro::
	.string "The sea doesn't care about gangs.\p"
	.string "But I do!$"

Route20_Text_DarrinDefeat::
	.string "Sunk! Totally sunk!$"

Route20_Text_DarrinPostBattle::
	.string "Smugglers use these waters at\n"
	.string "night. Stay sharp.$"

Route20_Text_RogerIntro::
	.string "Gangs come and go. The mountains\n"
	.string "stay!$"

Route20_Text_RogerDefeat::
	.string "Like a rockslide!$"

Route20_Text_RogerPostBattle::
	.string "Gangs never climb this far. Too\n"
	.string "lazy.$"

Route20_Text_NoraIntro::
	.string "Out here, only the strong stay\n"
	.string "afloat!$"

Route20_Text_NoraDefeat::
	.string "Gah! Swallowed water!$"

Route20_Text_NoraPostBattle::
	.string "The water's the only free place\n"
	.string "left.$"

Route20_Text_MissyIntro::
	.string "I'm the toughest kid on this road.\n"
	.string "Ask anyone!$"

Route20_Text_MissyDefeat::
	.string "Okay, okay! You win!$"

Route20_Text_MissyPostBattle::
	.string "The road to CINNABAR isn't safe.\p"
	.string "Watch out!$"

Route20_Text_MelissaIntro::
	.string "Splash! Nobody swims past me!$"

Route20_Text_MelissaDefeat::
	.string "Glub… I'm all washed up.$"

Route20_Text_MelissaPostBattle::
	.string "The currents carry loot bags. Keep\n"
	.string "your eyes open.$"

Route20_Text_SeafoamIslands::
	.string "SEAFOAM ISLANDS$"

Route20_Text_MistyTrainsHere::
	.string "Strong TRAINERS and WATER\n"
	.string "POKéMON are common here.\p"
	.string "They say TEAM GALACTIC ships\n"
	.string "pass by at night.$"
