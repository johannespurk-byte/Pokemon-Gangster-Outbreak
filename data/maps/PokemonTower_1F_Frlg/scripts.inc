PokemonTower_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonTower_1F_OnTransition
	.byte 0

PokemonTower_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POKEMON_TOWER_1F
	end

PokemonTower_1F_EventScript_Channeler::
	msgbox PokemonTower_1F_Text_SenseSpiritsUpToMischief, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman1::
	msgbox PokemonTower_1F_Text_GrowlitheWhyDidYouDie, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_BaldingMan::
	msgbox PokemonTower_1F_Text_CameToPrayForDepartedClefairy, MSGBOX_NPC
	end

PokemonTower_1F_EventScript_Woman2::
	lock
	faceplayer
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, PokemonTower_1F_EventScript_Woman2MalePlayer
	msgbox PokemonTower_1F_Text_ComeToPayRespectsGirl
	release
	end

PokemonTower_1F_EventScript_Woman2MalePlayer::
	msgbox PokemonTower_1F_Text_ComeToPayRespectsSon
	release
	end

PokemonTower_1F_EventScript_WorkerF::
	msgbox PokemonTower_1F_Text_ErectedInMemoryOfDeadMons, MSGBOX_NPC
	end

PokemonTower_1F_Text_ErectedInMemoryOfDeadMons::
	.string "POKéMON TOWER was built in memory\n"
	.string "of POKéMON that died.\p"
	.string "Since the outbreak, it fills up\n"
	.string "fast.$"

PokemonTower_1F_Text_ComeToPayRespectsSon::
	.string "Did you come to pay respects?\p"
	.string "Bless you, son. Few dare to come\n"
	.string "here since the ROCKETS moved in.$"

PokemonTower_1F_Text_ComeToPayRespectsGirl::
	.string "Did you come to pay respects?\p"
	.string "Bless you, girl. Few dare to come\n"
	.string "here since the ROCKETS moved in.$"

PokemonTower_1F_Text_CameToPrayForDepartedClefairy::
	.string "I came to pray for my CLEFAIRY.\p"
	.string "It was lost in the gang fights…\n"
	.string "Sniff!$"

PokemonTower_1F_Text_GrowlitheWhyDidYouDie::
	.string "My GROWLITHE… It protected me\n"
	.string "from the ROCKETS. Why did you have\p"
	.string "to die?$"

PokemonTower_1F_Text_SenseSpiritsUpToMischief::
	.string "I am a CHANNELER.\p"
	.string "Since the outbreak, the spirits\n"
	.string "here are restless.\p"
	.string "I sense them high in the TOWER.$"

