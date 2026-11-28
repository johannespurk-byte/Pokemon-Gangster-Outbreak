PewterCity_Museum_1F_Frlg_MapScripts::
	.byte 0

PewterCity_Museum_1F_EventScript_Scientist1::
	lock
	faceplayer
	goto_if_eq VAR_FACING, DIR_WEST, PewterCity_Museum_1F_EventScript_Scientist1BehindCounter
	goto_if_eq VAR_FACING, DIR_SOUTH, PewterCity_Museum_1F_EventScript_Scientist1BehindCounter
	goto_if_eq VAR_FACING, DIR_NORTH, PewterCity_Museum_1F_EventScript_Scientist1BehindCounter
	msgbox PewterCity_Museum_1F_Text_PleaseEnjoyYourself
	release
	end

PewterCity_Museum_1F_EventScript_Scientist1BehindCounter::
	msgbox PewterCity_Museum_1F_Text_DoYouKnowWhatAmberIs, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PewterCity_Museum_1F_EventScript_AmberHasGeneticMatter
	goto_if_eq VAR_RESULT, NO, PewterCity_Museum_1F_EventScript_ExplainAmber
	end

PewterCity_Museum_1F_EventScript_AmberHasGeneticMatter::
	msgbox PewterCity_Museum_1F_Text_AmberContainsGeneticMatter
	applymovement LOCALID_MUSEUM_SCIENTIST1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PewterCity_Museum_1F_EventScript_ExplainAmber::
	msgbox PewterCity_Museum_1F_Text_AmberIsFossilizedSap
	applymovement LOCALID_MUSEUM_SCIENTIST1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PewterCity_Museum_1F_EventScript_EntranceTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto PewterCity_Museum_1F_EventScript_EntranceTrigger
	end

PewterCity_Museum_1F_EventScript_EntranceTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto PewterCity_Museum_1F_EventScript_EntranceTrigger
	end

PewterCity_Museum_1F_EventScript_EntranceTriggerRight::
	lockall
	setvar VAR_TEMP_1, 2
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto PewterCity_Museum_1F_EventScript_EntranceTrigger
	end

PewterCity_Museum_1F_EventScript_EntranceTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	showmoneybox 0, 0
	msgbox PewterCity_Museum_1F_Text_Its50YForChildsTicket, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PewterCity_Museum_1F_EventScript_TryPayForTicket
	msgbox PewterCity_Museum_1F_Text_ComeAgain
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, PewterCity_Museum_1F_Movement_ForcePlayerExit
	waitmovement 0
	releaseall
	end

PewterCity_Museum_1F_EventScript_TryPayForTicket::
	checkmoney 50
	goto_if_eq VAR_RESULT, FALSE, PewterCity_Museum_1F_EventScript_NotEnoughMoney
	closemessage
	call_if_eq VAR_TEMP_1, 0, PewterCity_Museum_1F_EventScript_PlayerApproachCounterLeft
	call_if_eq VAR_TEMP_1, 1, PewterCity_Museum_1F_EventScript_PlayerApproachCounterMid
	call_if_eq VAR_TEMP_1, 2, PewterCity_Museum_1F_EventScript_PlayerApproachCounterRight
	playse SE_SHOP
	removemoney 50
	updatemoneybox
	waitse
	msgbox PewterCity_Museum_1F_Text_Right50YThankYou
	setvar VAR_MAP_SCENE_PEWTER_CITY_MUSEUM_1F, 1
	hidemoneybox
	releaseall
	end

PewterCity_Museum_1F_EventScript_PlayerApproachCounterLeft::
	applymovement LOCALID_PLAYER, PewterCity_Museum_1F_Movement_ApproachCounterLeft
	waitmovement 0
	return

PewterCity_Museum_1F_EventScript_PlayerApproachCounterMid::
	applymovement LOCALID_PLAYER, PewterCity_Museum_1F_Movement_ApproachCounterMid
	waitmovement 0
	return

