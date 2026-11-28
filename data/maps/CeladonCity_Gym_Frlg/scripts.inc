CeladonCity_Gym_Frlg_MapScripts::
	.byte 0

CeladonCity_Gym_EventScript_Erika::
	famechecker FAMECHECKER_ERIKA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_ERIKA, CeladonCity_Gym_EventScript_ArchieMaxieBeaten
	msgbox CeladonCity_Gym_Text_ErikaIntro
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, CeladonCity_Gym_EventScript_ArchieMaxieNeedTwo
	trainerbattle_two_trainers TRAINER_LEADER_ERIKA, CeladonCity_Gym_Text_ErikaDefeat, TRAINER_GANG_MAXIE, CeladonCity_Gym_Text_MaxieDefeat
	goto CeladonCity_Gym_EventScript_DefeatedErika
CeladonCity_Gym_EventScript_ArchieMaxieBeaten::
	goto_if_unset FLAG_GOT_TM19_FROM_ERIKA, CeladonCity_Gym_EventScript_GiveTM19
	call EventScript_GangGiveZRing
	famechecker FAMECHECKER_ERIKA, 4
	msgbox CeladonCity_Gym_Text_ErikaPostBattle
	release
	end

CeladonCity_Gym_EventScript_DefeatedErika::
	famechecker FAMECHECKER_ERIKA, 1
	clearflag FLAG_HIDE_FAME_CHECKER_ERIKA_JOURNALS
	setflag FLAG_DEFEATED_ERIKA
	setflag FLAG_BADGE04_GET
	set_gym_trainers_frlg 4
	goto CeladonCity_Gym_EventScript_GiveTM19
	end

CeladonCity_Gym_EventScript_GiveTM19::
	msgbox CeladonCity_Gym_Text_ExplainRainbowBadgeTakeThis
	checkitemspace ITEM_TM19
	goto_if_eq VAR_RESULT, FALSE, CeladonCity_Gym_EventScript_NoRoomForTM19
	giveitem_msg CeladonCity_Gym_Text_ReceivedTM19FromErika, ITEM_TM19
	setflag FLAG_GOT_TM19_FROM_ERIKA
	call EventScript_GangGiveZRing
	msgbox CeladonCity_Gym_Text_ExplainTM19
	release
	end

CeladonCity_Gym_EventScript_NoRoomForTM19::
	msgbox CeladonCity_Gym_Text_ShouldMakeRoomForThis
	release
	end

CeladonCity_Gym_EventScript_Kay::
	trainerbattle_single TRAINER_LASS_KAY, CeladonCity_Gym_Text_KayIntro, CeladonCity_Gym_Text_KayDefeat
	msgbox CeladonCity_Gym_Text_KayPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Lisa::
	trainerbattle_single TRAINER_LASS_LISA, CeladonCity_Gym_Text_LisaIntro, CeladonCity_Gym_Text_LisaDefeat
	famechecker FAMECHECKER_ERIKA, 2
	msgbox CeladonCity_Gym_Text_LisaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Tina::
	trainerbattle_single TRAINER_PICNICKER_TINA, CeladonCity_Gym_Text_TinaIntro, CeladonCity_Gym_Text_TinaDefeat
	msgbox CeladonCity_Gym_Text_TinaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Bridget::
	trainerbattle_single TRAINER_BEAUTY_BRIDGET, CeladonCity_Gym_Text_BridgetIntro, CeladonCity_Gym_Text_BridgetDefeat
	msgbox CeladonCity_Gym_Text_BridgetPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Tamia::
	trainerbattle_single TRAINER_BEAUTY_TAMIA, CeladonCity_Gym_Text_TamiaIntro, CeladonCity_Gym_Text_TamiaDefeat
	famechecker FAMECHECKER_ERIKA, 3
	msgbox CeladonCity_Gym_Text_TamiaPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Lori::
	trainerbattle_single TRAINER_BEAUTY_LORI, CeladonCity_Gym_Text_LoriIntro, CeladonCity_Gym_Text_LoriDefeat
	msgbox CeladonCity_Gym_Text_LoriPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_Mary::
	trainerbattle_single TRAINER_COOLTRAINER_MARY, CeladonCity_Gym_Text_MaryIntro, CeladonCity_Gym_Text_MaryDefeat
	msgbox CeladonCity_Gym_Text_MaryPostBattle, MSGBOX_AUTOCLOSE
	end

CeladonCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE04_GET, CeladonCity_Gym_EventScript_GymStatuePostVictory
	msgbox CeladonCity_Gym_Text_GymStatue
	releaseall
	end

CeladonCity_Gym_EventScript_GymStatuePostVictory::
	msgbox CeladonCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CeladonCity_Gym_Text_ErikaIntro::
	.string "ARCHIE: Bwahahaha! TEAM AQUA wants\n"
	.string "the sea, TEAM MAGMA wants the land.\p"
	.string "MAXIE: For once we fight together.\n"
	.string "Two bosses, one GYM. Ready?$"

