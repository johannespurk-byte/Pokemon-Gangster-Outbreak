PewterCity_Gym_Frlg_MapScripts::
	.byte 0

PewterCity_Gym_EventScript_Brock::
	famechecker FAMECHECKER_BROCK, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_BROCK, PewterCity_Gym_Text_BrockIntro, PewterCity_Gym_Text_BrockDefeat, PewterCity_Gym_EventScript_DefeatedBrock, NO_MUSIC
	goto_if_unset FLAG_GOT_TM39_FROM_BROCK, PewterCity_Gym_EventScript_GiveTM39
	msgbox PewterCity_Gym_Text_BrockPostBattle
	release
	end

PewterCity_Gym_EventScript_DefeatedBrock::
	famechecker FAMECHECKER_BROCK, 1
	setflag FLAG_DEFEATED_BROCK
	setflag FLAG_BADGE01_GET
	setvar VAR_MAP_SCENE_PEWTER_CITY, 1
	setflag FLAG_HIDE_PEWTER_CITY_GYM_GUIDE
	clearflag FLAG_HIDE_PEWTER_CITY_RUNNING_SHOES_GUY
	set_gym_trainers_frlg 1
	goto PewterCity_Gym_EventScript_GiveTM39
	end

PewterCity_Gym_EventScript_GiveTM39::
	msgbox PewterCity_Gym_Text_TakeThisWithYou
	checkitemspace ITEM_TM39
	goto_if_eq VAR_RESULT, FALSE, PewterCity_Gym_EventScript_NoRoomForTM39
	giveitem_msg PewterCity_Gym_Text_ReceivedTM39FromBrock, ITEM_TM39
	setflag FLAG_GOT_TM39_FROM_BROCK
	msgbox PewterCity_Gym_Text_ExplainTM39
	release
	end

PewterCity_Gym_EventScript_NoRoomForTM39::
	msgbox PewterCity_Gym_Text_DontHaveRoomForThis
	release
	end

PewterCity_Gym_EventScript_Liam::
	trainerbattle_single TRAINER_CAMPER_LIAM, PewterCity_Gym_Text_LiamIntro, PewterCity_Gym_Text_LiamDefeat
	msgbox PewterCity_Gym_Text_LiamPostBattle, MSGBOX_AUTOCLOSE
	end

PewterCity_Gym_EventScript_Marla::
	trainerbattle_single TRAINER_GANG_YELL_MARLA, PewterCity_Gym_Text_MarlaIntro, PewterCity_Gym_Text_MarlaDefeat
	msgbox PewterCity_Gym_Text_MarlaPostBattle, MSGBOX_AUTOCLOSE
	end

PewterCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_BROCK, PewterCity_Gym_EventScript_GymGuyPostVictory
	msgbox PewterCity_Gym_Text_LetMeTakeYouToTheTop, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, PewterCity_Gym_EventScript_GymGuyTakeMeToTop
	goto_if_eq VAR_RESULT, NO, PewterCity_Gym_EventScript_GymGuyDontTakeMeToTop
	end

PewterCity_Gym_EventScript_GymGuyPostVictory::
	msgbox PewterCity_Gym_Text_YoureChampMaterial
	release
	end

PewterCity_Gym_EventScript_GymGuyTakeMeToTop::
	msgbox PewterCity_Gym_Text_LetsGetHappening
	goto PewterCity_Gym_EventScript_GymGuyAdvice
	end

PewterCity_Gym_EventScript_GymGuyDontTakeMeToTop::
	msgbox PewterCity_Gym_Text_ItsFreeLetsGetHappening
	goto PewterCity_Gym_EventScript_GymGuyAdvice
	end

PewterCity_Gym_EventScript_GymGuyAdvice::
	msgbox PewterCity_Gym_Text_TryDifferentPartyOrders
	release
	end

PewterCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE01_GET, PewterCity_Gym_EventScript_GymStatuePostVictory
	msgbox PewterCity_Gym_Text_GymStatue
	releaseall
	end

PewterCity_Gym_EventScript_GymStatuePostVictory::
	msgbox PewterCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

