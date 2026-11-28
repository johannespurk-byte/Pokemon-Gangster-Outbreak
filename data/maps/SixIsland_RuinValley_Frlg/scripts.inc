SixIsland_RuinValley_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SixIsland_RuinValley_OnLoad
	.byte 0

SixIsland_RuinValley_OnLoad::
	call_if_set FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE, SixIsland_RuinValley_EventScript_OpenDottedHoleDoor
	end

SixIsland_RuinValley_EventScript_OpenDottedHoleDoor::
	setmetatile 24, 24, METATILE_SeviiIslands67_DottedHoleDoor_Open, 0
	return

SixIsland_RuinValley_EventScript_Scientist::
	lock
	msgbox SixIsland_RuinValley_Text_CantFigureOutHowToGetInside
	applymovement LOCALID_RUIN_VALLEY_SCIENTIST, Common_Movement_FacePlayer
	waitmovement 0
	msgbox SixIsland_RuinValley_Text_IFoundThisPlace
	applymovement LOCALID_RUIN_VALLEY_SCIENTIST, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

SixIsland_RuinValley_EventScript_DottedHoleDoor::
	lockall
	goto_if_set FLAG_USED_CUT_ON_RUIN_VALLEY_BRAILLE, SixIsland_RuinValley_EventScript_DottedHoleDoorOpen
	msgbox SixIsland_RuinValley_Text_CheckDoorMoreThoroughly, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SixIsland_RuinValley_EventScript_IgnoreDottedHoleDoor
	msgbox SixIsland_RuinValley_Text_SeveralDotsOnTheDoor
	braillemsgbox Braille_Text_Cut
	releaseall
	end

SixIsland_RuinValley_EventScript_DottedHoleDoorOpen::
	msgbox SixIsland_RuinValley_Text_DoorIsOpen
	releaseall
	end

SixIsland_RuinValley_EventScript_IgnoreDottedHoleDoor::
	msgbox SixIsland_RuinValley_Text_LeftDoorAlone
	releaseall
	end

SixIsland_RuinValley_Text_CantFigureOutHowToGetInside::
	.string "I can't figure out how to get\n"
	.string "inside.$"

SixIsland_RuinValley_Text_IFoundThisPlace::
	.string "I found this place first.\p"
	.string "Don't look so envious. And don't\n"
	.string "tell the ROCKETS.$"

SixIsland_RuinValley_Text_CheckDoorMoreThoroughly::
	.string "The door doesn't budge at all.\n"
	.string "Check it more thoroughly?$"

SixIsland_RuinValley_Text_LeftDoorAlone::
	.string "{PLAYER} left the door alone.$"

SixIsland_RuinValley_Text_SeveralDotsOnTheDoor::
	.string "On closer inspection, there are\n"
	.string "several dots on the door…$"

SixIsland_RuinValley_Text_DoorIsOpen::
	.string "The door is open.$"

SixIsland_RuinValley_Text_StanlyIntro::
	.string "There appear to be many secrets\n"
	.string "still unsolved in this world.$"

SixIsland_RuinValley_Text_StanlyDefeat::
	.string "My theory… failed!$"

SixIsland_RuinValley_Text_StanlyPostBattle::
	.string "I suggest to you that you, too,\n"
	.string "give a thought to the secrets and\l"
	.string "mysteries of our world once a day.$"

SixIsland_RuinValley_Text_FosterIntro::
	.string "I hacked a PC of TEAM ROCKET once.\p"
	.string "Now I hide out here!$"

SixIsland_RuinValley_Text_FosterDefeat::
	.string "Impossible! I calculated\n"
	.string "everything!$"

SixIsland_RuinValley_Text_FosterPostBattle::
	.string "ARCHER pays well for smart people.\p"
	.string "Just saying.$"

SixIsland_RuinValley_Text_LarryIntro::
	.string "My calculations say you lose.\p"
	.string "Let's test it!$"

SixIsland_RuinValley_Text_LarryDefeat::
	.string "Fine… I'll recalculate.$"

SixIsland_RuinValley_Text_LarryPostBattle::
	.string "I'll sell your battle data to TEAM\n"
	.string "ROCKET.\p"
	.string "Kidding! Mostly.$"

SixIsland_RuinValley_Text_DarylIntro::
	.string "My birds see every gang that\n"
	.string "moves.\p"
	.string "And they saw you!$"

SixIsland_RuinValley_Text_DarylDefeat::
	.string "Heh, you're tough as stone.$"

SixIsland_RuinValley_Text_DarylPostBattle::
	.string "The wild POKéMON are the only\n"
	.string "honest ones left.$"

SixIsland_RuinValley_Text_HectorIntro::
	.string "I sell info to every gang.\p"
	.string "Info on you is worth a lot!$"

SixIsland_RuinValley_Text_HectorDefeat::
	.string "Error! That wasn't in my data!$"

SixIsland_RuinValley_Text_HectorPostBattle::
	.string "Data says you'll beat ARCHER.\p"
	.string "Probably.$"
