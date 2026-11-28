FiveIsland_Meadow_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, FiveIsland_Meadow_OnLoad
	.byte 0

FiveIsland_Meadow_OnLoad::
	call_if_set FLAG_UNLOCKED_ROCKET_WAREHOUSE, FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked
	end

FiveIsland_Meadow_EventScript_WarehouseDoor::
	lockall
	goto_if_set FLAG_UNLOCKED_ROCKET_WAREHOUSE, FiveIsland_Meadow_EventScript_WarehouseDoorAlreadyOpen
	goto_if_set FLAG_LEARNED_YES_NAH_CHANSEY, FiveIsland_Meadow_EventScript_OpenWarehouseDoor
	msgbox FiveIsland_Meadow_Text_EnteredPasswordAnotherNeeded
	releaseall
	end

FiveIsland_Meadow_EventScript_OpenWarehouseDoor::
	setflag FLAG_UNLOCKED_ROCKET_WAREHOUSE
	message FiveIsland_Meadow_Text_EnteredPasswordDoorOpened
	waitmessage
	delay 60
	call FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked
	special DrawWholeMapView
	playse SE_PIN
	waitse
	releaseall
	end

FiveIsland_Meadow_EventScript_WarehouseDoorAlreadyOpen::
	msgbox FiveIsland_Meadow_Text_WarehouseDoorAlreadyOpen
	releaseall
	end

FiveIsland_Meadow_EventScript_SetWarehouseDoorUnlocked::
	setmetatile 12, 21, METATILE_SeviiIslands45_RocketWarehouseDoor_Unlocked, 1
	return

FiveIsland_Meadow_EventScript_Rocket1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_49, FiveIsland_Meadow_Text_Rocket1Intro, FiveIsland_Meadow_Text_Rocket1Defeat
	msgbox FiveIsland_Meadow_Text_Rocket1PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_Meadow_EventScript_Rocket2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_50, FiveIsland_Meadow_Text_Rocket2Intro, FiveIsland_Meadow_Text_Rocket2Defeat
	msgbox FiveIsland_Meadow_Text_Rocket2PostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_Meadow_EventScript_Rocket3::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_51, FiveIsland_Meadow_Text_Rocket3Intro, FiveIsland_Meadow_Text_Rocket3Defeat
	msgbox FiveIsland_Meadow_Text_Rocket3PostBattle, MSGBOX_AUTOCLOSE
	end

@ Unused
FiveIsland_Meadow_Text_DoorNeedsTwoPasswords::
	.string "とびらを　あけるには\n"
	.string "パスワードが　2つ　いるようだ$"

FiveIsland_Meadow_Text_EnteredPasswordAnotherNeeded::
	.string "{PLAYER} entered a password.\p"
	.string "“GOLDEEN need log.”\p"
	.string "… …  … …  … …\p"
	.string "Another password is needed to\n"
	.string "open the door…$"

FiveIsland_Meadow_Text_EnteredPasswordDoorOpened::
	.string "{PLAYER} entered two passwords.\p"
	.string "“GOLDEEN need log.”\n"
	.string "“Yes, nah, CHANSEY.”\p"
	.string "… …  … …  … …\p"
	.string "The WAREHOUSE's door opened!$"

FiveIsland_Meadow_Text_WarehouseDoorAlreadyOpen::
	.string "The WAREHOUSE's door is already\n"
	.string "open.$"

FiveIsland_Meadow_Text_Rocket1Intro::
	.string "You're not one of us. Get lost or\n"
	.string "get hurt!$"

FiveIsland_Meadow_Text_Rocket1Defeat::
	.string "Retreat! Retreat!$"

FiveIsland_Meadow_Text_Rocket1PostBattle::
	.string "I only joined TEAM ROCKET for the\n"
	.string "money…$"

FiveIsland_Meadow_Text_Rocket2Intro::
	.string "Another crew poking around here?\n"
	.string "Big mistake!$"

FiveIsland_Meadow_Text_Rocket2Defeat::
	.string "How can a kid be this strong?!$"

FiveIsland_Meadow_Text_Rocket2PostBattle::
	.string "You'd make a great grunt. Ever\n"
	.string "thought about it?$"

FiveIsland_Meadow_Text_Rocket3Intro::
	.string "I guard this place for ARCHER.\n"
	.string "Prepare yourself!$"

FiveIsland_Meadow_Text_Rocket3Defeat::
	.string "Urgh! Not again!$"

FiveIsland_Meadow_Text_Rocket3PostBattle::
	.string "You won't be able to get in without\n"
	.string "the passwords anyway.$"

