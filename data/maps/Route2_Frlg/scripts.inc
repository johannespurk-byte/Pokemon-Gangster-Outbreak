Route2_Frlg_MapScripts::
	.byte 0

Route2_EventScript_RouteSign::
	msgbox Route2_Text_RouteSign, MSGBOX_SIGN
	end

Route2_EventScript_DiglettsCaveSign::
	msgbox Route2_Text_DiglettsCave, MSGBOX_SIGN
	end

Route2_Text_RouteSign::
	.string "ROUTE 2\n"
	.string "VIRIDIAN CITY - PEWTER CITY$"

Route2_Text_DiglettsCave::
	.string "DIGLETT'S CAVE$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route2_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_1, Route2_Text_GangTrainer1Intro, Route2_Text_GangTrainer1Defeat
	msgbox Route2_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer1Intro:
	.string "GIOVANNI said nobody passes.\n"
	.string "That means you!$"

Route2_Text_GangTrainer1Defeat:
	.string "Fine! Keep your cash!$"

Route2_Text_GangTrainer1Post:
	.string "Tell your crew I fought fair.\n"
	.string "Mostly.$"

Route2_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_2, Route2_Text_GangTrainer2Intro, Route2_Text_GangTrainer2Defeat
	msgbox Route2_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer2Intro:
	.string "I'm the toughest kid on this road.\n"
	.string "Ask anyone!$"

Route2_Text_GangTrainer2Defeat:
	.string "Okay, okay! You win!$"

Route2_Text_GangTrainer2Post:
	.string "The road to VIRIDIAN isn't safe.\p"
	.string "Watch out!$"

Route2_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_3, Route2_Text_GangTrainer3Intro, Route2_Text_GangTrainer3Defeat
	msgbox Route2_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer3Intro:
	.string "I found a loot bag yesterday!\p"
	.string "Wanna fight for it?$"

Route2_Text_GangTrainer3Defeat:
	.string "Waaah! I'm telling my brother!$"

Route2_Text_GangTrainer3Post:
	.string "Don't tell my mom I battled you.$"

Route2_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_4, Route2_Text_GangTrainer4Intro, Route2_Text_GangTrainer4Defeat
	msgbox Route2_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer4Intro:
	.string "This road belongs to TEAM ROCKET.\p"
	.string "Pay up or fight!$"

Route2_Text_GangTrainer4Defeat:
	.string "GIOVANNI is gonna kill me…$"

Route2_Text_GangTrainer4Post:
	.string "Word travels fast.\p"
	.string "GIOVANNI will hear about you.$"

Route2_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_5, Route2_Text_GangTrainer5Intro, Route2_Text_GangTrainer5Defeat
	msgbox Route2_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer5Intro:
	.string "GIOVANNI wants your money.\p"
	.string "And I want a fight!$"

Route2_Text_GangTrainer5Defeat:
	.string "Who are you, anyway?!$"

Route2_Text_GangTrainer5Post:
	.string "Next time I'll bring the whole of\n"
	.string "TEAM ROCKET!$"

