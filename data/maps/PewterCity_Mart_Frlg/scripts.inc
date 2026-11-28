PewterCity_Mart_Frlg_MapScripts::
	.byte 0

PewterCity_Mart_EventScript_Youngster::
	msgbox PewterCity_Mart_Text_BoughtWeirdFishFromShadyGuy, MSGBOX_NPC
	end

PewterCity_Mart_EventScript_Boy::
	msgbox PewterCity_Mart_Text_GoodThingsIfRaiseMonsDiligently, MSGBOX_NPC
	end

PewterCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	call EventScript_GangMart
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
PewterCity_Mart_Items::
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	.2byte ITEM_NONE
	release
	end

PewterCity_Mart_Text_BoughtWeirdFishFromShadyGuy::
	.string "A shady old guy sucked me into\n"
	.string "buying this weird fish POKéMON!\p"
	.string "It's totally weak and it cost ¥500!$"

PewterCity_Mart_Text_GoodThingsIfRaiseMonsDiligently::
	.string "Good things happen if you raise\n"
	.string "POKéMON well.\p"
	.string "Even the weak ones can scare a\n"
	.string "gang if you train them.$"

