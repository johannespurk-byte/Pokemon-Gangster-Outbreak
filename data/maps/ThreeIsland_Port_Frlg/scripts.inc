ThreeIsland_Port_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ThreeIsland_Port_OnTransition
	.byte 0

ThreeIsland_Port_OnTransition::
	clearflag FLAG_SYS_PC_STORAGE_DISABLED
	setflag FLAG_SEVII_DETOUR_FINISHED
	end

ThreeIsland_Port_EventScript_Woman::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_TWO_ISLAND_JOYFUL_GAME_CORNER, 2, ThreeIsland_Port_EventScript_WomanLostelleFound
	goto_if_ge VAR_MAP_SCENE_THREE_ISLAND, 4, ThreeIsland_Port_EventScript_WomanBikersGone
	msgbox ThreeIsland_Port_Text_IllCallThePolice
	release
	end

ThreeIsland_Port_EventScript_WomanLostelleFound::
	msgbox ThreeIsland_Port_Text_EverythingTurnedOutForBest
	release
	end

ThreeIsland_Port_EventScript_WomanBikersGone::
	msgbox ThreeIsland_Port_Text_ThankGoodnessBikersGone
	release
	end

ThreeIsland_Port_EventScript_Biker1::
	msgbox ThreeIsland_Port_Text_WereKantoRiderFederation, MSGBOX_NPC
	end

ThreeIsland_Port_EventScript_Biker2::
	lock
	msgbox ThreeIsland_Port_Text_ForkOverMoney
	release
	end

ThreeIsland_Port_Text_WereKantoRiderFederation::
	.string "We're the KANTO RIDER\n"
	.string "FEDERATION!\p"
	.string "Vroom! KANTO got too hot with all\n"
	.string "the gangs, so this island is ours\p"
	.string "now!$"

ThreeIsland_Port_Text_ForkOverMoney::
	.string "Hey, hey, lady! Fork over some\n"
	.string "spending money! Protection fee!$"

ThreeIsland_Port_Text_IllCallThePolice::
	.string "Aiyeeh!\p"
	.string "Wh-who are you?! I'll call the\n"
	.string "police! If there still are any!$"

ThreeIsland_Port_Text_ThankGoodnessBikersGone::
	.string "Those BIKERS… Are they gone?\p"
	.string "Oh, thank goodness! They\n"
	.string "terrified me!$"

ThreeIsland_Port_Text_EverythingTurnedOutForBest::
	.string "The BIKERS ran off, and LOSTELLE\n"
	.string "was found safe.\p"
	.string "Everything turned out fine! Even\n"
	.string "with a KANTO gangster around.$"

