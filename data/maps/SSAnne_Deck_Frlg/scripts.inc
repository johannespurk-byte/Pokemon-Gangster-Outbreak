SSAnne_Deck_Frlg_MapScripts::
	.byte 0

SSAnne_Deck_EventScript_Youngster::
	msgbox SSAnne_Deck_Text_ShipDepartingSoon, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_BaldingMan::
	msgbox SSAnne_Deck_Text_FeelSeasick, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_Sailor::
	msgbox SSAnne_Deck_Text_ScrubbingDecksHardWork, MSGBOX_NPC
	end

SSAnne_Deck_EventScript_Edmond::
	trainerbattle_single TRAINER_SAILOR_EDMOND, SSAnne_Deck_Text_EdmondIntro, SSAnne_Deck_Text_EdmondDefeat
	msgbox SSAnne_Deck_Text_EdmondPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_Deck_EventScript_Trevor::
	trainerbattle_single TRAINER_SAILOR_TREVOR, SSAnne_Deck_Text_TrevorIntro, SSAnne_Deck_Text_TrevorDefeat
	msgbox SSAnne_Deck_Text_TrevorPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_Deck_Text_ShipDepartingSoon::
	.string "The party's over.\n"
	.string "The ship will be departing soon.$"

SSAnne_Deck_Text_ScrubbingDecksHardWork::
	.string "Whew!\n"
	.string "Scrubbing decks is hard work!$"

SSAnne_Deck_Text_FeelSeasick::
	.string "Urf… I feel ill…\p"
	.string "I got seasick, so I stepped out to\n"
	.string "get some air…$"

SSAnne_Deck_Text_EdmondIntro::
	.string "You'll sink like the last punk who\n"
	.string "came here!$"

SSAnne_Deck_Text_EdmondDefeat::
	.string "Glub… I'm all washed up.$"

SSAnne_Deck_Text_EdmondPostBattle::
	.string "Swim fast if you see a boat from\n"
	.string "TEAM ROCKET.$"

SSAnne_Deck_Text_TrevorIntro::
	.string "Out here, only the strong stay\n"
	.string "afloat!$"

SSAnne_Deck_Text_TrevorDefeat::
	.string "Sunk! Totally sunk!$"

SSAnne_Deck_Text_TrevorPostBattle::
	.string "The water's the only free place\n"
	.string "left.$"

