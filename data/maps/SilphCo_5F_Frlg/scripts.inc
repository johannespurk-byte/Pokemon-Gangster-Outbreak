SilphCo_5F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_5F_OnLoad
	.byte 0

SilphCo_5F_OnLoad::
	call_if_unset FLAG_SILPH_5F_DOOR_1, EventScript_Close5FDoor1
	call_if_unset FLAG_SILPH_5F_DOOR_2, EventScript_Close5FDoor2
	call_if_unset FLAG_SILPH_5F_DOOR_3, EventScript_Close5FDoor3
	end

SilphCo_5F_EventScript_WorkerM::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_5F_EventScript_WorkerMRocketsGone
	msgbox SilphCo_5F_Text_RocketsInUproarAboutIntruder
	release
	end

SilphCo_5F_EventScript_WorkerMRocketsGone::
	msgbox SilphCo_5F_Text_YoureOurHeroThankYou
	release
	end

SilphCo_5F_EventScript_PokemonReport1::
	msgbox SilphCo_5F_Text_PorygonFirstVRMon, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_PokemonReport2::
	msgbox SilphCo_5F_Text_Over350TechniquesConfirmed, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_PokemonReport3::
	msgbox SilphCo_5F_Text_SomeMonsEvolveWhenTraded, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_FloorSign::
	msgbox SilphCo_5F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_5F_EventScript_Beau::
	trainerbattle_single TRAINER_SCIENTIST_BEAU, SilphCo_5F_Text_BeauIntro, SilphCo_5F_Text_BeauDefeat
	msgbox SilphCo_5F_Text_BeauPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_EventScript_Grunt1::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_28, SilphCo_5F_Text_Grunt1Intro, SilphCo_5F_Text_Grunt1Defeat
	msgbox SilphCo_5F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_EventScript_Grunt2::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_29, SilphCo_5F_Text_Grunt2Intro, SilphCo_5F_Text_Grunt2Defeat
	msgbox SilphCo_5F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_EventScript_Dalton::
	trainerbattle_single TRAINER_JUGGLER_DALTON, SilphCo_5F_Text_DaltonIntro, SilphCo_5F_Text_DaltonDefeat
	msgbox SilphCo_5F_Text_DaltonPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_5F_Text_RocketsInUproarAboutIntruder::
	.string "TEAM ROCKET is in an uproar about\n"
	.string "an intruder.\p"
	.string "One of their own recruits! That's\n"
	.string "you, right?$"

SilphCo_5F_Text_YoureOurHeroThankYou::
	.string "TEAM ROCKET left! You're our hero!\n"
	.string "Thank you!$"

SilphCo_5F_Text_Grunt1Intro::
	.string "I guard this place for GIOVANNI.\n"
	.string "Prepare yourself!$"

SilphCo_5F_Text_Grunt1Defeat::
	.string "Urgh! Not again!$"

SilphCo_5F_Text_Grunt1PostBattle::
	.string "Don't tell GIOVANNI I lost to you.$"

SilphCo_5F_Text_BeauIntro::
	.string "Your POKéMON stats look…\n"
	.string "interesting!$"

SilphCo_5F_Text_BeauDefeat::
	.string "My theory… failed!$"

SilphCo_5F_Text_BeauPostBattle::
	.string "I'm writing a paper on gang\n"
	.string "warfare. You're in it now.$"

SilphCo_5F_Text_DaltonIntro::
	.string "I hacked a PC of TEAM ROCKET once.\p"
	.string "Now I hide out here!$"

SilphCo_5F_Text_DaltonDefeat::
	.string "Impossible! I calculated\n"
	.string "everything!$"

SilphCo_5F_Text_DaltonPostBattle::
	.string "GIOVANNI pays well for smart\n"
	.string "people.\p"
	.string "Just saying.$"

SilphCo_5F_Text_Grunt2Intro::
	.string "Hand over your POKéMON. Orders\n"
	.string "from GIOVANNI!$"

SilphCo_5F_Text_Grunt2Defeat::
	.string "I'm gonna be in so much trouble…$"

SilphCo_5F_Text_Grunt2PostBattle::
	.string "Word is you're GIOVANNI's new\n"
	.string "star. Is that true?$"

SilphCo_5F_Text_PorygonFirstVRMon::
	.string "It's a POKéMON REPORT!\p"
	.string "SILPH created PORYGON, the first\n"
	.string "virtual POKéMON. TEAM ROCKET\p"
	.string "wants the blueprints.$"

SilphCo_5F_Text_Over350TechniquesConfirmed::
	.string "It's a POKéMON REPORT!\p"
	.string "Over 350 moves have been\n"
	.string "confirmed. The gangs know most of\p"
	.string "them.$"

SilphCo_5F_Text_SomeMonsEvolveWhenTraded::
	.string "It's a POKéMON REPORT!\p"
	.string "Some POKéMON evolve when traded.\n"
	.string "The black markets love that.$"

SilphCo_5F_Text_FloorSign::
	.string "SILPH CO. HEAD OFFICE\n"
	.string "5F$"

SilphCo_5F_Text_RocketBossLookingForStrongMons::
	.string "TEAM ROCKET took the building,\n"
	.string "then TEAM FLARE tried too.\p"
	.string "Everyone wants SILPH's POKéMON.\n"
	.string "I hope our PRESIDENT is okay.$"
