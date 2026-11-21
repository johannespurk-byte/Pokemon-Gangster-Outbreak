Route4_Frlg_MapScripts::
	.byte 0

Route4_EventScript_Unused::
	end

Route4_EventScript_Woman::
	msgbox Route4_Text_TrippedOverGeodude, MSGBOX_NPC
	end

Route4_EventScript_MtMoonSign::
	msgbox Route4_Text_MtMoonEntrance, MSGBOX_SIGN
	end

Route4_EventScript_RouteSign::
	msgbox Route4_Text_RouteSign, MSGBOX_SIGN
	end

Route4_Text_TrippedOverGeodude::
	.string "Ouch! I tripped over a rocky\n"
	.string "POKéMON, GEODUDE!$"

Route4_Text_CrissyIntro::
	.string "I came to MT. MOON in search of\n"
	.string "mushroom POKéMON.$"

Route4_Text_CrissyDefeat::
	.string "After all I did to catch them!$"

Route4_Text_CrissyPostBattle::
	.string "There might not be any more\n"
	.string "mushrooms here.\p"
	.string "I think I got them all.$"

Route4_Text_MtMoonEntrance::
	.string "MT. MOON\n"
	.string "Tunnel Entrance$"

Route4_Text_RouteSign::
	.string "ROUTE 4\n"
	.string "MT. MOON - CERULEAN CITY$"

Text_MegaPunchTeach::
	.string "A punch of roaring ferocity!\p"
	.string "Packed with destructive power!\p"
	.string "When the chips are down,\n"
	.string "MEGA PUNCH is the ultimate attack!\l"
	.string "You agree, yes?\p"
	.string "Now!\n"
	.string "Let me teach it to your POKéMON!$"

Text_MegaPunchDeclined::
	.string "You'll be back when you understand\n"
	.string "the worth of MEGA PUNCH.$"

Text_MegaPunchWhichMon::
	.string "Fine!\n"
	.string "Which POKéMON will learn it?$"

Text_MegaPunchTaught::
	.string "Now, we are comrades in the way of\n"
	.string "punching!\p"
	.string "You should go before you're seen\n"
	.string "by the misguided fool who trains\l"
	.string "only his silly kicking over there.$"

Text_MegaKickTeach::
	.string "A kick of brutal ferocity!\p"
	.string "Packed with destructive power!\p"
	.string "When you get right down to it,\n"
	.string "MEGA KICK is the ultimate attack!\l"
	.string "Don't you agree?\p"
	.string "Okay!\n"
	.string "I'll teach it to your POKéMON!$"

Text_MegaKickDeclined::
	.string "You'll come crawling back when you\n"
	.string "realize the value of MEGA KICK.$"

Text_MegaKickWhichMon::
	.string "All right!\n"
	.string "Which POKéMON wants to learn it?$"

Text_MegaKickTaught::
	.string "Now, we are soul mates in the way\n"
	.string "of kicking!\p"
	.string "You should run before you're seen\n"
	.string "by the deluded nitwit who trains\l"
	.string "only simple punching over there.$"

Route4_Text_PeopleLikeAndRespectBrock::
	.string "Oh, wow, that's the PROSPECT RANK!\n"
	.string "You got it from BROCK, didn't you?\p"
	.string "BROCK is cool. He's not just tough.\n"
	.string "People like and respect him.\p"
	.string "I want to become a GYM LEADER\n"
	.string "like him.$"

@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route4_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_1, Route4_Text_GangTrainer1Intro, Route4_Text_GangTrainer1Defeat
	msgbox Route4_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer1Intro:
	.string "This is our turf now.\nPay the toll or get lost!$"

Route4_Text_GangTrainer1Defeat:
	.string "All right, all right!\nYou made your point!$"

Route4_Text_GangTrainer1Post:
	.string "Keep walking. This road's\nyours… for now.$"

Route4_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_2, Route4_Text_GangTrainer2Intro, Route4_Text_GangTrainer2Defeat
	msgbox Route4_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer2Intro:
	.string "Nobody walks through here\nwithout a fight!$"

Route4_Text_GangTrainer2Defeat:
	.string "My gang's gonna hear\nabout this…$"

Route4_Text_GangTrainer2Post:
	.string "Keep walking. This road's\nyours… for now.$"

Route4_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_3, Route4_Text_GangTrainer3Intro, Route4_Text_GangTrainer3Defeat
	msgbox Route4_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer3Intro:
	.string "Heh, fresh meat for my\nPOKéMON!$"

Route4_Text_GangTrainer3Defeat:
	.string "My gang's gonna hear\nabout this…$"

Route4_Text_GangTrainer3Post:
	.string "GIOVANNI's crew, huh?\nI should've known.$"

Route4_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_4, Route4_Text_GangTrainer4Intro, Route4_Text_GangTrainer4Defeat
	msgbox Route4_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer4Intro:
	.string "Wrong road, kid.\nThis one's ours.$"

Route4_Text_GangTrainer4Defeat:
	.string "Heh… Not bad, punk.$"

Route4_Text_GangTrainer4Post:
	.string "Ever since the outbreak,\nonly the strong survive.$"

