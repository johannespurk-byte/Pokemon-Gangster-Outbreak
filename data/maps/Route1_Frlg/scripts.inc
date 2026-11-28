Route1_Frlg_MapScripts::
	.byte 0

Route1_EventScript_MartClerk::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POTION_ON_ROUTE_1, Route1_EventScript_AlreadyGotPotion
	msgbox Route1_Text_WorkAtPokeMartTakeSample
	textcolor NPC_TEXT_COLOR_NEUTRAL
	checkitemspace ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	bufferitemname STR_VAR_2, ITEM_POTION
	playfanfare MUS_LEVEL_UP
	message gText_ObtainedTheItem
	waitmessage
	waitfanfare
	additem ITEM_POTION
	msgbox Route1_Text_PutPotionAway
	call EventScript_RestorePrevTextColor
	setflag FLAG_GOT_POTION_ON_ROUTE_1
	release
	end

Route1_EventScript_AlreadyGotPotion::
	msgbox Route1_Text_ComeSeeUsIfYouNeedPokeBalls
	release
	end

Route1_EventScript_Boy::
	msgbox Route1_Text_CanJumpFromLedges, MSGBOX_NPC
	end

Route1_EventScript_RouteSign::
	msgbox Route1_Text_RouteSign, MSGBOX_SIGN
	end

Route1_Text_WorkAtPokeMartTakeSample::
	.string "Hi! The MARTS are all vending\n"
	.string "machines now.\p"
	.string "I hand out free samples so people\n"
	.string "keep coming. Here!$"

Route1_Text_ComeSeeUsIfYouNeedPokeBalls::
	.string "The machine in VIRIDIAN sells\n"
	.string "POKé BALLS. If TEAM ROCKET left\p"
	.string "any.$"

Route1_Text_PutPotionAway::
	.string "{PLAYER} put the POTION away in\n"
	.string "the BAG's ITEMS POCKET.$"

Route1_Text_CanJumpFromLedges::
	.string "See those ledges? Jump down them\n"
	.string "and you're back in PALLET in no\p"
	.string "time.\p"
	.string "Handy when SILVER's crew is after\n"
	.string "you.$"

Route1_Text_RouteSign::
	.string "ROUTE 1\n"
	.string "PALLET TOWN - VIRIDIAN CITY$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route1_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_1, Route1_Text_GangTrainer1Intro, Route1_Text_GangTrainer1Defeat
	msgbox Route1_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer1Intro:
	.string "GIOVANNI said nobody passes.\n"
	.string "That means you!$"

Route1_Text_GangTrainer1Defeat:
	.string "Who are you, anyway?!$"

Route1_Text_GangTrainer1Post:
	.string "Tell your crew I fought fair.\n"
	.string "Mostly.$"

Route1_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_2, Route1_Text_GangTrainer2Intro, Route1_Text_GangTrainer2Defeat
	msgbox Route1_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer2Intro:
	.string "GIOVANNI says kids can't fight.\n"
	.string "I'll prove him wrong!$"

Route1_Text_GangTrainer2Defeat:
	.string "Okay, okay! You win!$"

Route1_Text_GangTrainer2Post:
	.string "Everybody's scared of GIOVANNI.\p"
	.string "Are you scared?$"

Route1_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_3, Route1_Text_GangTrainer3Intro, Route1_Text_GangTrainer3Defeat
	msgbox Route1_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer3Intro:
	.string "Even in a gang war, one must look\n"
	.string "fabulous.$"

Route1_Text_GangTrainer3Defeat:
	.string "Oh my. You're rather strong.$"

Route1_Text_GangTrainer3Post:
	.string "Money buys safety from TEAM\n"
	.string "ROCKET.\p"
	.string "Usually.$"

Route1_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_4, Route1_Text_GangTrainer4Intro, Route1_Text_GangTrainer4Defeat
	msgbox Route1_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer4Intro:
	.string "I live out here, far from TEAM\n"
	.string "ROCKET.\p"
	.string "Leave me be! …Or battle!$"

Route1_Text_GangTrainer4Defeat:
	.string "Like a rockslide!$"

Route1_Text_GangTrainer4Post:
	.string "From up here I watch TEAM ROCKET\n"
	.string "come and go.$"

Route1_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_5, Route1_Text_GangTrainer5Intro, Route1_Text_GangTrainer5Defeat
	msgbox Route1_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer5Intro:
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

Route1_Text_GangTrainer5Defeat:
	.string "Waaah! I'm telling my brother!$"

Route1_Text_GangTrainer5Post:
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

