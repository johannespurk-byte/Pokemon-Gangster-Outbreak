PokemonLeague_LancesRoom_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_LancesRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, PokemonLeague_LancesRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_LancesRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_LancesRoom_OnFrame
	.byte 0

PokemonLeague_LancesRoom_OnResume::
	setvar VAR_0x8004, 3
	call PokemonLeague_EventScript_DoLightingEffect
	end

PokemonLeague_LancesRoom_OnLoad::
	call_if_eq VAR_MAP_SCENE_POKEMON_LEAGUE, 4, PokemonLeague_LancesRoom_EventScript_CloseEntry
	call_if_set FLAG_DEFEATED_LANCE, PokemonLeague_LancesRoom_EventScript_SetDoorOpen
	end

PokemonLeague_LancesRoom_EventScript_CloseEntry::
	call PokemonLeague_LancesRoom_EventScript_SetEntryClosed
	return

PokemonLeague_LancesRoom_EventScript_SetDoorOpen::
	call PokemonLeague_EventScript_SetDoorOpenLance
	return

PokemonLeague_LancesRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_LancesRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_LancesRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_LancesRoom_OnFrame::
	map_script_2 VAR_MAP_SCENE_POKEMON_LEAGUE, 3, PokemonLeague_LancesRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_LancesRoom_EventScript_EnterRoom::
	lockall
	applymovement LOCALID_PLAYER, PokemonLeague_LancesRoom_Movement_WalkThroughCorridor
	waitmovement 0
	setflag FLAG_TEMP_2
	call PokemonLeague_LancesRoom_EventScript_SetEntryClosed
	playse SE_UNLOCK
	special DrawWholeMapView
	waitse
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 4
	releaseall
	end

PokemonLeague_LancesRoom_EventScript_SetEntryClosed::
	setmetatile 5, 14, METATILE_PokemonLeague_Entry_TopLeft_Closed, 1
	setmetatile 6, 14, METATILE_PokemonLeague_Entry_TopMid_Closed, 1
	setmetatile 7, 14, METATILE_PokemonLeague_Entry_TopRight_Closed, 1
	setmetatile 5, 15, METATILE_PokemonLeague_Entry_BottomLeft_Closed, 1
	setmetatile 6, 15, METATILE_PokemonLeague_Entry_BottomMid_Closed, 1
	setmetatile 7, 15, METATILE_PokemonLeague_Entry_BottomRight_Closed, 1
	setmetatile 6, 16, METATILE_PokemonLeague_Floor_ShadeFull_Lance, 0
	setmetatile 7, 16, METATILE_PokemonLeague_Floor_ShadeFull_Lance, 0
	return

PokemonLeague_LancesRoom_Movement_WalkThroughCorridor::
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

PokemonLeague_LancesRoom_EventScript_Lance::
	lock
	faceplayer
	famechecker FAMECHECKER_LANCE, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	goto_if_set FLAG_DEFEATED_LANCE, PokemonLeague_LancesRoom_EventScript_PostBattle
	famechecker FAMECHECKER_LANCE, 0
	famechecker FAMECHECKER_LANCE, 1
	call_if_unset FLAG_SYS_GAME_CLEAR, PokemonLeague_LancesRoom_EventScript_Intro
	call_if_set FLAG_SYS_GAME_CLEAR, PokemonLeague_LancesRoom_EventScript_RematchIntro
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	call_if_unset FLAG_IS_CHAMPION, PokemonLeague_LancesRoom_EventScript_Battle
	call_if_set FLAG_IS_CHAMPION, PokemonLeague_LancesRoom_EventScript_Rematch
	clearflag FLAG_TEMP_5
	goto PokemonLeague_LancesRoom_EventScript_DefeatedLance
	end

PokemonLeague_LancesRoom_EventScript_Intro::
	msgbox PokemonLeague_LancesRoom_Text_Intro
	return

PokemonLeague_LancesRoom_EventScript_RematchIntro::
	msgbox PokemonLeague_LancesRoom_Text_RematchIntro
	return

PokemonLeague_LancesRoom_EventScript_Battle::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, PokemonLeague_LancesRoom_EventScript_NeedTwoMons
	trainerbattle_two_trainers TRAINER_LEAGUE_VICTOR, PokemonLeague_LancesRoom_Text_Defeat, TRAINER_LEAGUE_GLORIA, PokemonLeague_LancesRoom_Text_DefeatPartner
	return

PokemonLeague_LancesRoom_EventScript_Rematch::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, PokemonLeague_LancesRoom_EventScript_NeedTwoMons
	trainerbattle_two_trainers TRAINER_LEAGUE_VICTOR, PokemonLeague_LancesRoom_Text_Defeat, TRAINER_LEAGUE_GLORIA, PokemonLeague_LancesRoom_Text_DefeatPartner
	return

PokemonLeague_LancesRoom_EventScript_PostBattle::
	msgbox PokemonLeague_LancesRoom_Text_PostBattle
	release
	end

PokemonLeague_LancesRoom_EventScript_DefeatedLance::
	setvar VAR_0x8004, 0
	special Script_TryGainNewFanFromCounterFrlg
	setflag FLAG_DEFEATED_LANCE
	call PokemonLeague_EventScript_OpenDoorLance
	msgbox PokemonLeague_LancesRoom_Text_PostBattle
	closemessage
	release
	end

PokemonLeague_LancesRoom_EventScript_LanceMoveOutOfWayLeft::
	applymovement LOCALID_LANCE, PokemonLeague_LancesRoom_Movement_LanceMoveOutOfWayLeft
	waitmovement 0
	return

PokemonLeague_LancesRoom_EventScript_LanceMoveOutOfWayRight::
	applymovement LOCALID_LANCE, PokemonLeague_LancesRoom_Movement_LanceMoveOutOfWayRight
	waitmovement 0
	return

PokemonLeague_LancesRoom_Movement_LanceMoveOutOfWayLeft::
	walk_left
	walk_in_place_faster_right
	step_end

PokemonLeague_LancesRoom_Movement_LanceMoveOutOfWayRight::
	walk_right
	walk_in_place_faster_left
	step_end

PokemonLeague_LancesRoom_Text_Intro::
	.string "VICTOR: So you made it all the\n"
	.string "way here, {PLAYER}.\p"
	.string "GLORIA: We lead the ELITE FOUR.\n"
	.string "In GALAR we were CHAMPIONS.\p"
	.string "VICTOR: Every battle there was\n"
	.string "a show for thousands of fans.\p"
	.string "GLORIA: So let's give them a show\n"
	.string "they won't forget!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PokemonLeague_LancesRoom_Text_RematchIntro::
	.string "VICTOR: The crowd wants a rematch!\n"
	.string "GLORIA: Let's go!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PokemonLeague_LancesRoom_Text_Defeat::
	.string "That was a championship\nbattle!$"

PokemonLeague_LancesRoom_Text_PostBattle::
	.string "VICTOR: Go on. The CHAMPION is\n"
	.string "waiting in the next room.\p"
	.string "GLORIA: You know him already…\n"
	.string "it's SILVER.$"

PokemonLeague_LancesRoom_EventScript_NeedTwoMons::
	msgbox PokemonLeague_LancesRoom_Text_NeedTwoMons
	release
	end

PokemonLeague_LancesRoom_Text_DefeatPartner::
	.string "You beat the best of\nGALAR…$"

PokemonLeague_LancesRoom_Text_NeedTwoMons::
	.string "We battle as a team.\n"
	.string "Come back with two POKéMON\l"
	.string "that can still fight!$"