CeladonCity_Gym_Text_ErikaDefeat::
	.string "ARCHIE: Bwahahaha! Beaten, and by\n"
	.string "a kid!\p"
	.string "MAXIE: Logic says you deserve the\n"
	.string "ENFORCER RANK.$"

CeladonCity_Gym_Text_ErikaPostBattle::
	.string "ARCHIE: Land or sea, you rule both\n"
	.string "now. Bwahahaha!$"

CeladonCity_Gym_Text_ExplainRainbowBadgeTakeThis::
	.string "The ENFORCER RANK raises your\n"
	.string "level cap to Lv. 44.\p"
	.string "With the HM in your BAG, your first\n"
	.string "POKéMON can use STRENGTH outside\p"
	.string "of battle.\p"
	.string "Take this with you too.$"

CeladonCity_Gym_Text_ReceivedTM19FromErika::
	.string "{PLAYER} received TM19\n"
	.string "from ARCHIE.$"

CeladonCity_Gym_Text_ExplainTM19::
	.string "TM19 contains GIGA DRAIN.\p"
	.string "Half the damage it deals heals\n"
	.string "your POKéMON.\p"
	.string "MAXIE calls it efficient. I call it\n"
	.string "greedy. Bwahaha!$"

CeladonCity_Gym_Text_ShouldMakeRoomForThis::
	.string "You should make room for this.$"

CeladonCity_Gym_Text_KayIntro::
	.string "This GYM belongs to TEAM AQUA and\n"
	.string "TEAM MAGMA. Pick a side or get\p"
	.string "lost!$"

CeladonCity_Gym_Text_KayDefeat::
	.string "I lost?! In my own GYM?!$"

CeladonCity_Gym_Text_KayPostBattle::
	.string "Every gang in KANTO is watching\n"
	.string "this GYM.$"

CeladonCity_Gym_Text_BridgetIntro::
	.string "Nobody reaches ARCHIE without\n"
	.string "going through me!$"

CeladonCity_Gym_Text_BridgetDefeat::
	.string "Tch… You're the real thing.$"

CeladonCity_Gym_Text_BridgetPostBattle::
	.string "ARCHIE is waiting. Don't keep the\n"
	.string "boss waiting.$"

CeladonCity_Gym_Text_TinaIntro::
	.string "Welcome to our turf. Hope you\n"
	.string "brought REVIVES!$"

CeladonCity_Gym_Text_TinaDefeat::
	.string "Fine… go on ahead.$"

CeladonCity_Gym_Text_TinaPostBattle::
	.string "If you win, you're one step closer\n"
	.string "to the LEAGUE.$"

CeladonCity_Gym_Text_TamiaIntro::
	.string "This is the GYM of TEAM AQUA and\n"
	.string "TEAM MAGMA. Turn back while you\p"
	.string "can!$"

CeladonCity_Gym_Text_TamiaDefeat::
	.string "No way! ARCHIE is gonna be mad!$"

CeladonCity_Gym_Text_TamiaPostBattle::
	.string "Beat ARCHIE and the whole of TEAM\n"
	.string "AQUA and TEAM MAGMA will respect\p"
	.string "you.$"

CeladonCity_Gym_Text_LisaIntro::
	.string "Uhuhuhu…\p"
	.string "I'm TABITHA, MAXIE's right hand.\n"
	.string "You won't get near him!$"

CeladonCity_Gym_Text_LisaDefeat::
	.string "Uhuhu… Impossible!\n"
	.string "Beaten by a brat!$"

CeladonCity_Gym_Text_LisaPostBattle::
	.string "MAXIE and ARCHIE side by side…\n"
	.string "You don't stand a chance. Uhuhu…$"

CeladonCity_Gym_Text_LoriIntro::
	.string "You want a RANK? Earn it the hard\n"
	.string "way!$"

CeladonCity_Gym_Text_LoriDefeat::
	.string "I lost?! In my own GYM?!$"

CeladonCity_Gym_Text_LoriPostBattle::
	.string "I trained for months under\n"
	.string "ARCHIE. You still beat me.$"

CeladonCity_Gym_Text_MaryIntro::
	.string "ARCHIE doesn't waste time on\n"
	.string "rookies. Neither do I!$"

CeladonCity_Gym_Text_MaryDefeat::
	.string "Fine… go on ahead.$"

CeladonCity_Gym_Text_MaryPostBattle::
	.string "Go on. Show ARCHIE what you've\n"
	.string "got.$"

CeladonCity_Gym_Text_GymStatue::
	.string "CELADON POKéMON GYM\n"
	.string "LEADER: ARCHIE\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"

CeladonCity_Gym_Text_GymStatuePlayerWon::
	.string "CELADON POKéMON GYM\n"
	.string "LEADER: ARCHIE\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"


CeladonCity_Gym_EventScript_ArchieMaxieNeedTwo::
	msgbox CeladonCity_Gym_Text_NeedTwo
	release
	end

CeladonCity_Gym_Text_MaxieDefeat::
	.string "MAXIE: Fuhahaha… Remarkable.$"

CeladonCity_Gym_Text_NeedTwo::
	.string "ARCHIE: Come back with at least two\n"
	.string "POKéMON that can still fight!$"
