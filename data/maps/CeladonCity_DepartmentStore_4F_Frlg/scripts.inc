CeladonCity_DepartmentStore_4F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_4F_EventScript_Man::
	msgbox CeladonCity_DepartmentStore_4F_Text_GettingPokeDollAsPresent, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_Youngster::
	msgbox CeladonCity_DepartmentStore_4F_Text_CanRunAwayWithPokeDoll, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_4F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_4F_Text_FloorSign, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_4F_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart CeladonCity_DepartmentStore_4F_Items
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
CeladonCity_DepartmentStore_4F_Items::
	.2byte ITEM_POKE_DOLL
	.2byte ITEM_RETRO_MAIL
	.2byte ITEM_FIRE_STONE
	.2byte ITEM_THUNDER_STONE
	.2byte ITEM_WATER_STONE
	.2byte ITEM_LEAF_STONE
	.2byte ITEM_NONE
	release
	end

CeladonCity_DepartmentStore_4F_Text_GettingPokeDollAsPresent::
	.string "I'm buying a present for my\n"
	.string "girlfriend.\p"
	.string "A POKé DOLL. Nothing says\n"
	.string "“I survived” like a doll.$"

CeladonCity_DepartmentStore_4F_Text_CanRunAwayWithPokeDoll::
	.string "I heard something useful.\p"
	.string "If a wild POKéMON appears, you can\n"
	.string "distract it with a POKé DOLL.\p"
	.string "You can run away while the wild\n"
	.string "POKéMON is distracted.$"

CeladonCity_DepartmentStore_4F_Text_FloorSign::
	.string "Gifts for the hard times!\n"
	.string "4F: WISE MAN GIFTS\p"
	.string "Evolution Special!\n"
	.string "STONES while stocks last!$"
