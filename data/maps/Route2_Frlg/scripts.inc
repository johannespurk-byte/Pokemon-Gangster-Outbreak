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
	.string "You look like easy money.\nHand it over!$"

Route2_Text_GangTrainer1Defeat:
	.string "Tch… Fine, you win.$"

Route2_Text_GangTrainer1Post:
	.string "Keep walking. This road's\nyours… for now.$"

Route2_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_2, Route2_Text_GangTrainer2Intro, Route2_Text_GangTrainer2Defeat
	msgbox Route2_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer2Intro:
	.string "Another gang punk? I'll show\nyou who runs this road!$"

Route2_Text_GangTrainer2Defeat:
	.string "Heh… Not bad, punk.$"

Route2_Text_GangTrainer2Post:
	.string "Word travels fast. Everyone\nwill know your name soon.$"

Route2_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_3, Route2_Text_GangTrainer3Intro, Route2_Text_GangTrainer3Defeat
	msgbox Route2_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer3Intro:
	.string "Heh, fresh meat for my\nPOKéMON!$"

Route2_Text_GangTrainer3Defeat:
	.string "Tch… Fine, you win.$"

Route2_Text_GangTrainer3Post:
	.string "Next time I'll bring the\nwhole gang!$"

Route2_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_4, Route2_Text_GangTrainer4Intro, Route2_Text_GangTrainer4Defeat
	msgbox Route2_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer4Intro:
	.string "Another gang punk? I'll show\nyou who runs this road!$"

Route2_Text_GangTrainer4Defeat:
	.string "My gang's gonna hear\nabout this…$"

Route2_Text_GangTrainer4Post:
	.string "Keep walking. This road's\nyours… for now.$"

Route2_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE2_5, Route2_Text_GangTrainer5Intro, Route2_Text_GangTrainer5Defeat
	msgbox Route2_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route2_Text_GangTrainer5Intro:
	.string "Heh, fresh meat for my\nPOKéMON!$"

Route2_Text_GangTrainer5Defeat:
	.string "All right, all right!\nYou made your point!$"

Route2_Text_GangTrainer5Post:
	.string "Next time I'll bring the\nwhole gang!$"

