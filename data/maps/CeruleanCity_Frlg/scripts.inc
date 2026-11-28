CeruleanCity_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_OnTransition
	.byte 0

CeruleanCity_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CERULEAN_CITY
	call_if_unset FLAG_GOT_SS_TICKET, CeruleanCity_EventScript_BlockExits
	end

CeruleanCity_EventScript_BlockExits::
	setobjectxyperm LOCALID_CERULEAN_POLICEMAN, 30, 12
	setobjectxyperm LOCALID_CERULEAN_SLOWBRO, 26, 31
	setobjectxyperm LOCALID_CERULEAN_LASS, 27, 31
	return

CeruleanCity_EventScript_RivalTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto CeruleanCity_EventScript_Rival
	end

CeruleanCity_EventScript_RivalTriggerMid::
	lockall
	setvar VAR_TEMP_1, 1
	setobjectxyperm LOCALID_CERULEAN_RIVAL, 23, 0
	goto CeruleanCity_EventScript_Rival
	end

CeruleanCity_EventScript_RivalTriggerRight::
	lockall
	setvar VAR_TEMP_1, 2
	setobjectxyperm LOCALID_CERULEAN_RIVAL, 24, 0
	goto CeruleanCity_EventScript_Rival
	end

CeruleanCity_EventScript_Rival::
	textcolor NPC_TEXT_COLOR_MALE
	setvar VAR_MAP_SCENE_ROUTE22, 2
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	addobject LOCALID_CERULEAN_RIVAL
	applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_RivalEnter
	waitmovement 0
	msgbox CeruleanCity_Text_RivalIntro
	setvar VAR_LAST_TALKED, LOCALID_CERULEAN_RIVAL
	call_if_eq VAR_STARTER_MON, 2, CeruleanCity_EventScript_RivalSquirtle
	call_if_eq VAR_STARTER_MON, 1, CeruleanCity_EventScript_RivalBulbasaur
	call_if_eq VAR_STARTER_MON, 0, CeruleanCity_EventScript_RivalCharmander
	call EventScript_GangRivalGivesHM01
	famechecker FAMECHECKER_BILL, 0
	msgbox CeruleanCity_Text_RivalPostBattle
	closemessage
	playbgm MUS_RG_RIVAL_EXIT, 0
	call_if_eq VAR_TEMP_1, 0, CeruleanCity_EventScript_RivalStartExit
	call_if_eq VAR_TEMP_1, 1, CeruleanCity_EventScript_RivalStartExit
	call_if_eq VAR_TEMP_1, 2, CeruleanCity_EventScript_RivalStartExitRight
	msgbox CeruleanCity_Text_OhRightLittlePresentAsFavor
	setvar VAR_MAP_SCENE_CERULEAN_CITY_RIVAL, 1
	setflag FLAG_GOT_FAME_CHECKER
	giveitem ITEM_FAME_CHECKER
	msgbox CeruleanCity_Text_ExplainFameCheckerSmellYa
	closemessage
	applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_RivalExit
	waitmovement 0
	fadedefaultbgm
	removeobject LOCALID_CERULEAN_RIVAL
	releaseall
	end

CeruleanCity_EventScript_RivalSquirtle::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_SQUIRTLE, CeruleanCity_Text_RivalDefeat
	return

CeruleanCity_EventScript_RivalBulbasaur::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_BULBASAUR, CeruleanCity_Text_RivalDefeat
	return

CeruleanCity_EventScript_RivalCharmander::
	trainerbattle_no_intro TRAINER_RIVAL_CERULEAN_CHARMANDER, CeruleanCity_Text_RivalDefeat
	return

CeruleanCity_EventScript_RivalStartExit::
	applymovement LOCALID_PLAYER, CeruleanCity_Movement_PlayerWatchRivalExit
	applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_RivalStartExit
	waitmovement 0
	return

CeruleanCity_EventScript_RivalStartExitRight::
	applymovement LOCALID_PLAYER, CeruleanCity_Movement_PlayerWatchRivalExitRight
	applymovement LOCALID_CERULEAN_RIVAL, CeruleanCity_Movement_RivalStartExitRight
	waitmovement 0
	return

CeruleanCity_Movement_PlayerWatchRivalExit::
	delay_16
	delay_8
	walk_in_place_faster_right
	delay_16
	walk_in_place_faster_down
	step_end

CeruleanCity_Movement_PlayerWatchRivalExitRight::
	delay_16
	delay_8
	walk_in_place_faster_left
	delay_16
	walk_in_place_faster_down
	step_end

CeruleanCity_Movement_RivalEnter::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CeruleanCity_Movement_RivalStartExit::
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	delay_16
	delay_16
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

