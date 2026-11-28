FuchsiaCity_Gym_Frlg_MapScripts::
	.byte 0

FuchsiaCity_Gym_EventScript_Koga::
	famechecker FAMECHECKER_KOGA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_KOGA, FuchsiaCity_Gym_Text_KogaIntro, FuchsiaCity_Gym_Text_KogaDefeat, FuchsiaCity_Gym_EventScript_DefeatedKoga, NO_MUSIC
	goto_if_unset FLAG_GOT_TM06_FROM_KOGA, FuchsiaCity_Gym_EventScript_GiveTM06
	msgbox FuchsiaCity_Gym_Text_KogaPostBattle
	release
	end

FuchsiaCity_Gym_EventScript_DefeatedKoga::
	famechecker FAMECHECKER_KOGA, 1
	clearflag FLAG_HIDE_FAME_CHECKER_KOGA_JOURNAL
	setflag FLAG_DEFEATED_KOGA
	setflag FLAG_BADGE05_GET
	set_gym_trainers_frlg 5
	goto FuchsiaCity_Gym_EventScript_GiveTM06
	end

FuchsiaCity_Gym_EventScript_GiveTM06::
	msgbox FuchsiaCity_Gym_Text_KogaExplainSoulBadge
	checkitemspace ITEM_TM06
	goto_if_eq VAR_RESULT, FALSE, FuchsiaCity_Gym_EventScript_NoRoomForTM06
	giveitem_msg FuchsiaCity_Gym_Text_ReceivedTM06FromKoga, ITEM_TM06
	setflag FLAG_GOT_TM06_FROM_KOGA
	msgbox FuchsiaCity_Gym_Text_KogaExplainTM06
	release
	end

FuchsiaCity_Gym_EventScript_NoRoomForTM06::
	msgbox FuchsiaCity_Gym_Text_MakeSpaceForThis
	release
	end

FuchsiaCity_Gym_EventScript_Phil::
	trainerbattle_single TRAINER_TAMER_PHIL, FuchsiaCity_Gym_Text_PhilIntro, FuchsiaCity_Gym_Text_PhilDefeat
	msgbox FuchsiaCity_Gym_Text_PhilPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Edgar::
	trainerbattle_single TRAINER_TAMER_EDGAR, FuchsiaCity_Gym_Text_EdgarIntro, FuchsiaCity_Gym_Text_EdgarDefeat
	msgbox FuchsiaCity_Gym_Text_EdgarPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Kirk::
	trainerbattle_single TRAINER_JUGGLER_KIRK, FuchsiaCity_Gym_Text_KirkIntro, FuchsiaCity_Gym_Text_KirkDefeat
	famechecker FAMECHECKER_KOGA, 2
	msgbox FuchsiaCity_Gym_Text_KirkPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Shawn::
	trainerbattle_single TRAINER_JUGGLER_SHAWN, FuchsiaCity_Gym_Text_ShawnIntro, FuchsiaCity_Gym_Text_ShawnDefeat
	msgbox FuchsiaCity_Gym_Text_ShawnPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Kayden::
	trainerbattle_single TRAINER_JUGGLER_KAYDEN, FuchsiaCity_Gym_Text_KaydenIntro, FuchsiaCity_Gym_Text_KaydenDefeat
	msgbox FuchsiaCity_Gym_Text_KaydenPostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_Nate::
	trainerbattle_single TRAINER_JUGGLER_NATE, FuchsiaCity_Gym_Text_NateIntro, FuchsiaCity_Gym_Text_NateDefeat
	msgbox FuchsiaCity_Gym_Text_NatePostBattle, MSGBOX_AUTOCLOSE
	end

FuchsiaCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_KOGA, FuchsiaCity_Gym_EventScript_GymGuyPostVictory
	msgbox FuchsiaCity_Gym_Text_GymGuyAdvice
	release
	end

FuchsiaCity_Gym_EventScript_GymGuyPostVictory::
	msgbox FuchsiaCity_Gym_Text_GymGuyPostVictory
	release
	end

FuchsiaCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE05_GET, FuchsiaCity_Gym_EventScript_GymStatuePostVictory
	msgbox FuchsiaCity_Gym_Text_GymStatue
	releaseall
	end

FuchsiaCity_Gym_EventScript_GymStatuePostVictory::
	msgbox FuchsiaCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

FuchsiaCity_Gym_Text_KogaIntro::
	.string "I am GHETSIS of TEAM PLASMA.\p"
	.string "POKéMON exist to serve the ones\n"
	.string "who rule. Kneel before my power!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

FuchsiaCity_Gym_Text_KogaDefeat::
	.string "GHETSIS: Impossible! A mere pawn…\p"
	.string "Take the HITMAN RANK. For now.$"

