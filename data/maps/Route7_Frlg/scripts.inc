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
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

Route7_Text_GangTrainer1Defeat:
	.string "Okay, okay! You win!$"

Route7_Text_GangTrainer1Post:
	.string "Can I join your crew when I'm\n"
	.string "older?$"

Route7_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_2, Route7_Text_GangTrainer2Intro, Route7_Text_GangTrainer2Defeat
	msgbox Route7_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer2Intro:
	.string "Ugh, a street kid.\p"
	.string "Let's get this over with.$"

Route7_Text_GangTrainer2Defeat:
	.string "My outfit! It's ruined!$"

Route7_Text_GangTrainer2Post:
	.string "Do tell LYSANDRE I said hello.$"

Route7_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_3, Route7_Text_GangTrainer3Intro, Route7_Text_GangTrainer3Defeat
	msgbox Route7_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer3Intro:
	.string "You walk like you own the place.\p"
	.string "Big mistake!$"

Route7_Text_GangTrainer3Defeat:
	.string "Who are you, anyway?!$"

Route7_Text_GangTrainer3Post:
	.string "Go on. This road's yours…\p"
	.string "for now.$"

Route7_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_4, Route7_Text_GangTrainer4Intro, Route7_Text_GangTrainer4Defeat
	msgbox Route7_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer4Intro:
	.string "Hey! You're in the wrong part of\n"
	.string "town!$"

Route7_Text_GangTrainer4Defeat:
	.string "Tch… You hit hard.$"

Route7_Text_GangTrainer4Post:
	.string "You're making a name for yourself,\n"
	.string "boss.$"

Route7_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE7_5, Route7_Text_GangTrainer5Intro, Route7_Text_GangTrainer5Defeat
	msgbox Route7_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route7_Text_GangTrainer5Intro:
	.string "My big brother rides with TEAM\n"
	.string "FLARE! I'm gonna be just like him!$"

Route7_Text_GangTrainer5Defeat:
	.string "Waaah! I'm telling my brother!$"

Route7_Text_GangTrainer5Post:
	.string "When I grow up, I'm joining TEAM\n"
	.string "FLARE.\p"
	.string "Or maybe your crew!$"

