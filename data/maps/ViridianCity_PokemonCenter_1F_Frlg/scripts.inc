ViridianCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

ViridianCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VIRIDIAN_CITY
	end

ViridianCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

ViridianCity_PokemonCenter_1F_EventScript_Boy::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCenterInEveryTown, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Gentleman::
	msgbox ViridianCity_PokemonCenter_1F_Text_FeelFreeToUsePC, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox ViridianCity_PokemonCenter_1F_Text_PokeCentersHealMons, MSGBOX_NPC
	end

ViridianCity_PokemonCenter_1F_Text_FeelFreeToUsePC::
	.string "Use the PC in the corner. It still\n"
	.string "works.\p"
	.string "TEAM ROCKET reads everything on\n"
	.string "it, though.$"

ViridianCity_PokemonCenter_1F_Text_PokeCenterInEveryTown::
	.string "There's a HEAL-BOT in every town\n"
	.string "ahead.\p"
	.string "They're free, so heal up before\n"
	.string "every fight.$"

ViridianCity_PokemonCenter_1F_Text_PokeCentersHealMons::
	.string "The nurses fled, but the\n"
	.string "HEAL-BOTS stayed.\p"
	.string "They fix any POKéMON. And they\n"
	.string "never ask questions.$"