PewterCity_Gym_Text_BrockIntro::
	.string "I'm PIERS, boss of TEAM YELL.\n"
	.string "Our gang used to cheer for the\l"
	.string "GYM challengers in GALAR.\p"
	.string "Now we run this GYM. Show me some\n"
	.string "noise, GIOVANNI's new blood!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

PewterCity_Gym_Text_BrockDefeat::
	.string "I took you for granted, and so\n"
	.string "I lost.\p"
	.string "Word on the street is you're GIOVANNI's\n"
	.string "new blood. Fine. You earned your\l"
	.string "first stripe.\p"
	.string "{FONT_NORMAL}{PLAYER} was promoted to\n"
	.string "PROSPECT!{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}\p"
	.string "{FONT_MALE}The PROSPECT RANK raises your\n"
	.string "level cap to Lv. 27.\p"
	.string "With the HM in your BAG, your first\n"
	.string "POKéMON can use FLASH, even if it\l"
	.string "never learned the move.$"

PewterCity_Gym_Text_TakeThisWithYou::
	.string "Wait!\n"
	.string "Take this with you.$"

PewterCity_Gym_Text_ReceivedTM39FromBrock::
	.string "{PLAYER} received TM39\n"
	.string "from PIERS.$"

PewterCity_Gym_Text_ExplainTM39::
	.string "A TM teaches a POKéMON a new move.\p"
	.string "Use it on the right POKéMON. The\n"
	.string "vending machines don't restock\p"
	.string "them.\p"
	.string "TM39 contains ROCK TOMB.\p"
	.string "It buries the foe in rocks and\n"
	.string "lowers its SPEED. Rock solid, like\p"
	.string "TEAM YELL.$"

PewterCity_Gym_Text_BrockPostBattle::
	.string "Heh. TEAM YELL will cheer for you\n"
	.string "from now on. Don't let us down.$"

PewterCity_Gym_Text_DontHaveRoomForThis::
	.string "You don't have room for this.$"

PewterCity_Gym_Text_LiamIntro::
	.string "Welcome to our turf. Hope you\n"
	.string "brought REVIVES!$"

PewterCity_Gym_Text_LiamDefeat::
	.string "No way! PIERS is gonna be mad!$"

PewterCity_Gym_Text_LiamPostBattle::
	.string "If you win, you're one step closer\n"
	.string "to the LEAGUE.$"

PewterCity_Gym_Text_MarlaIntro::
	.string "Oi! Nobody gets to PIERS without\n"
	.string "gettin' past TEAM YELL first!$"

PewterCity_Gym_Text_MarlaDefeat::
	.string "Ugh… Can't even yell\n"
	.string "loud enough to win!$"

PewterCity_Gym_Text_MarlaPostBattle::
	.string "Fine, go on. PIERS'll show ya\n"
	.string "what real noise sounds like!$"

PewterCity_Gym_Text_LetMeTakeYouToTheTop::
	.string "Hiya! Do you dream big?\p"
	.string "Do you dare to dream of becoming\n"
	.string "the KINGPIN of KANTO?\p"
	.string "I'm no fighter, but I know every\n"
	.string "boss. Let me take you to the top!$"

PewterCity_Gym_Text_LetsGetHappening::
	.string "All right!\n"
	.string "Let's get happening!$"

PewterCity_Gym_Text_TryDifferentPartyOrders::
	.string "The first POKéMON in your list\n"
	.string "goes out first.\p"
	.string "Know your opponent and change\n"
	.string "the order.\p"
	.string "That's how TEAM YELL wins. Well,\n"
	.string "that and the screaming.$"

PewterCity_Gym_Text_ItsFreeLetsGetHappening::
	.string "Advice is free! Let's get\n"
	.string "happening! Yell with me!$"

PewterCity_Gym_Text_YoureChampMaterial::
	.string "Just as I thought! You're KINGPIN\n"
	.string "material!$"

PewterCity_Gym_Text_GymStatue::
	.string "PEWTER POKéMON GYM\n"
	.string "LEADER: PIERS\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"

PewterCity_Gym_Text_GymStatuePlayerWon::
	.string "PEWTER POKéMON GYM\n"
	.string "LEADER: PIERS\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"

