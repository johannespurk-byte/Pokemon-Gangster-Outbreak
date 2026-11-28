PokemonTower_6F_Frlg_MapScripts::
	.byte 0

PokemonTower_6F_EventScript_MarowakGhost::
	lockall
	textcolor NPC_TEXT_COLOR_MON
	msgbox PokemonTower_6F_Text_BeGoneIntruders
	setwildbattle SPECIES_MAROWAK, 30
	special StartMarowakBattle
	waitstate
	goto_if_eq VAR_RESULT, FALSE, PokemonTower_6F_EventScript_DefeatedMarowakGhost  @ VAR_RESULT set by CB2_EndMarowakBattle
	applymovement LOCALID_PLAYER, PokemonTower_6F_Movement_ForcePlayerUp
	waitmovement 0
	releaseall
	end

PokemonTower_6F_EventScript_DefeatedMarowakGhost::
	message PokemonTower_6F_Text_GhostWasCubonesMother
	waitmessage
	waitse
	playmoncry SPECIES_MAROWAK, CRY_MODE_NORMAL
	waitbuttonpress
	waitmoncry
	msgbox PokemonTower_6F_Text_MothersSpiritWasCalmed
	setvar VAR_MAP_SCENE_POKEMON_TOWER_6F, 1
	releaseall
	end

PokemonTower_6F_Movement_ForcePlayerUp::
	walk_up
	step_end

PokemonTower_6F_EventScript_Angelica::
	trainerbattle_single TRAINER_CHANNELER_ANGELICA, PokemonTower_6F_Text_AngelicaIntro, PokemonTower_6F_Text_AngelicaDefeat
	msgbox PokemonTower_6F_Text_AngelicaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Emilia::
	trainerbattle_single TRAINER_CHANNELER_EMILIA, PokemonTower_6F_Text_EmiliaIntro, PokemonTower_6F_Text_EmiliaDefeat
	msgbox PokemonTower_6F_Text_EmiliaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_EventScript_Jennifer::
	trainerbattle_single TRAINER_CHANNELER_JENNIFER, PokemonTower_6F_Text_JenniferIntro, PokemonTower_6F_Text_JenniferDefeat
	msgbox PokemonTower_6F_Text_JenniferPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_6F_Text_AngelicaIntro::
	.string "Your future is dark.\p"
	.string "Let me show you!$"

PokemonTower_6F_Text_AngelicaDefeat::
	.string "The spirits abandoned me…$"

PokemonTower_6F_Text_AngelicaPostBattle::
	.string "Your aura is strong.\p"
	.string "Like a true boss.$"

PokemonTower_6F_Text_EmiliaIntro::
	.string "The outbreak woke the ghosts.\p"
	.string "And me!$"

PokemonTower_6F_Text_EmiliaDefeat::
	.string "I did not foresee this!$"

PokemonTower_6F_Text_EmiliaPostBattle::
	.string "Beware the road to VIRIDIAN.$"

PokemonTower_6F_Text_JenniferIntro::
	.string "The ghosts told me you'd come.$"

PokemonTower_6F_Text_JenniferDefeat::
	.string "The visions… were wrong!$"

PokemonTower_6F_Text_JenniferPostBattle::
	.string "The spirits say GIOVANNI fears\n"
	.string "you already.$"

PokemonTower_6F_Text_BeGoneIntruders::
	.string "Be gone…\n"
	.string "Intruders…$"

PokemonTower_6F_Text_GhostWasCubonesMother::
	.string "The ghost was the restless spirit\n"
	.string "of CUBONE's mother!$"

PokemonTower_6F_Text_MothersSpiritWasCalmed::
	.string "The mother's spirit was calmed.\p"
	.string "It finally left this world…$"

