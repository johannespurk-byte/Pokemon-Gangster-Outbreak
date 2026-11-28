Route12_NorthEntrance_2F_Frlg_MapScripts::
	.byte 0

Route12_NorthEntrance_2F_EventScript_LeftBinoculars::
	msgbox Route12_NorthEntrance_2F_Text_TheresManFishing, MSGBOX_SIGN
	end

Route12_NorthEntrance_2F_EventScript_RightBinoculars::
	msgbox Route12_NorthEntrance_2F_Text_ItsPokemonTower, MSGBOX_SIGN
	end

Route12_NorthEntrance_2F_EventScript_Lass::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM27, Route12_NorthEntrance_2F_EventScript_ExplainTM27
	checkplayergender
	call_if_eq VAR_RESULT, MALE, Route12_NorthEntrance_2F_EventScript_TakeTMMale
	call_if_eq VAR_RESULT, FEMALE, Route12_NorthEntrance_2F_EventScript_TakeTMFemale
	checkitemspace ITEM_TM27
	goto_if_eq VAR_RESULT, FALSE, Route12_NorthEntrance_2F_EventScript_NoRoomForTM27
	giveitem_msg Route12_NorthEntrance_2F_Text_ReceivedTM27FromLittleGirl, ITEM_TM27
	msgbox Route12_NorthEntrance_2F_Text_ExplainTM27
	setflag FLAG_GOT_TM27
	release
	end

Route12_NorthEntrance_2F_EventScript_TakeTMMale::
	msgbox Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreMale
	return

Route12_NorthEntrance_2F_EventScript_TakeTMFemale::
	msgbox Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreFemale
	return

Route12_NorthEntrance_2F_EventScript_NoRoomForTM27::
	msgbox Route12_NorthEntrance_2F_Text_DontHaveRoomForThis
	release
	end

Route12_NorthEntrance_2F_EventScript_ExplainTM27::
	msgbox Route12_NorthEntrance_2F_Text_ExplainTM27
	release
	end

@ Male and female text identical, differ (presumably) in JP
Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreMale::
	.string "My POKéMON rests in POKéMON\n"
	.string "TOWER. The ROCKETS…\p"
	.string "You can have this TM. I don't need\n"
	.string "it anymore…$"

Route12_NorthEntrance_2F_Text_TakeTMDontNeedAnymoreFemale::
	.string "My POKéMON rests in POKéMON\n"
	.string "TOWER. The ROCKETS…\p"
	.string "You can have this TM. I don't need\n"
	.string "it anymore…$"

Route12_NorthEntrance_2F_Text_ReceivedTM27FromLittleGirl::
	.string "{PLAYER} received TM27\n"
	.string "from the little girl.$"

Route12_NorthEntrance_2F_Text_ExplainTM27::
	.string "TM27 is called RETURN…\p"
	.string "If you treat your POKéMON well, it\n"
	.string "will fight its hardest for you.\p"
	.string "Please do.$"

Route12_NorthEntrance_2F_Text_DontHaveRoomForThis::
	.string "You don't have room for this.$"

Route12_NorthEntrance_2F_Text_TheresManFishing::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "There's a man fishing! Lucky guy.$"

Route12_NorthEntrance_2F_Text_ItsPokemonTower::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "It's POKéMON TOWER! Something is\n"
	.string "moving up there…$"