PewterCity_Museum_1F_EventScript_PlayerApproachCounterRight::
	applymovement LOCALID_PLAYER, PewterCity_Museum_1F_Movement_ApproachCounterRight
	waitmovement 0
	return

PewterCity_Museum_1F_EventScript_NotEnoughMoney::
	msgbox PewterCity_Museum_1F_Text_DontHaveEnoughMoney
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, PewterCity_Museum_1F_Movement_ForcePlayerExit
	waitmovement 0
	releaseall
	end

PewterCity_Museum_1F_Movement_ForcePlayerExit::
	walk_down
	step_end

PewterCity_Museum_1F_Movement_ApproachCounterLeft::
	walk_right
	walk_right
	delay_8
	walk_in_place_right
	step_end

PewterCity_Museum_1F_Movement_ApproachCounterMid::
	walk_right
	delay_8
	walk_in_place_right
	step_end

PewterCity_Museum_1F_Movement_ApproachCounterRight::
	delay_8
	walk_in_place_right
	step_end

PewterCity_Museum_1F_EventScript_Scientist2::
	msgbox PewterCity_Museum_1F_Text_WeHaveTwoFossilsOnExhibit, MSGBOX_NPC
	end

PewterCity_Museum_1F_EventScript_OldMan::
	msgbox PewterCity_Museum_1F_Text_ShouldBeGratefulForLongLife, MSGBOX_NPC
	end

PewterCity_Museum_1F_EventScript_OldAmberScientist::
	lock
	faceplayer
	goto_if_set FLAG_GOT_OLD_AMBER, PewterCity_Museum_1F_EventScript_AlreadyGotOldAmber
	msgbox PewterCity_Museum_1F_Text_WantYouToGetAmberExamined
	checkitemspace ITEM_OLD_AMBER
	goto_if_eq VAR_RESULT, FALSE, PewterCity_Museum_1F_EventScript_NoRoomForOldAmber
	setflag FLAG_GOT_OLD_AMBER
	removeobject LOCALID_OLD_AMBER
	giveitem_msg PewterCity_Museum_1F_Text_ReceivedOldAmberFromMan, ITEM_OLD_AMBER, 1, MUS_RG_OBTAIN_KEY_ITEM
	release
	end

PewterCity_Museum_1F_EventScript_NoRoomForOldAmber::
	msgbox PewterCity_Museum_1F_Text_DontHaveSpaceForThis
	release
	end

PewterCity_Museum_1F_EventScript_AlreadyGotOldAmber::
	msgbox PewterCity_Museum_1F_Text_GetOldAmberChecked
	release
	end

PewterCity_Museum_1F_EventScript_OldAmber::
	msgbox PewterCity_Museum_1F_Text_BeautifulPieceOfAmber, MSGBOX_NPC
	end

PewterCity_Museum_1F_EventScript_AerodactylFossil::
	lockall
	setvar VAR_0x8004, SPECIES_AERODACTYL
	setvar VAR_0x8005, 10
	setvar VAR_0x8006, 3
	special OpenMuseumFossilPic
	msgbox PewterCity_Museum_1F_Text_AerodactylFossil
	special CloseMuseumFossilPic
	releaseall
	end

PewterCity_Museum_1F_EventScript_KabutopsFossil::
	lockall
	setvar VAR_0x8004, SPECIES_KABUTOPS
	setvar VAR_0x8005, 10
	setvar VAR_0x8006, 3
	special OpenMuseumFossilPic
	msgbox PewterCity_Museum_1F_Text_KabutopsFossil
	special CloseMuseumFossilPic
	releaseall
	end

PewterCity_Museum_1F_Text_Its50YForChildsTicket::
	.string "TEAM YELL runs this place now.\n"
	.string "Entry costs ¥50, kid.\p"
	.string "Wanna come in?$"

