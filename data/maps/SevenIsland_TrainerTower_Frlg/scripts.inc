SevenIsland_TrainerTower_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SevenIsland_TrainerTower_OnTransition
	.byte 0

SevenIsland_TrainerTower_OnTransition::
	setvar VAR_MAP_SCENE_TRAINER_TOWER, 0
	end

SevenIsland_TrainerTower_EventScript_TrainerTowerSign::
	msgbox SevenIsland_TrainerTower_Text_TrainerTowerSign, MSGBOX_SIGN
	end

SevenIsland_TrainerTower_EventScript_TrainerTowerAheadSign::
	msgbox SevenIsland_TrainerTower_Text_TrainerTowerAhead, MSGBOX_SIGN
	end

SevenIsland_TrainerTower_Text_DarioIntro::
	.string "The outbreak woke the ghosts.\p"
	.string "And me!$"

SevenIsland_TrainerTower_Text_DarioDefeat::
	.string "My mind… is spinning!$"

SevenIsland_TrainerTower_Text_DarioPostBattle::
	.string "Beware the road to the SEVII\n"
	.string "ISLANDS.$"

SevenIsland_TrainerTower_Text_RodetteIntro::
	.string "The ghosts told me you'd come.$"

SevenIsland_TrainerTower_Text_RodetteDefeat::
	.string "The spirits abandoned me…$"

SevenIsland_TrainerTower_Text_RodettePostBattle::
	.string "The spirits say ARCHER fears you\n"
	.string "already.$"

SevenIsland_TrainerTower_Text_TrainerTowerSign::
	.string "TRAINER TOWER ENTRANCE\p"
	.string "Rise to the Challenge! Gang\n"
	.string "Bosses Welcome!$"

SevenIsland_TrainerTower_Text_TrainerTowerAhead::
	.string "TRAINER TOWER AHEAD$"

