.equ RECEIVED_TOWN_MAP, VAR_TEMP_1

PalletTown_RivalsHouse_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalletTown_RivalsHouse_OnTransition
	.byte 0

PalletTown_RivalsHouse_OnTransition::
	call_if_lt VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_MoveDaisyToTable
	call_if_ge VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap
	end

PalletTown_RivalsHouse_EventScript_MoveDaisyToTable::
	setobjectmovementtype LOCALID_DAISY, MOVEMENT_TYPE_FACE_RIGHT
	setobjectxyperm LOCALID_DAISY, 5, 4
	return

PalletTown_RivalsHouse_EventScript_AlreadyReceivedTownMap::
	setvar RECEIVED_TOWN_MAP, TRUE
	return

PalletTown_RivalsHouse_EventScript_Daisy::
	lock
	faceplayer
	famechecker FAMECHECKER_DAISY, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	goto_if_set FLAG_SYS_GAME_CLEAR, PalletTown_RivalsHouse_EventScript_GroomMon
	goto_if_eq RECEIVED_TOWN_MAP, TRUE, PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2, PalletTown_RivalsHouse_EventScript_ExplainTownMap
	goto_if_eq VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 1, PalletTown_RivalsHouse_EventScript_GiveTownMap
	goto_if_ge VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 1, PalletTown_RivalsHouse_EventScript_HeardBattledRival
	msgbox PalletTown_RivalsHouse_Text_HiBrothersAtLab
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_RivalsHouse_EventScript_HeardBattledRival::
	msgbox PalletTown_RivalsHouse_Text_HeardYouBattledRival
	release
	end

PalletTown_RivalsHouse_EventScript_GroomMon::
	goto_if_lt VAR_MASSAGE_COOLDOWN_STEP_COUNTER, 500, PalletTown_RivalsHouse_EventScript_RateMonFriendship
	msgbox PalletTown_RivalsHouse_Text_LikeMeToGroomMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	msgbox PalletTown_RivalsHouse_Text_GroomWhichOne
	special ChoosePartyMon
	lock
	faceplayer
	goto_if_ge VAR_0x8004, PARTY_SIZE, PalletTown_RivalsHouse_EventScript_DeclineGrooming
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, PalletTown_RivalsHouse_EventScript_CantGroomEgg
	msgbox PalletTown_RivalsHouse_Text_LookingNiceInNoTime
	closemessage
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	waitfanfare
	special DaisyMassageServices
	fadescreen FADE_FROM_BLACK
	special BufferMonNickname
	msgbox PalletTown_RivalsHouse_Text_ThereYouGoAllDone
	release
	end

PalletTown_RivalsHouse_EventScript_CantGroomEgg::
	msgbox PalletTown_RivalsHouse_Text_CantGroomAnEgg
	release
	end

PalletTown_RivalsHouse_EventScript_DeclineGrooming::
	msgbox PalletTown_RivalsHouse_Text_DontNeedAnyGrooming
	release
	end

PalletTown_RivalsHouse_EventScript_RateMonFriendship::
	msgbox PalletTown_RivalsHouse_Text_MayISeeFirstMon
	specialvar VAR_RESULT, GetLeadMonFriendship
	switch VAR_RESULT
	case 0, PalletTown_RivalsHouse_EventScript_MonFriendshipLowest
	case 1, PalletTown_RivalsHouse_EventScript_MonFriendshipLower
	case 2, PalletTown_RivalsHouse_EventScript_MonFriendshipLow
	case 3, PalletTown_RivalsHouse_EventScript_MonFriendshipMid
	case 4, PalletTown_RivalsHouse_EventScript_MonFriendshipHigh
	case 5, PalletTown_RivalsHouse_EventScript_MonFriendshipHigher
	case 6, PalletTown_RivalsHouse_EventScript_MonFriendshipHighest
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLowest::
	msgbox PalletTown_RivalsHouse_Text_WhyWouldMonHateYouSoMuch
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLower::
	msgbox PalletTown_RivalsHouse_Text_DontLikeWayItGlaresAtYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipLow::
	msgbox PalletTown_RivalsHouse_Text_NotFamiliarWithYouYet
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipMid::
	msgbox PalletTown_RivalsHouse_Text_ItsWarmingUpToYou
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigh::
	msgbox PalletTown_RivalsHouse_Text_ItsQuiteFriendly
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHigher::
	msgbox PalletTown_RivalsHouse_Text_ItLooksVeryHappy
	release
	end

