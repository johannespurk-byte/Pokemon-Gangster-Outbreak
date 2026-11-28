SevenIsland_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

SevenIsland_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_SEVEN_ISLAND
	end

SevenIsland_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

SevenIsland_PokemonCenter_1F_EventScript_OldMan::
	msgbox SevenIsland_PokemonCenter_1F_Text_EventsDependingOnConnections, MSGBOX_NPC
	end

SevenIsland_PokemonCenter_1F_EventScript_Sailor::
	msgbox SevenIsland_PokemonCenter_1F_Text_DidYouRideSeagallopHere, MSGBOX_NPC
	end

SevenIsland_PokemonCenter_1F_EventScript_Beauty::
	msgbox SevenIsland_PokemonCenter_1F_Text_CheckOutTrainerTower, MSGBOX_NPC
	end

SevenIsland_PokemonCenter_1F_Text_EventsDependingOnConnections::
	.string "On this island, different things\n"
	.string "happen depending on connections.\p"
	.string "What connections?\p"
	.string "Find out yourself. That's how it\n"
	.string "works on the streets, too.$"

SevenIsland_PokemonCenter_1F_Text_DidYouRideSeagallopHere::
	.string "Did you ride a SEAGALLOP ferry\n"
	.string "here?\p"
	.string "They're fast. The gangs can't\n"
	.string "catch them!$"

SevenIsland_PokemonCenter_1F_Text_CheckOutTrainerTower::
	.string "If you're confident, check out\n"
	.string "TRAINER TOWER. No gangs, just\p"
	.string "strong TRAINERS!$"