CeruleanCity_Movement_RivalStartExitRight::
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	delay_16
	delay_16
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

CeruleanCity_Movement_RivalExit::
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

CeruleanCity_EventScript_Grunt::
	lock
	faceplayer
	goto_if_defeated TRAINER_TEAM_ROCKET_GRUNT_5, CeruleanCity_EventScript_GruntDefeated
	message CeruleanCity_Text_GruntIntro
	waitmessage
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	waitbuttonpress
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_5, CeruleanCity_Text_GruntDefeat
	setvar VAR_MAP_SCENE_CERULEAN_CITY_ROCKET, 1
	goto CeruleanCity_EventScript_GruntDefeated
	end

CeruleanCity_EventScript_GruntDefeated::
	msgbox CeruleanCity_Text_OkayIllReturnStolenTM
	checkitemspace ITEM_TM28
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_EventScript_NoRoomForTM28
	giveitem_msg CeruleanCity_Text_RecoveredTM28FromGrunt, ITEM_TM28
	msgbox CeruleanCity_Text_BetterGetMovingBye
	closemessage
	fadescreenswapbuffers FADE_TO_BLACK
	removeobject LOCALID_CERULEAN_GRUNT
	fadescreenswapbuffers FADE_FROM_BLACK
	setflag FLAG_GOT_TM28_FROM_ROCKET
	release
	end

CeruleanCity_EventScript_NoRoomForTM28::
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CeruleanCity_Text_MakeRoomForThisCantRun
	release
	end

CeruleanCity_EventScript_GruntTriggerTop::
	lockall
	setvar VAR_TEMP_1, 0
	applymovement LOCALID_CERULEAN_GRUNT, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	goto CeruleanCity_EventScript_GruntTrigger
	end

CeruleanCity_EventScript_GruntTriggerBottom::
	lockall
	setvar VAR_TEMP_1, 1
	applymovement LOCALID_CERULEAN_GRUNT, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	goto CeruleanCity_EventScript_GruntTrigger
	end

CeruleanCity_EventScript_GruntTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	msgbox CeruleanCity_Text_GruntIntro
	setvar VAR_LAST_TALKED, LOCALID_CERULEAN_GRUNT
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_5, CeruleanCity_Text_GruntDefeat
	setvar VAR_MAP_SCENE_CERULEAN_CITY_ROCKET, 1
	goto CeruleanCity_EventScript_GruntDefeated
	end

CeruleanCity_EventScript_Policeman::
	lock
	faceplayer
	msgbox CeruleanCity_Text_PeopleHereWereRobbed
	closemessage
	applymovement LOCALID_CERULEAN_POLICEMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_LittleBoy::
	lock
	faceplayer
	goto_if_set FLAG_GOT_SS_TICKET, CeruleanCity_EventScript_LittleBoySlowbroMoved
	msgbox CeruleanCity_Text_IfSlowbroWasntThereCouldCutTree
	release
	end

CeruleanCity_EventScript_LittleBoySlowbroMoved::
	msgbox CeruleanCity_Text_YouCanCutDownSmallTrees
	release
	end

CeruleanCity_EventScript_BaldingMan::
	msgbox CeruleanCity_Text_PokemonEncyclopediaAmusing, MSGBOX_NPC
	end

CeruleanCity_EventScript_Youngster::
	msgbox CeruleanCity_Text_TrainerLifeIsToughIsntIt, MSGBOX_NPC
	end

CeruleanCity_EventScript_CeruleanCaveGuard::
	msgbox CeruleanCity_Text_ThisIsCeruleanCave, MSGBOX_NPC
	end

CeruleanCity_EventScript_Woman::
	lock
	faceplayer
	msgbox CeruleanCity_Text_WantBrightRedBicycle
	closemessage
	applymovement LOCALID_CERULEAN_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

CeruleanCity_EventScript_Lass::
	lock
	random 3
	copyvar VAR_0x8008, VAR_RESULT
	call_if_eq VAR_0x8008, 0, CeruleanCity_EventScript_SlowbroCommand1
	call_if_eq VAR_0x8008, 1, CeruleanCity_EventScript_SlowbroCommand2
	call_if_eq VAR_0x8008, 2, CeruleanCity_EventScript_SlowbroCommand3
	waitmessage
	delay 40
	playse SE_PIN
	applymovement LOCALID_CERULEAN_SLOWBRO, Common_Movement_QuestionMark
	waitmovement 0
	delay 30
	call_if_eq VAR_0x8008, 0, CeruleanCity_EventScript_SlowbroFailed1
	call_if_eq VAR_0x8008, 1, CeruleanCity_EventScript_SlowbroFailed2
	call_if_eq VAR_0x8008, 2, CeruleanCity_EventScript_SlowbroFailed3
	release
	end

