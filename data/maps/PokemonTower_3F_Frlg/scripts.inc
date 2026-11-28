PokemonTower_3F_Frlg_MapScripts::
	.byte 0

PokemonTower_3F_EventScript_Patricia::
	trainerbattle_single TRAINER_CHANNELER_PATRICIA, PokemonTower_3F_Text_PatriciaIntro, PokemonTower_3F_Text_PatriciaDefeat
	msgbox PokemonTower_3F_Text_PatriciaPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_3F_EventScript_Carly::
	trainerbattle_single TRAINER_CHANNELER_CARLY, PokemonTower_3F_Text_CarlyIntro, PokemonTower_3F_Text_CarlyDefeat
	msgbox PokemonTower_3F_Text_CarlyPostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_3F_EventScript_Hope::
	trainerbattle_single TRAINER_CHANNELER_HOPE, PokemonTower_3F_Text_HopeIntro, PokemonTower_3F_Text_HopeDefeat
	msgbox PokemonTower_3F_Text_HopePostBattle, MSGBOX_AUTOCLOSE
	end

PokemonTower_3F_Text_HopeIntro::
	.string "The ghosts told me you'd come.$"

PokemonTower_3F_Text_HopeDefeat::
	.string "The visions… were wrong!$"

PokemonTower_3F_Text_HopePostBattle::
	.string "The ghosts can be identified by\n"
	.string "the SILPH SCOPE.$"

PokemonTower_3F_Text_CarlyIntro::
	.string "The outbreak woke the ghosts.\p"
	.string "And me!$"

PokemonTower_3F_Text_CarlyDefeat::
	.string "I did not foresee this!$"

PokemonTower_3F_Text_CarlyPostBattle::
	.string "Beware the road to VIRIDIAN.$"

PokemonTower_3F_Text_PatriciaIntro::
	.string "Your future is dark.\p"
	.string "Let me show you!$"

PokemonTower_3F_Text_PatriciaDefeat::
	.string "The spirits abandoned me…$"

PokemonTower_3F_Text_PatriciaPostBattle::
	.string "Your aura is strong.\p"
	.string "Like a true boss.$"

