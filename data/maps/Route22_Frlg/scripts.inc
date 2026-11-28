Route22_Frlg_MapScripts::
	.byte 0

Route22_EventScript_EarlyRivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_EarlyRival
	end

Route22_EventScript_EarlyRival::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	addobject LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_EarlyRivalApproach
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_EarlyRivalApproach
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_EarlyRivalApproachBottom
	delay 6
	msgbox Route22_Text_EarlyRivalIntro
	call_if_eq VAR_STARTER_MON, 2, Route22_EventScript_EarlyRivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, Route22_EventScript_EarlyRivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, Route22_EventScript_EarlyRivalCharmander
	msgbox Route22_Text_EarlyRivalPostBattle
	closemessage
	delay 10
	playbgm MUS_RG_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_EarlyRivalExit
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_EarlyRivalExit
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_EarlyRivalExitBottom
	fadedefaultbgm
	removeobject LOCALID_ROUTE22_RIVAL
	setvar VAR_MAP_SCENE_ROUTE22, 2
	releaseall
	end

Route22_EventScript_EarlyRivalApproach::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproach
	waitmovement 0
	return

Route22_EventScript_EarlyRivalApproachBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproachBottom
	applymovement LOCALID_PLAYER, Route22_Movement_PlayerFaceRival
	waitmovement 0
	return

Route22_EventScript_EarlyRivalSquirtle::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_SQUIRTLE, 0, Route22_Text_EarlyRivalDefeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalBulbasaur::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_BULBASAUR, 0, Route22_Text_EarlyRivalDefeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalCharmander::
	trainerbattle_earlyrival TRAINER_RIVAL_ROUTE22_EARLY_CHARMANDER, 0, Route22_Text_EarlyRivalDefeat, Text_RivalVictory
	return

Route22_EventScript_EarlyRivalExit::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_EarlyRivalExit
	waitmovement 0
	return

Route22_EventScript_EarlyRivalExitBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_EarlyRivalExitBottom
	waitmovement 0
	return

Route22_Movement_UnusedRivalExit:: @ 8
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route22_Movement_EarlyRivalExit::
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route22_Movement_EarlyRivalExitBottom::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route22_Movement_RivalApproach::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route22_Movement_RivalApproachBottom::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_down
	step_end

Route22_Movement_PlayerFaceRival::
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_16
	delay_8
	walk_in_place_faster_up
	step_end

Route22_EventScript_LateRivalTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRivalTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_ROUTE22_RIVAL, 25, 5
	goto Route22_EventScript_LateRival
	end

Route22_EventScript_LateRival::
	textcolor NPC_TEXT_COLOR_MALE
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	addobject LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_TEMP_1, 0, Route22_EventScript_LateRivalApproach
	call_if_eq VAR_TEMP_1, 1, Route22_EventScript_LateRivalApproach
	call_if_eq VAR_TEMP_1, 2, Route22_EventScript_LateRivalApproachBottom
	msgbox Route22_Text_LateRivalIntro
	setvar VAR_LAST_TALKED, LOCALID_ROUTE22_RIVAL
	call_if_eq VAR_STARTER_MON, 2, Route22_EventScript_LateRivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, Route22_EventScript_LateRivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, Route22_EventScript_LateRivalCharmander
	call EventScript_GangRivalGivesHM06
	call EventScript_GangRivalGivesHM07
	msgbox Route22_Text_LateRivalPostBattle
	closemessage
	delay 10
	playbgm MUS_RG_RIVAL_EXIT, 0
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_LateRivalExit
	waitmovement 0
	fadedefaultbgm
	removeobject LOCALID_ROUTE22_RIVAL
	setvar VAR_MAP_SCENE_ROUTE22, 4
	releaseall
	end

Route22_EventScript_LateRivalApproach::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproach
	waitmovement 0
	return

Route22_EventScript_LateRivalApproachBottom::
	applymovement LOCALID_ROUTE22_RIVAL, Route22_Movement_RivalApproachBottom
	applymovement LOCALID_PLAYER, Route22_Movement_PlayerFaceRival
	waitmovement 0
	return

Route22_EventScript_LateRivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_SQUIRTLE, Route22_Text_LateRivalDefeat
	return

Route22_EventScript_LateRivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_BULBASAUR, Route22_Text_LateRivalDefeat
	return

Route22_EventScript_LateRivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_ROUTE22_LATE_CHARMANDER, Route22_Text_LateRivalDefeat
	return

Route22_Movement_LateRivalExit::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route22_EventScript_LeagueGateSign::
	msgbox Route22_Text_LeagueGateSign, MSGBOX_SIGN
	end

Route22_Text_EarlyRivalIntro::
	.string "{RIVAL}: Hey! {PLAYER}!\p"
	.string "You're heading for the LEAGUE?\n"
	.string "Don't make me laugh!\p"
	.string "No BADGES, no entry. The guards\n"
	.string "won't even look at you.\p"
	.string "My old man thinks you're special.\n"
	.string "Let's see about that!$"