PewterCity_Museum_1F_Text_ComeAgain::
	.string "Your loss!$"

PewterCity_Museum_1F_Text_Right50YThankYou::
	.string "¥50. Cheers, mate!$"

PewterCity_Museum_1F_Text_DontHaveEnoughMoney::
	.string "No cash, no entry!$"

PewterCity_Museum_1F_Text_PleaseEnjoyYourself::
	.string "Look all you want.\n"
	.string "Touch the loot and you're out!$"

PewterCity_Museum_1F_Text_DoYouKnowWhatAmberIs::
	.string "Oi! No sneaking in the back!\p"
	.string "…Whatever.\n"
	.string "Do you know what AMBER is?$"

PewterCity_Museum_1F_Text_AmberContainsGeneticMatter::
	.string "AMBER holds the genes of\n"
	.string "ancient POKéMON.\p"
	.string "The LAB on CINNABAR ISLAND can\n"
	.string "bring them back to life.$"

PewterCity_Museum_1F_Text_AmberIsFossilizedSap::
	.string "AMBER is fossilized tree sap.\p"
	.string "TEAM YELL dug half of it out of\n"
	.string "the walls and sold it.$"

@ Unused
PewterCity_Museum_1F_Text_PleaseGoAround::
	.string "あちらへ　おまわりください$"

PewterCity_Museum_1F_Text_ShouldBeGratefulForLongLife::
	.string "The gangs sold the real bones.\p"
	.string "What you see in these cases\n"
	.string "is cheap plaster!$"

PewterCity_Museum_1F_Text_WantYouToGetAmberExamined::
	.string "Ssh! TEAM YELL missed this\n"
	.string "chunk of AMBER.\p"
	.string "I think there's POKéMON DNA\n"
	.string "inside!\p"
	.string "Take it before they find it.\n"
	.string "Get it checked at a POKéMON LAB.$"

PewterCity_Museum_1F_Text_ReceivedOldAmberFromMan::
	.string "{PLAYER} received the OLD AMBER\n"
	.string "from the man.$"

PewterCity_Museum_1F_Text_GetOldAmberChecked::
	.string "Ssh!\n"
	.string "Get the OLD AMBER checked!$"

PewterCity_Museum_1F_Text_DontHaveSpaceForThis::
	.string "You don't have space for this.$"

PewterCity_Museum_1F_Text_WeHaveTwoFossilsOnExhibit::
	.string "The real fossils went to the\n"
	.string "black market.\p"
	.string "The dealer over there sells\n"
	.string "them. For a price…$"

PewterCity_Museum_1F_Text_BeautifulPieceOfAmber::
	.string "A beautiful piece of AMBER.\n"
	.string "Somebody chained it to the stand.$"

PewterCity_Museum_1F_Text_AerodactylFossil::
	.string "AERODACTYL Fossil\n"
	.string "A plaster copy. Cracked.$"

PewterCity_Museum_1F_Text_KabutopsFossil::
	.string "KABUTOPS Fossil\n"
	.string "A plaster copy. Cracked.$"

@ Gangster Outbreak: the fossil black market (fossils can be revived at the CINNABAR ISLAND lab)
PewterCity_Museum_1F_EventScript_FossilDealer::
	lock
	faceplayer
	message PewterCity_Museum_1F_Text_FossilDealer
	waitmessage
	pokemart PewterCity_Museum_1F_FossilDealerItems
	msgbox gText_PleaseComeAgain
	release
	end

	.align 2
PewterCity_Museum_1F_FossilDealerItems:
	.2byte ITEM_HELIX_FOSSIL
	.2byte ITEM_DOME_FOSSIL
	.2byte ITEM_OLD_AMBER
	pokemartlistend

PewterCity_Museum_1F_Text_FossilDealer::
	.string "KLONK! BLACK MARKET FOSSILS.\p"
	.string "Fresh from MT. MOON.\n"
	.string "No questions asked.$"
