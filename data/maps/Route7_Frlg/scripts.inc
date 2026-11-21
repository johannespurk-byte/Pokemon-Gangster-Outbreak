Route7_Frlg_MapScripts::
	.byte 0

Route7_EventScript_UndergroundPathSign::
	msgbox Route7_Text_UndergroundPathSign, MSGBOX_SIGN
	end

Route7_Text_UndergroundPathSign::
	.string "UNDERGROUND PATH\n"
	.string "CELADON CITY - LAVENDER TOWN$"


@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route7_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_1, Route7_Text_GangTrainer1Intro, Route7_Text_GangTrainer1Defeat
	msgbox Route7_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer1Intro:
	.string "Nobody walks through here\nwithout a fight!$"

Route7_Text_GangTrainer1Defeat:
	.string "Ugh! Who taught you to\nfight like that?$"

Route7_Text_GangTrainer1Post:
	.string "Keep walking. This road's\nyours… for now.$"

Route7_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_2, Route7_Text_GangTrainer2Intro, Route7_Text_GangTrainer2Defeat
	msgbox Route7_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer2Intro:
	.string "The city burned and I got\nstronger. Wanna see?$"

Route7_Text_GangTrainer2Defeat:
	.string "All right, all right!\nYou made your point!$"

Route7_Text_GangTrainer2Post:
	.string "Take what you want and\nget out of my sight.$"

Route7_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_3, Route7_Text_GangTrainer3Intro, Route7_Text_GangTrainer3Defeat
	msgbox Route7_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer3Intro:
	.string "You look like easy money.\nHand it over!$"

Route7_Text_GangTrainer3Defeat:
	.string "No way… I got wrecked!$"

Route7_Text_GangTrainer3Post:
	.string "Word travels fast. Everyone\nwill know your name soon.$"

Route7_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_4, Route7_Text_GangTrainer4Intro, Route7_Text_GangTrainer4Defeat
	msgbox Route7_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer4Intro:
	.string "Nobody walks through here\nwithout a fight!$"

Route7_Text_GangTrainer4Defeat:
	.string "My gang's gonna hear\nabout this…$"

Route7_Text_GangTrainer4Post:
	.string "Next time I'll bring the\nwhole gang!$"

Route7_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_5, Route7_Text_GangTrainer5Intro, Route7_Text_GangTrainer5Defeat
	msgbox Route7_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer5Intro:
	.string "Nobody walks through here\nwithout a fight!$"

Route7_Text_GangTrainer5Defeat:
	.string "Ugh! Who taught you to\nfight like that?$"

Route7_Text_GangTrainer5Post:
	.string "Next time I'll bring the\nwhole gang!$"

