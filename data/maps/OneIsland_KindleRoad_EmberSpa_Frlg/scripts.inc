OneIsland_KindleRoad_EmberSpa_Frlg_MapScripts::
	.byte 0

OneIsland_KindleRoad_EmberSpa_EventScript_OldMan::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_WaterWarmsMeToCore, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BaldingMan1::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_EnjoyBowlOfChowder, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BaldingMan2::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_HotSpringIsTherapeutic, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_OldWoman::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_SeeHowSmoothMySkinIs, MSGBOX_NPC
	end

OneIsland_KindleRoad_EmberSpa_EventScript_BlackBelt::
	lock
	faceplayer
	famechecker FAMECHECKER_BRUNO, 3
	msgbox OneIsland_KindleRoad_EmberSpa_Text_BrunoVisitsSpaOnOccasion
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_RockSmashMan::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HM06, OneIsland_KindleRoad_EmberSpa_EventScript_AlreadyGotHM06
	msgbox OneIsland_KindleRoad_EmberSpa_Text_UsedThisToMakeEmberSpa
	checkitem ITEM_HM06
	goto_if_eq VAR_RESULT, TRUE, Gang_OneIsland_KindleRoad_EmberSpa_SkipITEM_HM06
	giveitem ITEM_HM06
Gang_OneIsland_KindleRoad_EmberSpa_SkipITEM_HM06:
	setflag FLAG_GOT_HM06
	msgbox OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_AlreadyGotHM06::
	msgbox OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06
	release
	end

OneIsland_KindleRoad_EmberSpa_EventScript_SpaHeal::
	lockall
	fadescreen FADE_TO_WHITE
	special HealPlayerParty
	fadescreen FADE_FROM_WHITE
	msgbox OneIsland_KindleRoad_EmberSpa_Text_WaterExquisiteFullyRefreshed
	setvar VAR_TEMP_1, 1
	releaseall
	end

OneIsland_KindleRoad_EmberSpa_Text_WaterWarmsMeToCore::
	.string "Haaaah! This is heavenly!\p"
	.string "The water melts away every gang\n"
	.string "war. You should jump in too!$"

OneIsland_KindleRoad_EmberSpa_Text_EnjoyBowlOfChowder::
	.string "After the bath, a bowl of chowder.\p"
	.string "No gangs, no fights. Just chowder.$"

OneIsland_KindleRoad_EmberSpa_Text_WaterExquisiteFullyRefreshed::
	.string "The water is perfectly warm.\n"
	.string "It feels exquisite…\p"
	.string "{PLAYER} and the POKéMON were\n"
	.string "fully refreshed!$"

OneIsland_KindleRoad_EmberSpa_Text_HotSpringIsTherapeutic::
	.string "A hot spring is good for the soul.\n"
	.string "Especially after a gang fight.$"

OneIsland_KindleRoad_EmberSpa_Text_SeeHowSmoothMySkinIs::
	.string "See how smooth my skin is?\p"
	.string "No stress here. No gangs, no fires.\n"
	.string "Just hot water.$"

OneIsland_KindleRoad_EmberSpa_Text_BrunoVisitsSpaOnOccasion::
	.string "After a day of training, nothing\n"
	.string "beats a hot soak.\p"
	.string "BRUNO used to come here before\n"
	.string "the gangs took the LEAGUE from\p"
	.string "him.$"

OneIsland_KindleRoad_EmberSpa_Text_UsedThisToMakeEmberSpa::
	.string "Hot springs go with volcanoes.\p"
	.string "I built the EMBER SPA years ago.\p"
	.string "I carved the pool out of a giant\n"
	.string "boulder by hand.\p"
	.string "I used this to do it. You can have\n"
	.string "it.$"

OneIsland_KindleRoad_EmberSpa_Text_ExplainHM06::
	.string "That shatters boulders as if they\n"
	.string "were made of crackers.$"

