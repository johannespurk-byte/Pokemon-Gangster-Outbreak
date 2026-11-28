SevenIsland_Mart_Frlg_MapScripts::
	.byte 0

SevenIsland_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	call EventScript_GangMart
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
SevenIsland_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_MAX_POTION
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

SevenIsland_Mart_EventScript_Lass::
	msgbox SevenIsland_Mart_Text_MonHavePersonalitiesOfTheirOwn, MSGBOX_NPC
	end

SevenIsland_Mart_EventScript_Hiker::
	msgbox SevenIsland_Mart_Text_PreparationsCompleteForRuins, MSGBOX_NPC
	end

SevenIsland_Mart_EventScript_Fisher::
	msgbox SevenIsland_Mart_Text_NeedToFishOnSevenIsland, MSGBOX_NPC
	end

SevenIsland_Mart_Text_MonHavePersonalitiesOfTheirOwn::
	.string "POKéMON have personalities of their\n"
	.string "own, just like people.\p"
	.string "My PIKACHU has a HASTY nature,\n"
	.string "so it grew to be a speedy POKéMON.$"

SevenIsland_Mart_Text_PreparationsCompleteForRuins::
	.string "Preparations complete! I'll\n"
	.string "explore the RUINS before TEAM\p"
	.string "ROCKET loots them.$"

SevenIsland_Mart_Text_NeedToFishOnSevenIsland::
	.string "I need to fish on SEVEN ISLAND to\n"
	.string "finish my fishing tour.\p"
	.string "Better than fishing in KANTO's\n"
	.string "polluted harbors!$"

