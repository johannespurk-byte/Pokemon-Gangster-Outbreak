Route5_Frlg_MapScripts::
	.byte 0

Route5_EventScript_UndergroundPathSign::
	msgbox Route5_Text_UndergroundPathSign, MSGBOX_SIGN
	end

Route5_Text_UndergroundPathSign::
	.string "UNDERGROUND PATH\n"
	.string "CERULEAN CITY - VERMILION CITY$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route5_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_1, Route5_Text_GangTrainer1Intro, Route5_Text_GangTrainer1Defeat
	msgbox Route5_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer1Intro:
	.string "Wrong road, kid.\nThis one's ours.$"

Route5_Text_GangTrainer1Defeat:
	.string "Heh… Not bad, punk.$"

Route5_Text_GangTrainer1Post:
	.string "Ever since the outbreak,\nonly the strong survive.$"

Route5_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_2, Route5_Text_GangTrainer2Intro, Route5_Text_GangTrainer2Defeat
	msgbox Route5_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer2Intro:
	.string "This is our turf now.\nPay the toll or get lost!$"

Route5_Text_GangTrainer2Defeat:
	.string "No way… I got wrecked!$"

Route5_Text_GangTrainer2Post:
	.string "Take what you want and\nget out of my sight.$"

Route5_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_3, Route5_Text_GangTrainer3Intro, Route5_Text_GangTrainer3Defeat
	msgbox Route5_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer3Intro:
	.string "Another gang punk? I'll show\nyou who runs this road!$"

Route5_Text_GangTrainer3Defeat:
	.string "All right, all right!\nYou made your point!$"

Route5_Text_GangTrainer3Post:
	.string "Next time I'll bring the\nwhole gang!$"

Route5_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_4, Route5_Text_GangTrainer4Intro, Route5_Text_GangTrainer4Defeat
	msgbox Route5_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer4Intro:
	.string "This is our turf now.\nPay the toll or get lost!$"

Route5_Text_GangTrainer4Defeat:
	.string "Ugh! Who taught you to\nfight like that?$"

Route5_Text_GangTrainer4Post:
	.string "Take what you want and\nget out of my sight.$"

Route5_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_5, Route5_Text_GangTrainer5Intro, Route5_Text_GangTrainer5Defeat
	msgbox Route5_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer5Intro:
	.string "Rocket's gone, so the streets\nbelong to whoever's toughest!$"

Route5_Text_GangTrainer5Defeat:
	.string "No way… I got wrecked!$"

Route5_Text_GangTrainer5Post:
	.string "GIOVANNI's crew, huh?\nI should've known.$"

