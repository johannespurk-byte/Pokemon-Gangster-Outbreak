Route23_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route23_OnTransition
	.byte 0

Route23_OnTransition::
	clearflag FLAG_HIDE_VICTORY_ROAD_3F_BOULDER
	setflag FLAG_HIDE_VICTORY_ROAD_2F_BOULDER
	setvar VAR_MAP_SCENE_VICTORY_ROAD_1F, 0
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 0
	setvar VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 0
	setvar VAR_MAP_SCENE_VICTORY_ROAD_3F, 0
	end

Route23_EventScript_CascadeBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 2
	bufferstdstring STR_VAR_1, STDSTRING_CASCADE_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_ThunderBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 3
	bufferstdstring STR_VAR_1, STDSTRING_THUNDER_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_RainbowBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 4
	bufferstdstring STR_VAR_1, STDSTRING_RAINBOW_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_SoulBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 5
	bufferstdstring STR_VAR_1, STDSTRING_SOUL_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_MarshBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 6
	bufferstdstring STR_VAR_1, STDSTRING_MARSH_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_VolcanoBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 7
	bufferstdstring STR_VAR_1, STDSTRING_VOLCANO_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_EarthBadgeGuard::
	lock
	faceplayer
	setvar VAR_TEMP_1, 8
	bufferstdstring STR_VAR_1, STDSTRING_EARTH_BADGE
	goto Route23_EventScript_BadgeGuard
	end

Route23_EventScript_CascadeBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 2
	setvar VAR_0x8009, LOCALID_CASCADE_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_CASCADE_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_ThunderBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 3
	setvar VAR_0x8009, LOCALID_THUNDER_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_THUNDER_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_RainbowBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 4
	setvar VAR_0x8009, LOCALID_RAINBOW_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_RAINBOW_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_SoulBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 5
	setvar VAR_0x8009, LOCALID_SOUL_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_SOUL_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_MarshBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 6
	setvar VAR_0x8009, LOCALID_MARSH_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_MARSH_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_VolcanoBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 7
	setvar VAR_0x8009, LOCALID_VOLCANO_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_VOLCANO_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_EarthBadgeGuardTrigger::
	lockall
	setvar VAR_TEMP_1, 8
	setvar VAR_0x8009, LOCALID_EARTH_BADGE_GUARD
	bufferstdstring STR_VAR_1, STDSTRING_EARTH_BADGE
	goto Route23_EventScript_BadgeGuardTrigger
	end

Route23_EventScript_VictoryRoadGateSign::
	msgbox Route23_Text_VictoryRoadGateSign, MSGBOX_SIGN
	end

Route23_Text_VictoryRoadGateSign::
	.string "VICTORY ROAD GATE -\p"
	.string "POKéMON LEAGUE$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route23_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE23_1, Route23_Text_GangTrainer1Intro, Route23_Text_GangTrainer1Defeat
	msgbox Route23_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route23_Text_GangTrainer1Intro:
	.string "GIOVANNI says kids can't fight.\n"
	.string "I'll prove him wrong!$"

Route23_Text_GangTrainer1Defeat:
	.string "No fair! You're way too strong!$"

Route23_Text_GangTrainer1Post:
	.string "Everybody's scared of GIOVANNI.\p"
	.string "Are you scared?$"

Route23_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE23_2, Route23_Text_GangTrainer2Intro, Route23_Text_GangTrainer2Defeat
	msgbox Route23_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route23_Text_GangTrainer2Intro:
	.string "Style is a weapon too, darling!$"

Route23_Text_GangTrainer2Defeat:
	.string "How… vulgar of you to win.$"

Route23_Text_GangTrainer2Post:
	.string "Dirty roads, dirty fights.\p"
	.string "Such is life.$"

Route23_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE23_3, Route23_Text_GangTrainer3Intro, Route23_Text_GangTrainer3Defeat
	msgbox Route23_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route23_Text_GangTrainer3Intro:
	.string "These hills are older than any\n"
	.string "gang.\p"
	.string "So am I!$"

Route23_Text_GangTrainer3Defeat:
	.string "Whoa! Rolled right over me!$"

Route23_Text_GangTrainer3Post:
	.string "Head to VIRIDIAN if you want\n"
	.string "trouble.$"

Route23_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE23_4, Route23_Text_GangTrainer4Intro, Route23_Text_GangTrainer4Defeat
	msgbox Route23_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route23_Text_GangTrainer4Intro:
	.string "Darling, this road is far too dirty\n"
	.string "for you.$"

Route23_Text_GangTrainer4Defeat:
	.string "Oh my. You're rather strong.$"

Route23_Text_GangTrainer4Post:
	.string "The parties in VIRIDIAN are wild\n"
	.string "these days.$"

Route23_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE23_5, Route23_Text_GangTrainer5Intro, Route23_Text_GangTrainer5Defeat
	msgbox Route23_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route23_Text_GangTrainer5Intro:
	.string "Out here, nobody pays the tolls of\n"
	.string "GIOVANNI!$"

Route23_Text_GangTrainer5Defeat:
	.string "My birds flew off…$"

Route23_Text_GangTrainer5Post:
	.string "Rest a bit. The road ahead is\n"
	.string "rough.$"

