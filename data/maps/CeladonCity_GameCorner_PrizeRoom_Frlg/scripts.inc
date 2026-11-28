CeladonCity_GameCorner_PrizeRoom_Frlg_MapScripts::
	.byte 0

CeladonCity_GameCorner_PrizeRoom_EventScript_BaldingMan::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_FancyThatPorygon, MSGBOX_NPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_OldMan::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_RakedItInToday, MSGBOX_NPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkMons::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_COIN_CASE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	showcoinsbox 0, 0
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeMon::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTI_GAME_CORNER_POKEMON_PRIZES, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_Latias
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_Latios
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_Diancie
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_Rayquaza
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_Mewtwo
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange::
	hidecoinsbox
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Latias::
	setvar VAR_TEMP_1, SPECIES_LATIAS
	setvar VAR_TEMP_2, 3500
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Latios::
	setvar VAR_TEMP_1, SPECIES_LATIOS
	setvar VAR_TEMP_2, 3500
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Diancie::
	setvar VAR_TEMP_1, SPECIES_DIANCIE
	setvar VAR_TEMP_2, 5000
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Rayquaza::
	setvar VAR_TEMP_1, SPECIES_RAYQUAZA
	setvar VAR_TEMP_2, 7500
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Mewtwo::
	setvar VAR_TEMP_1, SPECIES_MEWTWO
	setvar VAR_TEMP_2, 9999
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeMon::
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_YouWantPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	checkcoins VAR_RESULT
	goto_if_lt VAR_RESULT, VAR_TEMP_2, CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins
	textcolor NPC_TEXT_COLOR_NEUTRAL
	switch VAR_TEMP_1
	case SPECIES_LATIAS, CeladonCity_GameCorner_PrizeRoom_EventScript_GiveLatias
	case SPECIES_LATIOS, CeladonCity_GameCorner_PrizeRoom_EventScript_GiveLatios
	case SPECIES_DIANCIE, CeladonCity_GameCorner_PrizeRoom_EventScript_GiveDiancie
	case SPECIES_RAYQUAZA, CeladonCity_GameCorner_PrizeRoom_EventScript_GiveRayquaza
	case SPECIES_MEWTWO, CeladonCity_GameCorner_PrizeRoom_EventScript_GiveMewtwo
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveLatias::
	givemon VAR_TEMP_1, 35, ITEM_LATIASITE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveLatios::
	givemon VAR_TEMP_1, 35, ITEM_LATIOSITE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveDiancie::
	givemon VAR_TEMP_1, 35, ITEM_DIANCITE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveRayquaza::
	givemon VAR_TEMP_1, 35, move1=MOVE_DRAGON_ASCENT, move2=MOVE_EXTREME_SPEED, move3=MOVE_EARTHQUAKE, move4=MOVE_DRAGON_DANCE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_GiveMewtwo::
	givemon VAR_TEMP_1, 35, ITEM_MEWTWONITE_Y
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_MegaBonus::
	goto_if_ne VAR_TEMP_1, SPECIES_MEWTWO, CeladonCity_GameCorner_PrizeRoom_EventScript_MegaBonusEnd
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_MewtwoSecondStone
	giveitem ITEM_MEWTWONITE_X
CeladonCity_GameCorner_PrizeRoom_EventScript_MegaBonusEnd::
	return

