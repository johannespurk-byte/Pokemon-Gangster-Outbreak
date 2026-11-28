.equ REQUIRED_CAUGHT_MONS, 30

Route11_EastEntrance_2F_Frlg_MapScripts::
	.byte 0

Route11_EastEntrance_2F_EventScript_LeftBinoculars::
	lockall
	goto_if_set FLAG_WOKE_UP_ROUTE_12_SNORLAX, Route11_EastEntrance_2F_EventScript_LeftBinocularsSnorlaxGone
	msgbox Route11_EastEntrance_2F_Text_BigMonAsleepOnRoad
	releaseall
	end

Route11_EastEntrance_2F_EventScript_LeftBinocularsSnorlaxGone::
	msgbox Route11_EastEntrance_2F_Text_WhatABreathtakingView
	releaseall
	end

Route11_EastEntrance_2F_EventScript_RightBinoculars::
	msgbox Route11_EastEntrance_2F_Text_RockTunnelGoodRouteToLavender, MSGBOX_SIGN
	end

Route11_EastEntrance_2F_EventScript_Turner::
	lock
	faceplayer
	setvar VAR_0x8008, INGAME_TRADE_NIDORINOA
	call EventScript_GetInGameTradeSpeciesInfo
	goto_if_set FLAG_DID_NINA_TRADE, Route11_EastEntrance_2F_EventScript_AlreadyTraded
	msgbox Trade_Text_LookingForMonWannaTradeForMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route11_EastEntrance_2F_EventScript_DeclineTrade
	call EventScript_ChooseMonForInGameTrade
	goto_if_ge VAR_0x8004, PARTY_SIZE, Route11_EastEntrance_2F_EventScript_DeclineTrade
	call EventScript_GetInGameTradeSpecies
	goto_if_ne VAR_RESULT, VAR_0x8009, Route11_EastEntrance_2F_EventScript_NotRequestedMon
	call EventScript_DoInGameTrade
	msgbox Trade_Text_HeyThanks
	setflag FLAG_DID_NINA_TRADE
	release
	end

Route11_EastEntrance_2F_EventScript_DeclineTrade::
	msgbox Trade_Text_AwwOhWell
	release
	end

Route11_EastEntrance_2F_EventScript_NotRequestedMon::
	bufferspeciesname STR_VAR_1, VAR_0x8009
	msgbox Trade_Text_WhatThatsNoMon
	release
	end

Route11_EastEntrance_2F_EventScript_AlreadyTraded::
	msgbox Trade_Text_IsntMyOldMonGreat
	release
	end

Route11_EastEntrance_2F_EventScript_Aide::
	lock
	faceplayer
	call Route11_EastEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_ITEMFINDER, Route11_EastEntrance_2F_EventScript_AlreadyGotItemfinder
	msgbox Route11_EastEntrance_2F_Text_GiveItemfinderIfCaught30, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetFrlgPokedexCount
	buffernumberstring STR_VAR_3, VAR_0x8006
	call Route11_EastEntrance_2F_EventScript_GetAideRequestInfo
	goto_if_lt VAR_0x8006, REQUIRED_CAUGHT_MONS, Aide_EventScript_HaventCaughtEnough
	msgbox Route11_EastEntrance_2F_Text_GreatHereYouGo
	checkitemspace ITEM_ITEMFINDER
	goto_if_eq VAR_RESULT, FALSE, Aide_EventScript_NoRoomForItem
	giveitem_msg Route11_EastEntrance_2F_Text_ReceivedItemfinderFromAide, ITEM_ITEMFINDER
	setflag FLAG_GOT_ITEMFINDER
	msgbox Route11_EastEntrance_2F_Text_ExplainItemfinder
	release
	end

Route11_EastEntrance_2F_EventScript_AlreadyGotItemfinder::
	msgbox Route11_EastEntrance_2F_Text_ExplainItemfinder
	release
	end

Route11_EastEntrance_2F_EventScript_GetAideRequestInfo::
	buffernumberstring STR_VAR_1, REQUIRED_CAUGHT_MONS
	bufferitemname STR_VAR_2, ITEM_ITEMFINDER
	return

Route11_EastEntrance_2F_Text_GiveItemfinderIfCaught30::
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

Route11_EastEntrance_2F_Text_GreatHereYouGo::
	.string "Great! You have caught or owned\n"
	.string "{STR_VAR_3} kinds of POKéMON!\p"
	.string "Here you go!$"

Route11_EastEntrance_2F_Text_ReceivedItemfinderFromAide::
	.string "{PLAYER} received the {STR_VAR_2}\n"
	.string "from the AIDE.$"

Route11_EastEntrance_2F_Text_ExplainItemfinder::
	.string "Some items lie hidden on the\n"
	.string "ground. Gang stashes, mostly.\p"
	.string "The ITEMFINDER detects hidden\n"
	.string "items close to you.\p"
	.string "It can't pinpoint them exactly.\p"
	.string "It shows the direction of the\n"
	.string "item.\p"
	.string "Then search the area by hand.$"

Route11_EastEntrance_2F_Text_BigMonAsleepOnRoad::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "A big POKéMON is asleep on the\n"
	.string "road! Even the gangs can't move\p"
	.string "it!$"

Route11_EastEntrance_2F_Text_WhatABreathtakingView::
	.string "Let's look through the\n"
	.string "binoculars…\p"
	.string "What a view! You can't even see\n"
	.string "the gang graffiti from here.$"

Route11_EastEntrance_2F_Text_RockTunnelGoodRouteToLavender::
	.string "Let's see what the binoculars have\n"
	.string "to show…\p"
	.string "To get to LAVENDER TOWN from\n"
	.string "CERULEAN CITY…\p"
	.string "ROCK TUNNEL appears to be a good\n"
	.string "route to take.$"