PalletTown_RivalsHouse_EventScript_MonFriendshipHighest::
	msgbox PalletTown_RivalsHouse_Text_CouldntLoveYouMore
	release
	end

PalletTown_RivalsHouse_EventScript_PleaseGiveMonsRest::
	msgbox PalletTown_RivalsHouse_Text_PleaseGiveMonsRest
	release
	end

PalletTown_RivalsHouse_EventScript_GiveTownMap::
	msgbox PalletTown_RivalsHouse_Text_ErrandForGrandpaThisWillHelp
	closemessage
	checkitemspace ITEM_TOWN_MAP
	goto_if_eq VAR_RESULT, FALSE, PalletTown_RivalsHouse_EventScript_NoRoomForTownMap
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	removeobject LOCALID_TOWN_MAP
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	delay 15
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	delay 12
	giveitem_msg PalletTown_RivalsHouse_Text_ReceivedTownMapFromDaisy, ITEM_TOWN_MAP, 1, MUS_RG_OBTAIN_KEY_ITEM
	release
	end

PalletTown_RivalsHouse_EventScript_NoRoomForTownMap::
	msgbox PalletTown_RivalsHouse_Text_DontHaveSpaceForThis
	release
	end

PalletTown_RivalsHouse_EventScript_ExplainTownMap::
	msgbox PalletTown_RivalsHouse_Text_ExplainTownMap
	release
	end

PalletTown_RivalsHouse_EventScript_TownMap::
	msgbox PalletTown_RivalsHouse_Text_ItsBigMapOfKanto, MSGBOX_NPC
	end

PalletTown_RivalsHouse_EventScript_Bookshelf::
	msgbox PalletTown_RivalsHouse_Text_ShelvesCrammedFullOfBooks, MSGBOX_SIGN
	end

PalletTown_RivalsHouse_EventScript_Picture::
	msgbox PalletTown_RivalsHouse_Text_LovelyAndSweetClefairy, MSGBOX_SIGN
	end

PalletTown_RivalsHouse_Text_HiBrothersAtLab::
	.string "ROXY: This is SILVER's hideout.\n"
	.string "He's outside, guarding his turf.$"

PalletTown_RivalsHouse_Text_HeardYouBattledRival::
	.string "ROXY: So you fought {RIVAL}?\p"
	.string "He hasn't stopped talking\n"
	.string "about you since.$"

PalletTown_RivalsHouse_Text_ErrandForGrandpaThisWillHelp::
	.string "Running errands out there? Gee,\n"
	.string "brave of you.\p"
	.string "Here, this will help you find your\n"
	.string "way around the gangs.$"

PalletTown_RivalsHouse_Text_ReceivedTownMapFromDaisy::
	.string "{PLAYER} received a TOWN MAP\n"
	.string "from DAISY.$"

PalletTown_RivalsHouse_Text_DontHaveSpaceForThis::
	.string "You don't have space for this in\n"
	.string "your BAG.$"

PalletTown_RivalsHouse_Text_ExplainTownMap::
	.string "The TOWN MAP shows where you are\n"
	.string "and the names of places.\p"
	.string "Mark the gang turfs yourself.$"

PalletTown_RivalsHouse_Text_PleaseGiveMonsRest::
	.string "ROXY: This is {RIVAL}'s hideout.\n"
	.string "He said you might show up.\p"
	.string "Let your POKéMON rest a little\n"
	.string "before you face him outside.$"

PalletTown_RivalsHouse_Text_ItsBigMapOfKanto::
	.string "A big map of KANTO. Every city is\n"
	.string "circled in red marker.\p"
	.string "Gang turf, probably.$"

PalletTown_RivalsHouse_Text_ShelvesCrammedFullOfBooks::
	.string "The shelves are crammed with\n"
	.string "stolen goods.$"