CeladonCity_GameCorner_PrizeRoom_EventScript_CheckReceivedMon::
	goto_if_eq VAR_RESULT, 0, CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonParty
	goto_if_eq VAR_RESULT, 1, CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonPC
	goto_if_eq VAR_RESULT, 2, CeladonCity_GameCorner_PrizeRoom_EventScript_PartyFull
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PartyFull::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox gText_NoMoreRoomForPokemon
	hidecoinsbox
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NicknamePartyMon::
	getpartysize
	subvar VAR_RESULT, 1
	copyvar VAR_0x8004, VAR_RESULT
	call Common_EventScript_NameReceivedPartyMon
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_CoinCaseRequired
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_NeedMoreCoins
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonParty::
	removecoins VAR_TEMP_2
	updatecoinsbox
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message gText_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	call CeladonCity_GameCorner_PrizeRoom_EventScript_MegaBonus
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, CeladonCity_GameCorner_PrizeRoom_EventScript_NicknamePartyMon
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ReceivedMonPC::
	removecoins VAR_TEMP_2
	updatecoinsbox
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	playfanfare MUS_LEVEL_UP
	message gText_PlayerObtainedTheMon
	waitmessage
	waitfanfare
	call CeladonCity_GameCorner_PrizeRoom_EventScript_MegaBonus
	bufferspeciesname STR_VAR_1, VAR_TEMP_1
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TransferredToPC::
	call Common_EventScript_TransferredToPC
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkTMs::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_COIN_CASE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	showcoinsbox 0, 0
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeTM::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 11, 0, MULTI_GAME_CORNER_TMPRIZES, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_TM13
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_TM23
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_TM24
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_TM30
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_TM35
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM13::
	setvar VAR_TEMP_1, ITEM_TM13
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_ICE_BEAM
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM23::
	setvar VAR_TEMP_1, ITEM_TM24
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_THUNDERBOLT
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM24::
	setvar VAR_TEMP_1, ITEM_TM35
	setvar VAR_TEMP_2, 3000
	buffermovename STR_VAR_2, MOVE_FLAMETHROWER
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM30::
	setvar VAR_TEMP_1, ITEM_TM29
	setvar VAR_TEMP_2, 3500
	buffermovename STR_VAR_2, MOVE_PSYCHIC
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TM35::
	setvar VAR_TEMP_1, ITEM_TM26
	setvar VAR_TEMP_2, 4000
	buffermovename STR_VAR_2, MOVE_EARTHQUAKE
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeTM::
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_YouWantTM, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_TryGivePrize
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem::
	bufferitemname STR_VAR_1, VAR_TEMP_1
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_YouWantPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_TryGivePrize
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_TryGivePrize::
	bufferitemname STR_VAR_1, VAR_TEMP_1
	checkcoins VAR_RESULT
	goto_if_lt VAR_RESULT, VAR_TEMP_2, CeladonCity_GameCorner_PrizeRoom_EventScript_NotEnoughCoins
	checkitemspace VAR_TEMP_1
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_GameCorner_PrizeRoom_EventScript_BagFull
	removecoins VAR_TEMP_2
	updatecoinsbox
	giveitem VAR_TEMP_1
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_BagFull::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox gText_TooBadBagIsFull
	hidecoinsbox
	release
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_PrizeClerkItems::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_COIN_CASE, CeladonCity_GameCorner_PrizeRoom_EventScript_NeedCoinCase
	showcoinsbox 0, 0
	msgbox CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_ChoosePrizeItem::
	message CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize
	waitmessage
	multichoice 10, 0, MULTI_GAME_CORNER_BATTLE_ITEM_PRIZES, FALSE
	switch VAR_RESULT
	case 0, CeladonCity_GameCorner_PrizeRoom_EventScript_SmokeBall
	case 1, CeladonCity_GameCorner_PrizeRoom_EventScript_MiracleSeed
	case 2, CeladonCity_GameCorner_PrizeRoom_EventScript_Charcoal
	case 3, CeladonCity_GameCorner_PrizeRoom_EventScript_MysticWater
	case 4, CeladonCity_GameCorner_PrizeRoom_EventScript_YellowFlute
	case 5, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	case 127, CeladonCity_GameCorner_PrizeRoom_EventScript_EndPrizeExchange
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_SmokeBall::
	setvar VAR_TEMP_1, ITEM_ASSAULT_VEST
	setvar VAR_TEMP_2, 3000
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_MiracleSeed::
	setvar VAR_TEMP_1, ITEM_LEFTOVERS
	setvar VAR_TEMP_2, 2500
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_Charcoal::
	setvar VAR_TEMP_1, ITEM_LIFE_ORB
	setvar VAR_TEMP_2, 3000
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_MysticWater::
	setvar VAR_TEMP_1, ITEM_CHOICE_SCARF
	setvar VAR_TEMP_2, 3500
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_EventScript_YellowFlute::
	setvar VAR_TEMP_1, ITEM_FOCUS_SASH
	setvar VAR_TEMP_2, 2000
	goto CeladonCity_GameCorner_PrizeRoom_EventScript_ConfirmPrizeItem
	end

CeladonCity_GameCorner_PrizeRoom_Text_FancyThatPorygon::
	.string "They say the boss caught a real\n"
	.string "MEWTWO and put it up as a prize.\p"
	.string "Nine thousand nine hundred and\n"
	.string "ninety-nine COINS, though… Brutal!$"

CeladonCity_GameCorner_PrizeRoom_Text_RakedItInToday::
	.string "Gahaha! I raked it in today!\p"
	.string "Now I just have to get out\n"
	.string "before the ROCKETS notice.$"

CeladonCity_GameCorner_PrizeRoom_Text_CoinCaseRequired::
	.string "A COIN CASE is required…$"

CeladonCity_GameCorner_PrizeRoom_Text_WeExchangeCoinsForPrizes::
	.string "Welcome, high roller. We trade\n"
	.string "COINS for prizes you won't find\l"
	.string "on any shelf in KANTO.$"

CeladonCity_GameCorner_PrizeRoom_Text_WhichPrize::
	.string "Which prize would you like?$"

CeladonCity_GameCorner_PrizeRoom_Text_HereYouGo::
	.string "はい どうぞ$"

CeladonCity_GameCorner_PrizeRoom_Text_YouWantPrize::
	.string "So, you want the {STR_VAR_1}?$"

CeladonCity_GameCorner_PrizeRoom_Text_YouWantTM::
	.string "Okay, a TM of {STR_VAR_2}\n"
	.string "is what you want?$"

CeladonCity_GameCorner_PrizeRoom_Text_NeedMoreCoins::
	.string "Sorry, you'll need more COINS\n"
	.string "than that.$"

CeladonCity_GameCorner_PrizeRoom_Text_OopsNotEnoughRoom::
	.string "おきゃくさん もう もてないよ$"

CeladonCity_GameCorner_PrizeRoom_Text_OhFineThen::
	.string "あっ そう$"

CeladonCity_GameCorner_PrizeRoom_Text_MewtwoSecondStone::
	.string "MEWTWO came with two stones.\n"
	.string "Take the other one too!$"