CeruleanCity_EventScript_SlowbroCommand1::
	message CeruleanCity_Text_SlowbroUseSonicboom
	return

CeruleanCity_EventScript_SlowbroCommand2::
	message CeruleanCity_Text_SlowbroPunch
	return

CeruleanCity_EventScript_SlowbroCommand3::
	message CeruleanCity_Text_SlowbroWithdraw
	return

CeruleanCity_EventScript_SlowbroFailed1::
	msgbox CeruleanCity_Text_SlowbroPayAttention
	return

CeruleanCity_EventScript_SlowbroFailed2::
	msgbox CeruleanCity_Text_NoYouBlewItAgain
	return

CeruleanCity_EventScript_SlowbroFailed3::
	msgbox CeruleanCity_Text_HardToControlMonsObedience
	return

CeruleanCity_EventScript_Slowbro::
	lock
	random 4
	copyvar VAR_0x8008, VAR_RESULT
	call_if_eq VAR_0x8008, 0, CeruleanCity_EventScript_SlowbroText1
	call_if_eq VAR_0x8008, 1, CeruleanCity_EventScript_SlowbroText2
	call_if_eq VAR_0x8008, 2, CeruleanCity_EventScript_SlowbroText3
	call_if_eq VAR_0x8008, 3, CeruleanCity_EventScript_SlowbroText4
	release
	end

CeruleanCity_EventScript_SlowbroText1::
	msgbox CeruleanCity_Text_SlowbroTookSnooze
	return

CeruleanCity_EventScript_SlowbroText2::
	msgbox CeruleanCity_Text_SlowbroLoafingAround
	return

CeruleanCity_EventScript_SlowbroText3::
	msgbox CeruleanCity_Text_SlowbroTurnedAway
	return

CeruleanCity_EventScript_SlowbroText4::
	msgbox CeruleanCity_Text_SlowbroIgnoredOrders
	return

CeruleanCity_EventScript_CitySign::
	msgbox CeruleanCity_Text_CitySign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_TrainerTips::
	msgbox CeruleanCity_Text_TrainerTipsHeldItems, MSGBOX_SIGN
	end

CeruleanCity_EventScript_BikeShopSign::
	msgbox CeruleanCity_Text_BikeShopSign, MSGBOX_SIGN
	end

CeruleanCity_EventScript_GymSign::
	lockall
	famechecker FAMECHECKER_MISTY, 0
	msgbox CeruleanCity_Text_GymSign
	releaseall
	end

CeruleanCity_Text_RivalIntro::
	.string "{RIVAL}: Yo! {PLAYER}!\p"
	.string "Still crawling around back here?\p"
	.string "I've got strong POKéMON. Stolen\n"
	.string "from the best!\p"
	.string "Here, let me see what my old man's\n"
	.string "pet has caught!$"

CeruleanCity_Text_RivalDefeat::
	.string "Hey! Take it easy! You won\n"
	.string "already!$"

CeruleanCity_Text_RivalPostBattle::
	.string "{RIVAL}: Hey, guess what?\p"
	.string "I went to BILL's and made him\n"
	.string "show me his rare POKéMON.\p"
	.string "That filled up my POKéDEX nicely.\p"
	.string "BILL built the PC storage system.\n"
	.string "Every gang in KANTO uses it.\p"
	.string "You use it too, so go thank him. Or\n"
	.string "rob him. Your call.\p"
	.string "I'm out. Later!$"

CeruleanCity_Text_OhRightLittlePresentAsFavor::
	.string "Oh, yeah, right.\p"
	.string "You're always one step behind me.\n"
	.string "It's kind of sad.\p"
	.string "So here, a little present. Don't\n"
	.string "get used to it.$"

CeruleanCity_Text_ExplainFameCheckerSmellYa::
	.string "A gossip like you will love that\n"
	.string "thing.\p"
	.string "It tracks what people say about\n"
	.string "the gang bosses.\p"
	.string "I don't care what anyone says.\n"
	.string "This time I'm really gone!$"

CeruleanCity_Text_GruntIntro::
	.string "Hey! Stay out! It's not your yard!\p"
	.string "…Huh? Me?\p"
	.string "I'm an innocent bystander! Ask\n"
	.string "GUZMA!{PLAY_BGM}{MUS_RG_ENCOUNTER_ROCKET}$"

CeruleanCity_Text_GruntDefeat::
	.string "GRUNT: Stop! I give up! I'll leave\n"
	.string "quietly!$"

