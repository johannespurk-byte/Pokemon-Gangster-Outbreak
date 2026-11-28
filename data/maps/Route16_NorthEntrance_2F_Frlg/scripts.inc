.equ REQUIRED_CAUGHT_MONS, 40

Route16_NorthEntrance_2F_Frlg_MapScripts::
	.byte 0

Route16_NorthEntrance_2F_EventScript_LittleBoy::
	msgbox Route16_NorthEntrance_2F_Text_OnBikeRideWithGirlfriend, MSGBOX_NPC
	end

Route16_NorthEntrance_2F_EventScript_LittleGirl::
	msgbox Route16_NorthEntrance_2F_Text_RidingTogetherOnNewBikes, MSGBOX_NPC
	end

Route16_NorthEntrance_2F_EventScript_LeftBinoculars::
	msgbox Route16_NorthEntrance_2F_Text_ItsCeladonDeptStore, MSGBOX_SIGN
	end

Route16_NorthEntrance_2F_EventScript_RightBinoculars::
	msgbox Route16_NorthEntrance_2F_Text_LongPathOverWater, MSGBOX_SIGN
	end

Route16_NorthEntrance_2F_EventScript_Aide::
	lock
	faceplayer
	call Route16_NorthEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_AMULET_COIN_FROM_OAKS_AIDE, Route16_NorthEntrance_2F_EventScript_AlreadyGotAmuletCoin
	msgbox Route16_NorthEntrance_2F_Text_GiveAmuletCoinIfCaught40, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route16_NorthEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_CAUGHT_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route16_NorthEntrance_2F_Text_GreatHereYouGo
	checkitemspace ITEM_AMULET_COIN
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route16_NorthEntrance_2F_Text_ReceivedAmuletCoinFromAide, ITEM_AMULET_COIN
	setflag FLAG_GOT_AMULET_COIN_FROM_OAKS_AIDE
	msgbox Route16_NorthEntrance_2F_Text_ExplainAmuletCoin
	release
	end

Route16_NorthEntrance_2F_EventScript_AlreadyGotAmuletCoin::
	msgbox Route16_NorthEntrance_2F_Text_ExplainAmuletCoin
	release
	end

Route16_NorthEntrance_2F_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_CAUGHT_MONS
	bufferitemname STR_VAR_2, ITEM_AMULET_COIN
	return

Route16_NorthEntrance_2F_Text_OnBikeRideWithGirlfriend::
	.string "I'm riding with my girlfriend.\p"
	.string "If we ride fast, the gangs can't\n"
	.string "catch us!$"

Route16_NorthEntrance_2F_Text_RidingTogetherOnNewBikes::
	.string "We stole these bikes back from\n"
	.string "TEAM SKULL!$"

Route16_NorthEntrance_2F_Text_ItsCeladonDeptStore::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "It's the CELADON DEPT. STORE!\n"
	.string "There's an AQUA flag on the roof!$"

Route16_NorthEntrance_2F_Text_LongPathOverWater::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "A long path over water. TEAM MAGMA\n"
	.string "patrols it.$"

Route16_NorthEntrance_2F_Text_GiveAmuletCoinIfCaught40::
	.string "Hi! I'm one of PROF. OAK's AIDES.\p"
	.string "If your POKéDEX has data on 40\n"
	.string "species, I have a reward.\p"
	.string "PROF. OAK left an AMULET COIN for\n"
	.string "you.\p"
	.string "So, {PLAYER}, let me ask you.\p"
	.string "Have you gathered data on at\n"
	.string "least 40 kinds of POKéMON?$"

Route16_NorthEntrance_2F_Text_GreatHereYouGo::
	.string "Great! You have caught or owned\n"
	.string "{STR_VAR_3} kinds of POKéMON!\p"
	.string "Here you go!$"

Route16_NorthEntrance_2F_Text_ReceivedAmuletCoinFromAide::
	.string "{PLAYER} received the AMULET COIN\n"
	.string "from the AIDE.$"

Route16_NorthEntrance_2F_Text_ExplainAmuletCoin::
	.string "A POKéMON holds the AMULET COIN.\p"
	.string "If it takes part in a winning\n"
	.string "battle, you earn more money.\p"
	.string "Gangster money!$"

