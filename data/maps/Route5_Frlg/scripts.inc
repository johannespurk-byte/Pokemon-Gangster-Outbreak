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
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

Route5_Text_GangTrainer1Defeat:
	.string "No fair! You're way too strong!$"

Route5_Text_GangTrainer1Post:
	.string "Can I join your crew when I'm\n"
	.string "older?$"

Route5_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_2, Route5_Text_GangTrainer2Intro, Route5_Text_GangTrainer2Defeat
	msgbox Route5_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer2Intro:
	.string "I only battle people with style.\n"
	.string "You'll do.$"

Route5_Text_GangTrainer2Defeat:
	.string "I broke a nail! Unforgivable!$"

Route5_Text_GangTrainer2Post:
	.string "One day this mess will end. I'll be\n"
	.string "fabulous then, too.$"

Route5_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_3, Route5_Text_GangTrainer3Intro, Route5_Text_GangTrainer3Defeat
	msgbox Route5_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer3Intro:
	.string "This road belongs to TEAM ROCKET.\p"
	.string "Pay up or fight!$"

Route5_Text_GangTrainer3Defeat:
	.string "Tch… You hit hard.$"

Route5_Text_GangTrainer3Post:
	.string "Word travels fast.\p"
	.string "ARCHER will hear about you.$"

Route5_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_4, Route5_Text_GangTrainer4Intro, Route5_Text_GangTrainer4Defeat
	msgbox Route5_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer4Intro:
	.string "My birds see every gang that\n"
	.string "moves.\p"
	.string "And they saw you!$"

Route5_Text_GangTrainer4Defeat:
	.string "My birds flew off…$"

Route5_Text_GangTrainer4Post:
	.string "The wild POKéMON are the only\n"
	.string "honest ones left.$"

Route5_EventScript_GangTrainer5::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE5_5, Route5_Text_GangTrainer5Intro, Route5_Text_GangTrainer5Defeat
	msgbox Route5_Text_GangTrainer5Post, MSGBOX_AUTOCLOSE
	end

Route5_Text_GangTrainer5Intro:
	.string "My big brother rides with TEAM\n"
	.string "ROCKET! I'm gonna be just like\p"
	.string "him!$"

Route5_Text_GangTrainer5Defeat:
	.string "Aww… There goes my lunch money.$"

Route5_Text_GangTrainer5Post:
	.string "When I grow up, I'm joining TEAM\n"
	.string "ROCKET.\p"
	.string "Or maybe your crew!$"

