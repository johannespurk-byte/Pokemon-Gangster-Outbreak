OneIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OneIsland_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, OneIsland_OnFrame
	.byte 0

OneIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ONE_ISLAND
	end

OneIsland_OnFrame::
	map_script_2 VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 2, OneIsland_EventScript_EnterOneIslandFirstTime
	.2byte 0

OneIsland_EventScript_EnterOneIslandFirstTime::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, OneIsland_Movement_PlayerExitHarbor
	waitmovement 0
	msgbox OneIsland_Text_BillLetsGoSeeCelio
	closemessage
	applymovement LOCALID_ONE_ISLAND_BILL, OneIsland_Movement_BillWalkToPokeCenter
	applymovement LOCALID_PLAYER, OneIsland_Movement_PlayerWalkToPokeCenter
	waitmovement 0
	opendoor 14, 5
	waitdooranim
	applymovement LOCALID_ONE_ISLAND_BILL, OneIsland_Movement_BillEnterPokeCenter
	applymovement LOCALID_PLAYER, OneIsland_Movement_PlayerEnterPokeCenter
	waitmovement 0
	closedoor 14, 5
	waitdooranim
	removeobject LOCALID_ONE_ISLAND_BILL
	setvar VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 3
	warp MAP_ONE_ISLAND_POKEMON_CENTER_1F, 9, 9
	waitstate
	releaseall
	end

	.macro walk_to_pokecenter
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	.endm

OneIsland_Movement_PlayerWalkToPokeCenter::
	walk_up
	walk_to_pokecenter
	step_end

OneIsland_Movement_PlayerEnterPokeCenter::
	walk_up
	walk_up
	set_invisible
	step_end

OneIsland_Movement_PlayerExitHarbor::
	walk_up
	step_end

OneIsland_Movement_BillWalkToPokeCenter::
	walk_to_pokecenter
	walk_up
	step_end

OneIsland_Movement_BillEnterPokeCenter::
	walk_up
	set_invisible
	step_end

OneIsland_EventScript_OldMan::
	lock
	faceplayer
	goto_if_set FLAG_IS_CHAMPION, OneIsland_EventScript_OldManLinkHoenn
	goto_if_set FLAG_SEVII_DETOUR_FINISHED, OneIsland_EventScript_OldManLinkKanto
	msgbox OneIsland_Text_LuckyToHaveCelioHere
	release
	end

OneIsland_EventScript_OldManLinkKanto::
	msgbox OneIsland_Text_HavePCLinkageWithKanto
	release
	end

OneIsland_EventScript_OldManLinkHoenn::
	msgbox OneIsland_Text_HavePCLinkageWithHoenn
	release
	end

OneIsland_EventScript_BaldingMan::
	msgbox OneIsland_Text_IsntWarmClimateHereGreat, MSGBOX_NPC
	end

OneIsland_EventScript_IslandSign::
	msgbox OneIsland_Text_IslandSign, MSGBOX_SIGN
	end

OneIsland_EventScript_PokemonNetCenterSign::
	msgbox OneIsland_Text_PokemonNetCenterSign, MSGBOX_SIGN
	end

OneIsland_Text_BillLetsGoSeeCelio::
	.string "BILL: Here we are! ONE ISLAND.\p"
	.string "No gang flags, no graffiti. Feels\n"
	.string "weird, right?\p"
	.string "My friend CELIO sent the boat for\n"
	.string "me.\p"
	.string "He runs the island's PC network\n"
	.string "all by himself.\p"
	.string "…And TEAM ROCKET wants it. Let's\n"
	.string "go see CELIO!$"

Text_WhereDoYouWantToSail::
	.string "Ahoy, there!\n"
	.string "Where do you want to sail?$"

OneIsland_Text_LuckyToHaveCelioHere::
	.string "We're on an island in the middle of\n"
	.string "nowhere.\p"
	.string "Lucky for us, the gangs think so\n"
	.string "too.$"

OneIsland_Text_HavePCLinkageWithKanto::
	.string "My granddaughter was in a tizzy.\p"
	.string "We have PC linkage with KANTO now.\p"
	.string "I hope the gangs don't use it to\n"
	.string "find us.$"

OneIsland_Text_HavePCLinkageWithHoenn::
	.string "My granddaughter was in a tizzy.\p"
	.string "We're linked with HOENN now!\p"
	.string "They say HOENN has gangs too.\n"
	.string "AQUA and MAGMA came from there.\p"
	.string "Still, that CELIO would make a fine\n"
	.string "husband for her!$"

OneIsland_Text_IslandSign::
	.string "ONE ISLAND\p"
	.string "Friends Gather Here. Gangs Don't.$"

OneIsland_Text_PokemonNetCenterSign::
	.string "Connecting the Islands to KANTO!\p"
	.string "POKéMON NET CENTER$"

@ Unused
OneIsland_Text_ReturnToCinnabar::
	.string "グレンタウンに　もどりますか？$"

OneIsland_Text_IsntWarmClimateHereGreat::
	.string "Sight-seeing, are you?\p"
	.string "The islands are the last place the\n"
	.string "gangs haven't taken. Yet.$"

