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
	.string "Hi!\n"
	.string "I work at a POKéMON MART.\p"
	.string "It's part of a convenient chain\n"
	.string "selling all sorts of items.\p"
	.string "Please, visit us in VIRIDIAN CITY.\p"
	.string "I know, I'll give you a sample.\n"
	.string "Here you go!$"

Route1_Text_ComeSeeUsIfYouNeedPokeBalls::
	.string "Please come see us if you need\n"
	.string "POKé BALLS for catching POKéMON.$"

Route1_Text_PutPotionAway::
	.string "{PLAYER} put the POTION away in\n"
	.string "the BAG's ITEMS POCKET.$"

Route1_Text_CanJumpFromLedges::
	.string "See those ledges along the road?\p"
	.string "It's a bit scary, but you can jump\n"
	.string "from them.\p"
	.string "You can get back to PALLET TOWN\n"
	.string "quicker that way.$"

Route1_Text_RouteSign::
	.string "ROUTE 1\n"
	.string "PALLET TOWN - VIRIDIAN CITY$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route1_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_1, Route1_Text_GangTrainer1Intro, Route1_Text_GangTrainer1Defeat
	msgbox Route1_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer1Intro:
	.string "Another gang punk? I'll show\nyou who runs this road!$"

Route1_Text_GangTrainer1Defeat:
	.string "All right, all right!\nYou made your point!$"

Route1_Text_GangTrainer1Post:
	.string "Word travels fast. Everyone\nwill know your name soon.$"

Route1_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_2, Route1_Text_GangTrainer2Intro, Route1_Text_GangTrainer2Defeat
	msgbox Route1_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer2Intro:
	.string "Wrong road, kid.\nThis one's ours.$"

Route1_Text_GangTrainer2Defeat:
	.string "Tch… Fine, you win.$"

Route1_Text_GangTrainer2Post:
	.string "Next time I'll bring the\nwhole gang!$"

Route1_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_3, Route1_Text_GangTrainer3Intro, Route1_Text_GangTrainer3Defeat
	msgbox Route1_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer3Intro:
	.string "You look like easy money.\nHand it over!$"

Route1_Text_GangTrainer3Defeat:
	.string "Tch… Fine, you win.$"

Route1_Text_GangTrainer3Post:
	.string "Next time I'll bring the\nwhole gang!$"

Route1_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_4, Route1_Text_GangTrainer4Intro, Route1_Text_GangTrainer4Defeat
	msgbox Route1_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer4Intro:
	.string "Nobody walks through here\nwithout a fight!$"

Route1_Text_GangTrainer4Defeat:
	.string "Heh… Not bad, punk.$"

Route1_Text_GangTrainer4Post:
	.string "Take what you want and\nget out of my sight.$"

Route1_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE1_5, Route1_Text_GangTrainer5Intro, Route1_Text_GangTrainer5Defeat
	msgbox Route1_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route1_Text_GangTrainer5Intro:
	.string "Another gang punk? I'll show\nyou who runs this road!$"

Route1_Text_GangTrainer5Defeat:
	.string "All right, all right!\nYou made your point!$"

Route1_Text_GangTrainer5Post:
	.string "GIOVANNI's crew, huh?\nI should've known.$"

