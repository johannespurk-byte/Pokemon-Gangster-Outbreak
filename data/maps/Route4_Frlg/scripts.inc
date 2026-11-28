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
	.string "Ouch! I tripped over a GEODUDE!\p"
	.string "TEAM YELL put it there as a trap, I\n"
	.string "bet!$"

Route4_Text_CrissyIntro::
	.string "PIERS says kids can't fight. I'll\n"
	.string "prove him wrong!$"

Route4_Text_CrissyDefeat::
	.string "No fair! You're way too strong!$"

Route4_Text_CrissyPostBattle::
	.string "Everybody's scared of PIERS.\p"
	.string "Are you scared?$"

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
	.string "Whoa, a real gang BADGE! You took\n"
	.string "it from PIERS?\p"
	.string "Even TEAM YELL respects that.\n"
	.string "Keep it safe!$"

@ Gangster Outbreak: gang trainers (tools/route_trainers.py)
Route4_EventScript_GangTrainer1::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_1, Route4_Text_GangTrainer1Intro, Route4_Text_GangTrainer1Defeat
	msgbox Route4_Text_GangTrainer1Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer1Intro:
	.string "PIERS invited me to his parties.\p"
	.string "Jealous?$"

Route4_Text_GangTrainer1Defeat:
	.string "I broke a nail! Unforgivable!$"

Route4_Text_GangTrainer1Post:
	.string "You fight like a true boss.\p"
	.string "How charming.$"

Route4_EventScript_GangTrainer2::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_2, Route4_Text_GangTrainer2Intro, Route4_Text_GangTrainer2Defeat
	msgbox Route4_Text_GangTrainer2Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer2Intro:
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

Route4_Text_GangTrainer2Defeat:
	.string "Aww… There goes my lunch money.$"

Route4_Text_GangTrainer2Post:
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

Route4_EventScript_GangTrainer3::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_3, Route4_Text_GangTrainer3Intro, Route4_Text_GangTrainer3Defeat
	msgbox Route4_Text_GangTrainer3Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer3Intro:
	.string "PIERS wants your money.\p"
	.string "And I want a fight!$"

Route4_Text_GangTrainer3Defeat:
	.string "Fine! Keep your cash!$"

Route4_Text_GangTrainer3Post:
	.string "Next time I'll bring the whole of\n"
	.string "TEAM YELL!$"

Route4_EventScript_GangTrainer4::
	trainerbattle_single TRAINER_GANGTRAINER_ROUTE4_4, Route4_Text_GangTrainer4Intro, Route4_Text_GangTrainer4Defeat
	msgbox Route4_Text_GangTrainer4Post, MSGBOX_AUTOCLOSE
	end

Route4_Text_GangTrainer4Intro:
	.string "Wrong turf, rookie. Nobody passes\n"
	.string "without a fight!$"

Route4_Text_GangTrainer4Defeat:
	.string "PIERS is gonna kill me…$"

Route4_Text_GangTrainer4Post:
	.string "Respect. Not many beat me on my\n"
	.string "own road.$"

