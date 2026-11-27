PalletTown_PlayersHouse_1F_Frlg_MapScripts::
	.byte 0

PalletTown_PlayersHouse_1F_EventScript_Mom::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_RIVAL_IN_OAKS_LAB, PalletTown_PlayersHouse_1F_EventScript_MomHeal
	checkplayergender
	call_if_eq VAR_RESULT, MALE, PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale
	call_if_eq VAR_RESULT, FEMALE, PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouFemale
	closemessage
	applymovement LOCALID_PLAYERS_HOUSE_1F_MOM, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale::
	msgbox PalletTown_PlayersHouse_1F_Text_AllBoysLeaveOakLookingForYou
	return

PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouFemale::
	msgbox PalletTown_PlayersHouse_1F_Text_AllGirlsLeaveOakLookingForYou
	return

PalletTown_PlayersHouse_1F_EventScript_MomHeal::
	msgbox PalletTown_PlayersHouse_1F_Text_YouShouldTakeQuickRest
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare
	release
	end

@ Displays special text if interacted with from side or back (which are normally inaccessible)
PalletTown_PlayersHouse_1F_EventScript_TV::
	lockall
	goto_if_eq VAR_FACING, DIR_NORTH, PalletTown_PlayersHouse_1F_EventScript_TVScreen
	msgbox PalletTown_PlayersHouse_1F_Text_OopsWrongSide
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreen::
	checkplayergender
	call_if_eq VAR_RESULT, MALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenMale
	call_if_eq VAR_RESULT, FEMALE, PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale
	releaseall
	end

PalletTown_PlayersHouse_1F_EventScript_TVScreenMale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVFourBoysOnRailroad
	return

PalletTown_PlayersHouse_1F_EventScript_TVScreenFemale::
	msgbox PalletTown_PlayersHouse_1F_Text_MovieOnTVGirlOnBrickRoad
	return

PalletTown_PlayersHouse_1F_Text_AllBoysLeaveOakLookingForYou::
	.string "MA: GIOVANNI called for you.\n"
	.string "Don't keep the boss waiting!$"

PalletTown_PlayersHouse_1F_Text_AllGirlsLeaveOakLookingForYou::
	.string "MA: GIOVANNI called for you.\n"
	.string "Don't keep the boss waiting!$"

PalletTown_PlayersHouse_1F_Text_YouShouldTakeQuickRest::
	.string "MA: {PLAYER}! Back in one piece?\p"
	.string "Sit down. I'll patch up\n"
	.string "your crew.$"

PalletTown_PlayersHouse_1F_Text_LookingGreatTakeCare::
	.string "MA: There. Good as new.\p"
	.string "Now go show them who\n"
	.string "runs KANTO!$"

PalletTown_PlayersHouse_1F_Text_MovieOnTVFourBoysOnRailroad::
	.string "The news: gang war in KANTO.\n"
	.string "The police lost another town.\p"
	.string "…Time to go to work.$"

PalletTown_PlayersHouse_1F_Text_MovieOnTVGirlOnBrickRoad::
	.string "The news: gang war in KANTO.\n"
	.string "The police lost another town.\p"
	.string "…Time to go to work.$"

PalletTown_PlayersHouse_1F_Text_OopsWrongSide::
	.string "Oops, wrong side…$"

@ Gangster Outbreak: the crew's lookout tells the player which gym boss is next
PalletTown_PlayersHouse_1F_EventScript_Lookout::
	lock
	faceplayer
	special GangItems_CountBadges
	switch VAR_RESULT
	case 0, PalletTown_PlayersHouse_1F_EventScript_Lookout0
	case 1, PalletTown_PlayersHouse_1F_EventScript_Lookout1
	case 2, PalletTown_PlayersHouse_1F_EventScript_Lookout2
	case 3, PalletTown_PlayersHouse_1F_EventScript_Lookout3
	case 4, PalletTown_PlayersHouse_1F_EventScript_Lookout4
	case 5, PalletTown_PlayersHouse_1F_EventScript_Lookout5
	case 6, PalletTown_PlayersHouse_1F_EventScript_Lookout6
	case 7, PalletTown_PlayersHouse_1F_EventScript_Lookout7
	msgbox PalletTown_PlayersHouse_1F_Text_LookoutLeague
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout0::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout0
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout1::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout1
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout2::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout2
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout3::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout3
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout4::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout4
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout5::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout5
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout6::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout6
	release
	end

PalletTown_PlayersHouse_1F_EventScript_Lookout7::
	msgbox PalletTown_PlayersHouse_1F_Text_Lookout7
	release
	end

PalletTown_PlayersHouse_1F_Text_Lookout0::
	.string "TINO: Lookout report, boss!\p"
	.string "PIERS runs the GYM in\n"
	.string "PEWTER CITY. Start there!$"

PalletTown_PlayersHouse_1F_Text_Lookout1::
	.string "TINO: Lookout report, boss!\p"
	.string "GUZMA took over the GYM\n"
	.string "in CERULEAN CITY.$"

PalletTown_PlayersHouse_1F_Text_Lookout2::
	.string "TINO: Lookout report, boss!\p"
	.string "ARCHER holds the GYM in\n"
	.string "VERMILION CITY.$"

PalletTown_PlayersHouse_1F_Text_Lookout3::
	.string "TINO: Lookout report, boss!\p"
	.string "ARCHIE and MAXIE share the\n"
	.string "GYM in CELADON CITY.$"

PalletTown_PlayersHouse_1F_Text_Lookout4::
	.string "TINO: Lookout report, boss!\p"
	.string "GHETSIS sits in the GYM\n"
	.string "of FUCHSIA CITY.$"

PalletTown_PlayersHouse_1F_Text_Lookout5::
	.string "TINO: Lookout report, boss!\p"
	.string "LYSANDRE rules the GYM in\n"
	.string "SAFFRON CITY.$"

PalletTown_PlayersHouse_1F_Text_Lookout6::
	.string "TINO: Lookout report, boss!\p"
	.string "CYRUS hides in the GYM on\n"
	.string "CINNABAR ISLAND.$"

PalletTown_PlayersHouse_1F_Text_Lookout7::
	.string "TINO: Lookout report, boss!\p"
	.string "GIOVANNI himself waits in\n"
	.string "VIRIDIAN CITY. Make him proud!$"

PalletTown_PlayersHouse_1F_Text_LookoutLeague::
	.string "TINO: Eight BADGES, boss!\p"
	.string "Only the LEAGUE is left.\n"
	.string "Show them who rules KANTO!$"
