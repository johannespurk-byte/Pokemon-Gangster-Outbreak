ThreeIsland_Mart_Frlg_MapScripts::
	.byte 0

ThreeIsland_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	call EventScript_GangMart
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
ThreeIsland_Mart_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_NONE
	release
	end

ThreeIsland_Mart_EventScript_Picnicker::
	msgbox ThreeIsland_Mart_Text_TrueThatCeldadonDeptStoreBigger, MSGBOX_NPC
	end

ThreeIsland_Mart_EventScript_BugCatcher::
	msgbox ThreeIsland_Mart_Text_PeopleHealWithBerriesFromForest, MSGBOX_NPC
	end

ThreeIsland_Mart_EventScript_Youngster::
	msgbox ThreeIsland_Mart_Text_BikersWereAboutToTrashMart, MSGBOX_NPC
	end

ThreeIsland_Mart_Text_TrueThatCeldadonDeptStoreBigger::
	.string "Is it true? The CELADON DEPT.\n"
	.string "STORE is huge?\p"
	.string "And AQUA and MAGMA fight over it?$"

ThreeIsland_Mart_Text_PeopleHealWithBerriesFromForest::
	.string "I buy medicine here sometimes.\p"
	.string "But BERRIES from the BERRY\n"
	.string "FOREST are free. No gang taxes!$"

ThreeIsland_Mart_Text_BikersWereAboutToTrashMart::
	.string "Those BIKERS were about to trash\n"
	.string "this POKéMON MART.\p"
	.string "Am I glad they decided to leave!$"

