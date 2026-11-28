CeruleanCity_Gym_Frlg_MapScripts::
	.byte 0

CeruleanCity_Gym_EventScript_Misty::
	famechecker FAMECHECKER_MISTY, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_MISTY, CeruleanCity_Gym_Text_MistyIntro, CeruleanCity_Gym_Text_MistyDefeat, CeruleanCity_Gym_EventScript_MistyDefeated, NO_MUSIC
	goto_if_unset FLAG_GOT_TM03_FROM_MISTY, CeruleanCity_Gym_EventScript_GiveTM03
	msgbox CeruleanCity_Gym_Text_ExplainTM03
	release
	end

CeruleanCity_Gym_EventScript_MistyDefeated::
	famechecker FAMECHECKER_MISTY, 1
	setflag FLAG_DEFEATED_MISTY
	setflag FLAG_BADGE02_GET
	setrespawn HEAL_LOCATION_CERULEAN_CITY
	set_gym_trainers_frlg 2
	goto CeruleanCity_Gym_EventScript_GiveTM03
	end

CeruleanCity_Gym_EventScript_GiveTM03::
	msgbox CeruleanCity_Gym_Text_ExplainCascadeBadge
	checkitemspace ITEM_TM03
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_Gym_EventScript_NoRoomForTM03
	giveitem_msg CeruleanCity_Gym_Text_ReceivedTM03FromMisty, ITEM_TM03
	setflag FLAG_GOT_TM03_FROM_MISTY
	msgbox CeruleanCity_Gym_Text_ExplainTM03
	release
	end

CeruleanCity_Gym_EventScript_NoRoomForTM03::
	msgbox CeruleanCity_Gym_Text_BetterMakeRoomForThis
	release
	end

CeruleanCity_Gym_EventScript_Diana::
	trainerbattle_single TRAINER_PICNICKER_DIANA, CeruleanCity_Gym_Text_DianaIntro, CeruleanCity_Gym_Text_DianaDefeat
	msgbox CeruleanCity_Gym_Text_DianaPostBattle, MSGBOX_AUTOCLOSE
	end

CeruleanCity_Gym_EventScript_Luis::
	trainerbattle_single TRAINER_SWIMMER_MALE_LUIS, CeruleanCity_Gym_Text_LuisIntro, CeruleanCity_Gym_Text_LuisDefeat
	famechecker FAMECHECKER_MISTY, 2
	msgbox CeruleanCity_Gym_Text_LuisPostBattle, MSGBOX_AUTOCLOSE
	end

CeruleanCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MISTY, CeruleanCity_Gym_EventScript_GymGuyPostVictory
	msgbox CeruleanCity_Gym_Text_GymGuyAdvice
	release
	end

CeruleanCity_Gym_EventScript_GymGuyPostVictory::
	msgbox CeruleanCity_Gym_Text_WeMakePrettyGoodTeam
	release
	end

CeruleanCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE02_GET, CeruleanCity_Gym_EventScript_GymStatuePostVictory
	msgbox CeruleanCity_Gym_Text_GymStatue
	releaseall
	end

CeruleanCity_Gym_EventScript_GymStatuePostVictory::
	msgbox CeruleanCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

CeruleanCity_Gym_Text_MistyIntro::
	.string "GUZMA! That's my name, the guy who\n"
	.string "beats you down and keeps on going!\p"
	.string "TEAM SKULL owns this GYM now.\n"
	.string "Let's see if you're worth it!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

CeruleanCity_Gym_Text_ExplainTM03::
	.string "TM03 teaches WATER PULSE.\p"
	.string "Use it on an aquatic POKéMON!$"

CeruleanCity_Gym_Text_ExplainCascadeBadge::
	.string "The RUNNER RANK raises your level\n"
	.string "cap to Lv. 32.\p"
	.string "With the HM in your BAG, your first\n"
	.string "POKéMON can CUT small trees, even\p"
	.string "if it never learned the move.\p"
	.string "Here, take my favorite TM too.$"

CeruleanCity_Gym_Text_ReceivedTM03FromMisty::
	.string "{PLAYER} received TM03\n"
	.string "from GUZMA.$"

CeruleanCity_Gym_Text_BetterMakeRoomForThis::
	.string "You better make room for this!$"

CeruleanCity_Gym_Text_MistyDefeat::
	.string "GUZMA: What?! Beat down by a\n"
	.string "rookie?\p"
	.string "…Fine. Take the RUNNER RANK. You\n"
	.string "earned it.$"

CeruleanCity_Gym_Text_DianaIntro::
	.string "This is the GYM of TEAM SKULL.\n"
	.string "Turn back while you can!$"

CeruleanCity_Gym_Text_DianaDefeat::
	.string "Fine… go on ahead.$"

CeruleanCity_Gym_Text_DianaPostBattle::
	.string "Beat GUZMA and the whole of TEAM\n"
	.string "SKULL will respect you.$"

CeruleanCity_Gym_Text_LuisIntro::
	.string "You want a RANK? Earn it the hard\n"
	.string "way!$"

CeruleanCity_Gym_Text_LuisDefeat::
	.string "Tch… You're the real thing.$"

CeruleanCity_Gym_Text_LuisPostBattle::
	.string "I trained for months under GUZMA.\n"
	.string "You still beat me.$"

CeruleanCity_Gym_Text_GymGuyAdvice::
	.string "Yo! Boss in the making!\p"
	.string "GUZMA of TEAM SKULL uses BUG and\n"
	.string "WATER POKéMON.\p"
	.string "FIRE, FLYING and ROCK moves crush\n"
	.string "his bugs. ELECTRIC zaps the water\p"
	.string "ones!$"

CeruleanCity_Gym_Text_WeMakePrettyGoodTeam::
	.string "You beat GUZMA! See, what'd I tell\n"
	.string "ya?\p"
	.string "You and me, kid, we make a pretty\n"
	.string "good crew!$"

CeruleanCity_Gym_Text_GymStatue::
	.string "CERULEAN POKéMON GYM\n"
	.string "LEADER: GUZMA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"

CeruleanCity_Gym_Text_GymStatuePlayerWon::
	.string "CERULEAN POKéMON GYM\n"
	.string "LEADER: GUZMA\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"