PalletTown_RivalsHouse_Text_LikeMeToGroomMon::
	.string "DAISY: Hi, {PLAYER}! The crew is\n"
	.string "out, so it's quiet.\p"
	.string "I'm about to have some tea. Join\n"
	.string "me?\p"
	.string "Oh, but your POKéMON look roughed\n"
	.string "up.\p"
	.string "Would you like me to groom one?$"

PalletTown_RivalsHouse_Text_DontNeedAnyGrooming::
	.string "No grooming? Okay, we'll just have\n"
	.string "tea. Don't tell SILVER.$"

PalletTown_RivalsHouse_Text_GroomWhichOne::
	.string "Which one should I groom?$"

PalletTown_RivalsHouse_Text_LookingNiceInNoTime::
	.string "DAISY: Okay, I'll get it looking\n"
	.string "nice in no time.$"

PalletTown_RivalsHouse_Text_ThereYouGoAllDone::
#ifdef BUGFIX @ The localizers missed what should be a textcolor change in the localizations.
	.string "{COLOR DARK_GRAY}{STR_VAR_1} looks dreamily content…\p"
	.string "{COLOR RED}DAISY: There you go! All done.\n"
#else @ In the JP games, gender-based text used a different font instead of different colors.
	.string "{FONT_NORMAL}{STR_VAR_1} looks dreamily content…\p"
	.string "{FONT_FEMALE}DAISY: There you go! All done.\n"
#endif
	.string "See? Doesn't it look nice?\p"
	.string "Giggle…\n"
	.string "It's such a cute POKéMON.$"

PalletTown_RivalsHouse_Text_CantGroomAnEgg::
	.string "Oh, sorry. I honestly can't\n"
	.string "groom an EGG.$"

PalletTown_RivalsHouse_Text_MayISeeFirstMon::
	.string "DAISY: Out there, POKéMON fight\n"
	.string "for whoever treats them right.\p"
	.string "{PLAYER}, may I see your first\n"
	.string "POKéMON?$"

PalletTown_RivalsHouse_Text_CouldntLoveYouMore::
	.string "It would follow you into any gang\n"
	.string "war.\p"
	.string "It couldn't possibly trust you\n"
	.string "more.$"

PalletTown_RivalsHouse_Text_ItLooksVeryHappy::
	.string "It looks very happy.\p"
	.string "I wish SILVER treated his POKéMON\n"
	.string "like this.$"

PalletTown_RivalsHouse_Text_ItsQuiteFriendly::
	.string "It's quite friendly with you. Keep\n"
	.string "being good to it!$"

PalletTown_RivalsHouse_Text_ItsWarmingUpToYou::
	.string "It's warming up to you. Trust\n"
	.string "grows slowly out here.$"

PalletTown_RivalsHouse_Text_NotFamiliarWithYouYet::
	.string "It's not quite sure about you yet.\p"
	.string "Most POKéMON were stolen at least\n"
	.string "once. They're wary.$"

PalletTown_RivalsHouse_Text_DontLikeWayItGlaresAtYou::
	.string "{PLAYER}, I don't like the way it\n"
	.string "glares at you.\p"
	.string "Are you treating it like a gang\n"
	.string "does? Be nicer to it.$"

PalletTown_RivalsHouse_Text_WhyWouldMonHateYouSoMuch::
	.string "…Um, it's not easy for me to say\n"
	.string "this, but…\p"
	.string "Did you steal this one? It really\n"
	.string "hates you.$"

PalletTown_RivalsHouse_Text_LovelyAndSweetClefairy::
	.string "A WANTED poster of {RIVAL}.\p"
	.string "Someone crossed out the reward\n"
	.string "and wrote a bigger number.$"

@ Gangster Outbreak: the bouncer of Silver's clubhouse
PalletTown_RivalsHouse_EventScript_Bouncer::
	msgbox PalletTown_RivalsHouse_Text_Bouncer, MSGBOX_NPC
	end

PalletTown_RivalsHouse_Text_Bouncer::
	.string "DEX: Easy, pal. This is\n"
	.string "{RIVAL}'s clubhouse.\p"
	.string "Beat him outside after every\n"
	.string "new BADGE.\p"
	.string "He pays up with HMs. Respect\n"
	.string "is respect.$"
