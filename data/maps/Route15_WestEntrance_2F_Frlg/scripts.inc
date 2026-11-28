.equ REQUIRED_CAUGHT_MONS, 50

Route15_WestEntrance_2F_Frlg_MapScripts::
	.byte 0

Route15_WestEntrance_2F_EventScript_LeftBinoculars::
	lockall
	msgbox Route15_WestEntrance_2F_Text_LargeShiningBird
	showmonpic SPECIES_ARTICUNO, 10, 3
	delay 20
	waitbuttonpress
	hidemonpic
	setvar VAR_0x8004, SPECIES_ARTICUNO
	special SetSeenMon
	releaseall
	end

Route15_WestEntrance_2F_EventScript_RightBinoculars::
	msgbox Route15_WestEntrance_2F_Text_SmallIslandOnHorizon, MSGBOX_SIGN
	end

Route15_WestEntrance_2F_EventScript_Aide::
	lock
	faceplayer
	call Route15_WestEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_EXP_SHARE_FROM_OAKS_AIDE, Route15_WestEntrance_2F_EventScript_AlreadyGotExpShare
	msgbox Route15_WestEntrance_2F_Text_GiveItemIfCaughtEnough, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route15_WestEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_CAUGHT_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route15_WestEntrance_2F_Text_GreatHereYouGo
	checkitemspace ITEM_EXP_SHARE
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route15_WestEntrance_2F_Text_ReceivedItemFromAide, ITEM_EXP_SHARE
	setflag FLAG_GOT_EXP_SHARE_FROM_OAKS_AIDE
	msgbox Route15_WestEntrance_2F_Text_ExplainExpShare
	release
	end

Route15_WestEntrance_2F_EventScript_AlreadyGotExpShare::
	msgbox Route15_WestEntrance_2F_Text_ExplainExpShare
	release
	end

Route15_WestEntrance_2F_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_CAUGHT_MONS
	bufferitemname STR_VAR_2, ITEM_EXP_SHARE
	return

Route15_WestEntrance_2F_Text_GiveItemIfCaughtEnough::
	.string "Hi! I'm one of PROF. OAK's AIDES.\p"
	.string "If your POKéDEX has data on\n"
	.string "{STR_VAR_1} species, I have a\p"
	.string "reward.\p"
	.string "PROF. OAK left the {STR_VAR_2}\n"
	.string "for you.\p"
	.string "So, {PLAYER}, let me ask you.\p"
	.string "Have you gathered data on at\n"
	.string "least {STR_VAR_1} kinds of\p"
	.string "POKéMON?$"

Route15_WestEntrance_2F_Text_GreatHereYouGo::
	.string "Great! You have caught or owned\n"
	.string "{STR_VAR_3} kinds of POKéMON!\p"
	.string "Here you go!$"

Route15_WestEntrance_2F_Text_ReceivedItemFromAide::
	.string "{PLAYER} received the {STR_VAR_2}\n"
	.string "from the AIDE.$"

Route15_WestEntrance_2F_Text_ExplainExpShare::
	.string "A POKéMON holds the EXP. SHARE.\p"
	.string "It gets EXP. Points without\n"
	.string "fighting. Like a gang boss.$"

Route15_WestEntrance_2F_Text_LargeShiningBird::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "A large, shining bird is flying\n"
	.string "toward the sea!$"

Route15_WestEntrance_2F_Text_SmallIslandOnHorizon::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "A small island on the horizon! TEAM\n"
	.string "PLASMA boats are heading there.$"

