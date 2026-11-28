PokemonTower_5F_Frlg_MapScripts::
	.byte 0

PokemonTower_5F_EventScript_Channeler::
	msgbox PokemonTower_5F_Text_RestHereInPurifiedSpace, MSGBOX_NPC
	end

PokemonTower_5F_EventScript_PurifiedZone::
	lockall
	fadescreen FADE_TO_WHITE
	special HealPlayerParty
	fadescreen FADE_FROM_WHITE
	msgbox PokemonTower_5F_Text_PurifiedZoneMonsFullyHealed
	setvar VAR_TEMP_1, 1
	releaseall
	end

PokemonTower_5F_EventScript_ExitPurifiedZone::
	lockall
	setvar VAR_TEMP_1, 0
	releaseall
	end

PokemonTower_5F_EventScript_Tammy::
	trainerbattle_single TRAINER_CHANNELER_TAMMY, PokemonTower_5F_Text_TammyIntro, PokemonTower_5F_Text_TammyDefeat
	msgbox PokemonTower_5F_Text_TammyPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_5F_EventScript_Ruth::
	trainerbattle_single TRAINER_CHANNELER_RUTH, PokemonTower_5F_Text_RuthIntro, PokemonTower_5F_Text_RuthDefeat
	msgbox PokemonTower_5F_Text_RuthPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_5F_EventScript_Karina::
	trainerbattle_single TRAINER_CHANNELER_KARINA, PokemonTower_5F_Text_KarinaIntro, PokemonTower_5F_Text_KarinaDefeat
	msgbox PokemonTower_5F_Text_KarinaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_5F_EventScript_Janae::
	trainerbattle_single TRAINER_CHANNELER_JANAE, PokemonTower_5F_Text_JanaeIntro, PokemonTower_5F_Text_JanaeDefeat
	msgbox PokemonTower_5F_Text_JanaePostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_5F_Text_RestHereInPurifiedSpace::
	.string "Come, child! This space is pure. No\n"
	.string "gangs, no ghosts. Rest here.$"

PokemonTower_5F_Text_TammyIntro::
	.string "Your future is dark.\p"
	.string "Let me show you!$"

PokemonTower_5F_Text_TammyDefeat::
	.string "The visions… were wrong!$"

PokemonTower_5F_Text_TammyPostBattle::
	.string "Your aura is strong.\p"
	.string "Like a true boss.$"

PokemonTower_5F_Text_RuthIntro::
	.string "The outbreak woke the ghosts.\p"
	.string "And me!$"

PokemonTower_5F_Text_RuthDefeat::
	.string "My mind… is spinning!$"

PokemonTower_5F_Text_RuthPostBattle::
	.string "Beware the road to VIRIDIAN.$"

PokemonTower_5F_Text_KarinaIntro::
	.string "The ghosts told me you'd come.$"

PokemonTower_5F_Text_KarinaDefeat::
	.string "The spirits abandoned me…$"

PokemonTower_5F_Text_KarinaPostBattle::
	.string "The spirits say GIOVANNI fears\n"
	.string "you already.$"

PokemonTower_5F_Text_JanaeIntro::
	.string "I feel a strange power around\n"
	.string "you…$"

PokemonTower_5F_Text_JanaeDefeat::
	.string "I did not foresee this!$"

PokemonTower_5F_Text_JanaePostBattle::
	.string "Leave this place before night\n"
	.string "falls.$"

PokemonTower_5F_Text_PurifiedZoneMonsFullyHealed::
	.string "Entered the purified and protected\n"
	.string "zone.\p"
	.string "{PLAYER}'s POKéMON were fully\n"
	.string "healed.$"