CeruleanCity_Text_OkayIllReturnStolenTM::
	.string "…Okay. Here's the TM I stole.\n"
	.string "Don't tell GUZMA!$"

CeruleanCity_Text_RecoveredTM28FromGrunt::
	.string "{PLAYER} recovered TM28 from\n"
	.string "the TEAM ROCKET GRUNT.$"

CeruleanCity_Text_BetterGetMovingBye::
	.string "I better get moving!\n"
	.string "Bye!$"

CeruleanCity_Text_MakeRoomForThisCantRun::
	.string "Make room for this! I can't run\n"
	.string "until I give it back!$"

CeruleanCity_Text_TrainerLifeIsToughIsntIt::
	.string "You're a TRAINER, too?\p"
	.string "Since TEAM SKULL came, it's a\n"
	.string "tough life, isn't it?$"

CeruleanCity_Text_YouCanCutDownSmallTrees::
	.string "Did you know that you can CUT down\n"
	.string "small trees?\p"
	.string "Even that small tree in front of\n"
	.string "the shop can be CUT down.\p"
	.string "I think there's a way around it,\n"
	.string "though.$"

CeruleanCity_Text_IfSlowbroWasntThereCouldCutTree::
	.string "If that SLOWBRO wasn't there, you\n"
	.string "could CUT down the small tree.\p"
	.string "That way, you could get to the\n"
	.string "other side.\p"
	.string "I think there's a way around it,\n"
	.string "though.$"

CeruleanCity_Text_PokemonEncyclopediaAmusing::
	.string "You're making a POKéMON\n"
	.string "encyclopedia? In the middle of a\p"
	.string "gang war? Amusing.$"

CeruleanCity_Text_PeopleHereWereRobbed::
	.string "The people here were robbed.\p"
	.string "The ROCKETS broke in, and TEAM\n"
	.string "SKULL took the rest!\p"
	.string "Even the POLICE gave up.$"

CeruleanCity_Text_SlowbroUseSonicboom::
	.string "Okay! SLOWBRO!\n"
	.string "Use SONICBOOM!$"

CeruleanCity_Text_SlowbroPayAttention::
	.string "Come on, SLOWBRO, pay attention!$"

CeruleanCity_Text_SlowbroPunch::
	.string "SLOWBRO, punch!$"

CeruleanCity_Text_NoYouBlewItAgain::
	.string "No!\n"
	.string "You blew it again!$"

CeruleanCity_Text_SlowbroWithdraw::
	.string "SLOWBRO, WITHDRAW!$"

CeruleanCity_Text_HardToControlMonsObedience::
	.string "No! That's wrong!\p"
	.string "SLOWBRO only listens to tough\n"
	.string "bosses. I guess I'm not one…$"

CeruleanCity_Text_SlowbroTookSnooze::
	.string "SLOWBRO took a snooze…$"

CeruleanCity_Text_SlowbroLoafingAround::
	.string "SLOWBRO is loafing around…$"

CeruleanCity_Text_SlowbroTurnedAway::
	.string "SLOWBRO turned away…$"

CeruleanCity_Text_SlowbroIgnoredOrders::
	.string "SLOWBRO ignored orders…$"

CeruleanCity_Text_WantBrightRedBicycle::
	.string "I want a bright red bicycle.\p"
	.string "But TEAM SKULL would just steal\n"
	.string "it.$"

CeruleanCity_Text_ThisIsCeruleanCave::
	.string "This is CERULEAN CAVE.\p"
	.string "Horribly strong POKéMON live\n"
	.string "inside. Even TEAM SKULL stays out.\p"
	.string "Only a very special TRAINER may\n"
	.string "enter.\p"
	.string "You'd have to be strong enough to\n"
	.string "beat the CHAMPION for starters.\p"
	.string "And you'd need to have done\n"
	.string "something truly great.$"

CeruleanCity_Text_CitySign::
	.string "CERULEAN CITY\p"
	.string "A Mysterious Blue Aura… and TEAM\n"
	.string "SKULL$"

CeruleanCity_Text_TrainerTipsHeldItems::
	.string "TRAINER TIPS\p"
	.string "A POKéMON can be made to hold\n"
	.string "an item.\p"
	.string "Some items can even be used by\n"
	.string "the holding POKéMON in battle.$"

CeruleanCity_Text_BikeShopSign::
	.string "Grass and caves handled easily!\n"
	.string "BIKE SHOP$"

CeruleanCity_Text_GymSign::
	.string "CERULEAN CITY POKéMON GYM\p"
	.string "BOSS: GUZMA - TEAM SKULL$"

