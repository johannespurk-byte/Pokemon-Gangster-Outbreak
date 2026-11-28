Route16_House_Frlg_MapScripts::
	.byte 0

Route16_House_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM02, Route16_House_EventScript_AlreadyGotHM02
	msgbox Route16_House_Text_FoundMySecretRetreat
	checkitemspace ITEM_HM02
	goto_if_eq VAR_RESULT, FALSE, Route16_House_EventScript_NoRoomForHM02
	checkitem ITEM_HM02
	goto_if_eq VAR_RESULT, TRUE, Gang_Route16_House_SkipITEM_HM02
	giveitem_msg Route16_House_Text_ReceivedHM02FromGirl, ITEM_HM02
Gang_Route16_House_SkipITEM_HM02:
	msgbox Route16_House_Text_ExplainHM02
	setflag FLAG_GOT_HM02
	release
	end

Route16_House_EventScript_NoRoomForHM02::
	msgbox Route16_House_Text_DontHaveAnyRoomForThis
	release
	end

Route16_House_EventScript_AlreadyGotHM02::
	msgbox Route16_House_Text_ExplainHM02
	release
	end

Route16_House_EventScript_Fearow::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_FEAROW, CRY_MODE_NORMAL
	msgbox Route16_House_Text_Fearow
	waitmoncry
	release
	end

Route16_House_Text_FoundMySecretRetreat::
	.string "Oh, dear.\n"
	.string "You've found my secret retreat.\p"
	.string "Please don't tell anyone I'm here.\n"
	.string "I'll make it up to you with this!$"

Route16_House_Text_ReceivedHM02FromGirl::
	.string "{PLAYER} received HM02\n"
	.string "from the girl.$"

Route16_House_Text_ExplainHM02::
	.string "HM02 is FLY.\p"
	.string "With a BADGE and the HM in your\n"
	.string "BAG, you can fly away from any\p"
	.string "gang. Use it well.$"

Route16_House_Text_DontHaveAnyRoomForThis::
	.string "You don't have any room for this.$"

Route16_House_Text_Fearow::
	.string "FEAROW: Kyueen!$"