FuchsiaCity_Gym_Text_KogaPostBattle::
	.string "Hmph. Even I can use a pawn as\n"
	.string "strong as you.$"

FuchsiaCity_Gym_Text_KogaExplainSoulBadge::
	.string "The HITMAN RANK raises your level\n"
	.string "cap to Lv. 49.\p"
	.string "With the HM in your BAG, your first\n"
	.string "POKéMON can SURF outside of\p"
	.string "battle.\p"
	.string "Take this too.$"

FuchsiaCity_Gym_Text_ReceivedTM06FromKoga::
	.string "{PLAYER} received TM06\n"
	.string "from GHETSIS.$"

FuchsiaCity_Gym_Text_KogaExplainTM06::
	.string "Sealed within that TM06 lies\n"
	.string "TOXIC!\p"
	.string "It is a secret technique dating\n"
	.string "back some four hundred years.$"

FuchsiaCity_Gym_Text_MakeSpaceForThis::
	.string "Make space for this, child!$"

FuchsiaCity_Gym_Text_KaydenIntro::
	.string "Strength isn't the key for POKéMON.\n"
	.string "Do you understand this?\p"
	.string "POKéMON is about strategy!\p"
	.string "I'll show you how strategy can\n"
	.string "beat brute strength.$"

FuchsiaCity_Gym_Text_KaydenDefeat::
	.string "No way! GHETSIS is gonna be mad!$"

FuchsiaCity_Gym_Text_KaydenPostBattle::
	.string "Beat GHETSIS and the whole of\n"
	.string "TEAM PLASMA will respect you.$"

FuchsiaCity_Gym_Text_KirkIntro::
	.string "Welcome to our turf. Hope you\n"
	.string "brought REVIVES!$"

FuchsiaCity_Gym_Text_KirkDefeat::
	.string "Fine… go on ahead.$"

FuchsiaCity_Gym_Text_KirkPostBattle::
	.string "If you win, you're one step closer\n"
	.string "to the LEAGUE.$"

FuchsiaCity_Gym_Text_NateIntro::
	.string "You want a RANK? Earn it the hard\n"
	.string "way!$"

FuchsiaCity_Gym_Text_NateDefeat::
	.string "I lost?! In my own GYM?!$"

FuchsiaCity_Gym_Text_NatePostBattle::
	.string "I trained for months under\n"
	.string "GHETSIS. You still beat me.$"

FuchsiaCity_Gym_Text_PhilIntro::
	.string "Stop right there!\p"
	.string "The famed invisible walls of\n"
	.string "FUCHSIA GYM have you frustrated?$"

FuchsiaCity_Gym_Text_PhilDefeat::
	.string "No way! GHETSIS is gonna be mad!$"

FuchsiaCity_Gym_Text_PhilPostBattle::
	.string "You impressed me!\n"
	.string "Here's a hint!\p"
	.string "Look very closely for gaps in the\n"
	.string "invisible walls!$"

FuchsiaCity_Gym_Text_EdgarIntro::
	.string "Another challenger? GHETSIS will\n"
	.string "love this.$"

FuchsiaCity_Gym_Text_EdgarDefeat::
	.string "I lost?! In my own GYM?!$"

FuchsiaCity_Gym_Text_EdgarPostBattle::
	.string "Every gang in KANTO is watching\n"
	.string "this GYM.$"

FuchsiaCity_Gym_Text_ShawnIntro::
	.string "Nobody reaches GHETSIS without\n"
	.string "going through me!$"

FuchsiaCity_Gym_Text_ShawnDefeat::
	.string "Tch… You're the real thing.$"

FuchsiaCity_Gym_Text_ShawnPostBattle::
	.string "GHETSIS is waiting. Don't keep the\n"
	.string "boss waiting.$"

FuchsiaCity_Gym_Text_GymGuyAdvice::
	.string "Yo! Boss in the making!\p"
	.string "This GYM is full of invisible walls!\n"
	.string "GHETSIS looks close, but he's\p"
	.string "blocked off.\p"
	.string "Find the gaps in the walls to reach\n"
	.string "him.\p"
	.string "His HYDREIGON is DARK and DRAGON.\n"
	.string "FAIRY and FIGHTING moves hit it\p"
	.string "hard!$"

FuchsiaCity_Gym_Text_GymGuyPostVictory::
	.string "You beat GHETSIS! TEAM PLASMA is\n"
	.string "shaking!$"

FuchsiaCity_Gym_Text_GymStatue::
	.string "FUCHSIA POKéMON GYM\n"
	.string "LEADER: GHETSIS\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"

FuchsiaCity_Gym_Text_GymStatuePlayerWon::
	.string "FUCHSIA POKéMON GYM\n"
	.string "LEADER: GHETSIS\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"