Route22_Text_EarlyRivalDefeat::
	.string "Awww!\n"
	.string "You just lucked out!$"

Route22_Text_EarlyRivalPostBattle::
	.string "The LEAGUE is crawling with tough\n"
	.string "TRAINERS.\p"
	.string "And I'm going to beat every single\n"
	.string "one.\p"
	.string "Quit dawdling, {PLAYER}. You're\n"
	.string "wasting my time.$"

@ Translated in RB as "{RIVAL}: What? Why do I have 2 POKéMON? You should catch some more too!"
Route22_Text_RivalShouldCatchSomeMons::
	.string "{RIVAL}“なんだ？\n"
	.string "ポケモン　2ひきも\l"
	.string "もってるの　なぜか　だって？\p"
	.string "おまえも\n"
	.string "つかまえれば　いい　じゃん！$"

Route22_Text_LateRivalIntro::
	.string "{RIVAL}: Well, well. {PLAYER}.\p"
	.string "All eight gang bosses down? Not\n"
	.string "bad for my old man's pet.\p"
	.string "You're my warm-up before the\n"
	.string "LEAGUE!\p"
	.string "Come on!$"

Route22_Text_LateRivalDefeat::
	.string "What!?\p"
	.string "I was just careless!$"

Route22_Text_LateRivalPostBattle::
	.string "That loosened me up. I'm ready for\n"
	.string "the LEAGUE.\p"
	.string "You still need practice, {PLAYER}.\p"
	.string "See you at the top. Don't keep me\n"
	.string "waiting!$"

@ Translated in RB as "{RIVAL}: Hahaha! {PLAYER}! That's your best? You're nowhere near as good as me, pal! Go train some more! You loser!"
Route22_Text_LateRivalVictory::
	.string "{RIVAL}“ひゃははッ　{PLAYER}ー！\n"
	.string "それで　がんばってるのかよ！\l"
	.string "おれの　さいのうに　くらべりゃ\l"
	.string "{PLAYER}は　まだまだ　だな！\p"
	.string "もっと　れんしゅう　こいよ！\n"
	.string "あははーッ！$"

Route22_Text_LeagueGateSign::
	.string "POKéMON LEAGUE\p"
	.string "Front Gate - BOSSES ONLY$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route22_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE22_1, Route22_Text_GangTrainer1Intro, Route22_Text_GangTrainer1Defeat
	msgbox Route22_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route22_Text_GangTrainer1Intro:
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

Route22_Text_GangTrainer1Defeat:
	.string "Okay, okay! You win!$"

Route22_Text_GangTrainer1Post:
	.string "Can I join your crew when I'm\n"
	.string "older?$"

Route22_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE22_2, Route22_Text_GangTrainer2Intro, Route22_Text_GangTrainer2Defeat
	msgbox Route22_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route22_Text_GangTrainer2Intro:
	.string "Hey! You're in the wrong part of\n"
	.string "town!$"

Route22_Text_GangTrainer2Defeat:
	.string "Tch… You hit hard.$"

Route22_Text_GangTrainer2Post:
	.string "You're making a name for yourself,\n"
	.string "boss.$"

Route22_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE22_3, Route22_Text_GangTrainer3Intro, Route22_Text_GangTrainer3Defeat
	msgbox Route22_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route22_Text_GangTrainer3Intro:
	.string "GIOVANNI said nobody passes.\n"
	.string "That means you!$"

Route22_Text_GangTrainer3Defeat:
	.string "Fine! Keep your cash!$"

Route22_Text_GangTrainer3Post:
	.string "Tell your crew I fought fair.\n"
	.string "Mostly.$"

Route22_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE22_4, Route22_Text_GangTrainer4Intro, Route22_Text_GangTrainer4Defeat
	msgbox Route22_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route22_Text_GangTrainer4Intro:
	.string "My big brother rides with TEAM\n"
	.string "ROCKET! I'm gonna be just like\p"
	.string "him!$"

Route22_Text_GangTrainer4Defeat:
	.string "Waaah! I'm telling my brother!$"

Route22_Text_GangTrainer4Post:
	.string "When I grow up, I'm joining TEAM\n"
	.string "ROCKET.\p"
	.string "Or maybe your crew!$"

Route22_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE22_5, Route22_Text_GangTrainer5Intro, Route22_Text_GangTrainer5Defeat
	msgbox Route22_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route22_Text_GangTrainer5Intro:
	.string "This road belongs to TEAM ROCKET.\p"
	.string "Pay up or fight!$"

Route22_Text_GangTrainer5Defeat:
	.string "GIOVANNI is gonna kill me…$"

Route22_Text_GangTrainer5Post:
	.string "Word travels fast.\p"
	.string "GIOVANNI will hear about you.$"

