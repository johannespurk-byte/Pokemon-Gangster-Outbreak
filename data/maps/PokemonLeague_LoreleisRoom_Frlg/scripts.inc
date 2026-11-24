PokemonLeague_LoreleisRoom_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, PokemonLeague_LoreleisRoom_OnResume
	map_script MAP_SCRIPT_ON_LOAD, PokemonLeague_LoreleisRoom_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, PokemonLeague_LoreleisRoom_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PokemonLeague_LoreleisRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PokemonLeague_LoreleisRoom_OnFrame
	.byte 0

PokemonLeague_LoreleisRoom_OnResume::
	setvar VAR_0x8004, 0
	call PokemonLeague_EventScript_DoLightingEffect
	end

PokemonLeague_LoreleisRoom_OnLoad::
	call_if_set FLAG_DEFEATED_LORELEI, PokemonLeague_LoreleisRoom_EventScript_SetDoorOpen
	call_if_eq VAR_MAP_SCENE_POKEMON_LEAGUE, 1, PokemonLeague_LoreleisRoom_EventScript_CloseEntry
	end

PokemonLeague_LoreleisRoom_EventScript_CloseEntry::
	call PokemonLeague_EventScript_CloseEntry
	return

PokemonLeague_LoreleisRoom_EventScript_SetDoorOpen::
	call PokemonLeague_EventScript_SetDoorOpen
	return

PokemonLeague_LoreleisRoom_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_POKEMON_LEAGUE_LORELEIS_ROOM
	end

PokemonLeague_LoreleisRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, PokemonLeague_LoreleisRoom_EventScript_TurnPlayerNorth
	.2byte 0

PokemonLeague_LoreleisRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

PokemonLeague_LoreleisRoom_OnFrame::
	map_script_2 VAR_MAP_SCENE_POKEMON_LEAGUE, 0, PokemonLeague_LoreleisRoom_EventScript_EnterRoom
	.2byte 0

PokemonLeague_LoreleisRoom_EventScript_EnterRoom::
	lockall
	call PokemonLeague_EventScript_EnterRoom
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 1
	releaseall
	end

PokemonLeague_LoreleisRoom_EventScript_Lorelei::
	lock
	faceplayer
	famechecker FAMECHECKER_LORELEI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	goto_if_set FLAG_DEFEATED_LORELEI, PokemonLeague_LoreleisRoom_EventScript_PostBattle
	famechecker FAMECHECKER_LORELEI, 0
	famechecker FAMECHECKER_LORELEI, 1
	call_if_unset FLAG_IS_CHAMPION, PokemonLeague_LoreleisRoom_EventScript_Intro
	call_if_set FLAG_IS_CHAMPION, PokemonLeague_LoreleisRoom_EventScript_RematchIntro
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	call_if_unset FLAG_IS_CHAMPION, PokemonLeague_LoreleisRoom_EventScript_Battle
	call_if_set FLAG_IS_CHAMPION, PokemonLeague_LoreleisRoom_EventScript_Rematch
	clearflag FLAG_TEMP_5
	goto PokemonLeague_LoreleisRoom_EventScript_DefeatedLorelei
	end

PokemonLeague_LoreleisRoom_EventScript_Intro::
	msgbox PokemonLeague_LoreleisRoom_Text_Intro
	return

PokemonLeague_LoreleisRoom_EventScript_RematchIntro::
	msgbox PokemonLeague_LoreleisRoom_Text_RematchIntro
	return

PokemonLeague_LoreleisRoom_EventScript_Battle::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, PokemonLeague_LoreleisRoom_EventScript_NeedTwoMons
	trainerbattle_two_trainers TRAINER_LEAGUE_ETHAN, PokemonLeague_LoreleisRoom_Text_Defeat, TRAINER_LEAGUE_LYRA, PokemonLeague_LoreleisRoom_Text_DefeatPartner
	return

PokemonLeague_LoreleisRoom_EventScript_Rematch::
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, PokemonLeague_LoreleisRoom_EventScript_NeedTwoMons
	trainerbattle_two_trainers TRAINER_LEAGUE_ETHAN, PokemonLeague_LoreleisRoom_Text_Defeat, TRAINER_LEAGUE_LYRA, PokemonLeague_LoreleisRoom_Text_DefeatPartner
	return

PokemonLeague_LoreleisRoom_EventScript_PostBattle::
	msgbox PokemonLeague_LoreleisRoom_Text_PostBattle
	release
	end

PokemonLeague_LoreleisRoom_EventScript_DefeatedLorelei::
	setflag FLAG_DEFEATED_LORELEI
	call PokemonLeague_EventScript_OpenDoor
	msgbox PokemonLeague_LoreleisRoom_Text_PostBattle
	release
	end

PokemonLeague_LoreleisRoom_Text_Intro::
	.string "ETHAN: Welcome to the POKéMON\n"
	.string "LEAGUE, {PLAYER}!\p"
	.string "LYRA: We came all the way from\n"
	.string "JOHTO to keep the gangs out.\p"
	.string "ETHAN: And now a gangster walks\n"
	.string "right into the ELITE FOUR?\p"
	.string "LYRA: Not while we're here!\n"
	.string "Two against one, ready?{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PokemonLeague_LoreleisRoom_Text_RematchIntro::
	.string "Back again, {PLAYER}?\n"
	.string "JOHTO doesn't give up that easy!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PokemonLeague_LoreleisRoom_Text_Defeat::
	.string "Our partners… beaten?$"

PokemonLeague_LoreleisRoom_Text_PostBattle::
	.string "ETHAN: Fine. The way is open.\p"
	.string "LYRA: But the next room won't\n"
	.string "go easy on you!$"

PokemonLeague_LoreleisRoom_EventScript_NeedTwoMons::
	msgbox PokemonLeague_LoreleisRoom_Text_NeedTwoMons
	release
	end

PokemonLeague_LoreleisRoom_Text_DefeatPartner::
	.string "We should have trained harder…$"

PokemonLeague_LoreleisRoom_Text_NeedTwoMons::
	.string "We battle as a team.\n"
	.string "Come back with two POKéMON\l"
	.string "that can still fight!$"
