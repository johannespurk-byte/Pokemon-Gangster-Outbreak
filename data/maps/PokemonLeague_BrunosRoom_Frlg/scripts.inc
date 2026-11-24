PokemonLeague_BrunosRoom_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_BrunosRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, PokemonLeague_BrunosRoom_OnLoad
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_BrunosRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_BrunosRoom_OnFrame
	.byte 0

PokemonLeague_BrunosRoom_OnResume::
	setvar VAR_0x8004, 1
	call PokemonLeague_EventScript_DoLightingEffect
	end

PokemonLeague_BrunosRoom_OnLoad::
	call_if_set FLAG_DEFEATED_BRUNO, PokemonLeague_BrunosRoom_EventScript_SetDoorOpen
	call_if_eq VAR_MAP_SCENE_POKEMON_LEAGUE, 2, PokemonLeague_BrunosRoom_EventScript_CloseEntry
	end

PokemonLeague_BrunosRoom_EventScript_CloseEntry::
	call PokemonLeague_EventScript_CloseEntry
	return

PokemonLeague_BrunosRoom_EventScript_SetDoorOpen::
	call PokemonLeague_EventScript_SetDoorOpen
	return

PokemonLeague_BrunosRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_BrunosRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_BrunosRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_BrunosRoom_OnFrame::
	map_script_2 VAR_MAP_SCENE_POKEMON_LEAGUE, 1, PokemonLeague_BrunosRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_BrunosRoom_EventScript_EnterRoom::
	lockall
	call PokemonLeague_EventScript_EnterRoom
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 2
	releaseall
	end

PokemonLeague_BrunosRoom_EventScript_Bruno::
	lock
	faceplayer
	famechecker FAMECHECKER_BRUNO, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	goto_if_set FLAG_DEFEATED_BRUNO, PokemonLeague_BrunosRoom_EventScript_PostBattle
	famechecker FAMECHECKER_BRUNO, 0
	famechecker FAMECHECKER_BRUNO, 1
	call_if_unset FLAG_SYS_GAME_CLEAR, PokemonLeague_BrunosRoom_EventScript_Intro
	call_if_set FLAG_SYS_GAME_CLEAR, PokemonLeague_BrunosRoom_EventScript_RematchIntro
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	call_if_unset FLAG_IS_CHAMPION, PokemonLeague_BrunosRoom_EventScript_Battle
	call_if_set FLAG_IS_CHAMPION, PokemonLeague_BrunosRoom_EventScript_Rematch
	clearflag FLAG_TEMP_5
	goto PokemonLeague_BrunosRoom_EventScript_DefeatedBruno
	end

PokemonLeague_BrunosRoom_EventScript_Intro::
	msgbox PokemonLeague_BrunosRoom_Text_Intro
	return

PokemonLeague_BrunosRoom_EventScript_RematchIntro::
	msgbox PokemonLeague_BrunosRoom_Text_RematchIntro
	return

PokemonLeague_BrunosRoom_EventScript_Battle::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, PokemonLeague_BrunosRoom_EventScript_NeedTwoMons
	trainerbattle_two_trainers TRAINER_LEAGUE_LUCAS, PokemonLeague_BrunosRoom_Text_Defeat, TRAINER_LEAGUE_DAWN, PokemonLeague_BrunosRoom_Text_DefeatPartner
	return

PokemonLeague_BrunosRoom_EventScript_Rematch::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, PokemonLeague_BrunosRoom_EventScript_NeedTwoMons
	trainerbattle_two_trainers TRAINER_LEAGUE_LUCAS, PokemonLeague_BrunosRoom_Text_Defeat, TRAINER_LEAGUE_DAWN, PokemonLeague_BrunosRoom_Text_DefeatPartner
	return

PokemonLeague_BrunosRoom_EventScript_PostBattle::
	msgbox PokemonLeague_BrunosRoom_Text_PostBattle
	closemessage
	call_if_eq VAR_FACING, DIR_NORTH, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayRight
	call_if_eq VAR_FACING, DIR_SOUTH, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayLeft
	call_if_eq VAR_FACING, DIR_WEST, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayDown
	call_if_eq VAR_FACING, DIR_EAST, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayDown
	release
	end

PokemonLeague_BrunosRoom_EventScript_DefeatedBruno::
	setflag FLAG_DEFEATED_BRUNO
	call PokemonLeague_EventScript_OpenDoor
	msgbox PokemonLeague_BrunosRoom_Text_PostBattle
	closemessage
	call_if_eq VAR_FACING, DIR_NORTH, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayRight
	call_if_eq VAR_FACING, DIR_SOUTH, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayLeft
	call_if_eq VAR_FACING, DIR_WEST, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayDown
	call_if_eq VAR_FACING, DIR_EAST, PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayDown
	release
	end

PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayLeft::
	applymovement LOCALID_BRUNO, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayRight::
	applymovement LOCALID_BRUNO, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	return

PokemonLeague_BrunosRoom_EventScript_BrunoLookAwayDown::
	applymovement LOCALID_BRUNO, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	return

PokemonLeague_BrunosRoom_Text_Intro::
	.string "LUCAS: So you beat ETHAN and\n"
	.string "LYRA? Impressive.\p"
	.string "DAWN: We're from SINNOH.\n"
	.string "The cold made us tough!\p"
	.string "LUCAS: Our POKéMON have seen\n"
	.string "mountains, caves and storms.\p"
	.string "DAWN: No need to worry…\n"
	.string "about us, that is!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PokemonLeague_BrunosRoom_Text_RematchIntro::
	.string "LUCAS: You're back!\n"
	.string "DAWN: This time we're ready!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PokemonLeague_BrunosRoom_Text_Defeat::
	.string "This can't be happening…$"

PokemonLeague_BrunosRoom_Text_PostBattle::
	.string "LUCAS: You really are strong.\p"
	.string "DAWN: Go on. UNOVA's heroes\n"
	.string "are waiting for you.$"

PokemonLeague_BrunosRoom_EventScript_NeedTwoMons::
	msgbox PokemonLeague_BrunosRoom_Text_NeedTwoMons
	release
	end

PokemonLeague_BrunosRoom_Text_DefeatPartner::
	.string "No need to worry…\nwe'll be back.$"

PokemonLeague_BrunosRoom_Text_NeedTwoMons::
	.string "We battle as a team.\n"
	.string "Come back with two POKéMON\l"
	.string "that can still fight!$"
