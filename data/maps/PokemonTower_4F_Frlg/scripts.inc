PokemonTower_4F_Frlg_MapScripts::
	.byte 0

PokemonTower_4F_EventScript_Paula::
	trainerbattle_single TRAINER_CHANNELER_PAULA, PokemonTower_4F_Text_PaulaIntro, PokemonTower_4F_Text_PaulaDefeat
	msgbox PokemonTower_4F_Text_PaulaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_4F_EventScript_Laurel::
	trainerbattle_single TRAINER_CHANNELER_LAUREL, PokemonTower_4F_Text_LaurelIntro, PokemonTower_4F_Text_LaurelDefeat
	msgbox PokemonTower_4F_Text_LaurelPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_4F_EventScript_Jody::
	trainerbattle_single TRAINER_CHANNELER_JODY, PokemonTower_4F_Text_JodyIntro, PokemonTower_4F_Text_JodyDefeat
	msgbox PokemonTower_4F_Text_JodyPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_4F_Text_PaulaIntro::
	.string "The spirits whisper of GIOVANNI.\p"
	.string "They are afraid.$"

PokemonTower_4F_Text_PaulaDefeat::
	.string "My mind… is spinning!$"

PokemonTower_4F_Text_PaulaPostBattle::
	.string "Even ghosts avoid TEAM ROCKET.$"

PokemonTower_4F_Text_LaurelIntro::
	.string "Your future is dark.\p"
	.string "Let me show you!$"

PokemonTower_4F_Text_LaurelDefeat::
	.string "The spirits abandoned me…$"

PokemonTower_4F_Text_LaurelPostBattle::
	.string "Your aura is strong.\p"
	.string "Like a true boss.$"

PokemonTower_4F_Text_JodyIntro::
	.string "The outbreak woke the ghosts.\p"
	.string "And me!$"

PokemonTower_4F_Text_JodyDefeat::
	.string "I did not foresee this!$"

PokemonTower_4F_Text_JodyPostBattle::
	.string "Beware the road to VIRIDIAN.$"

