SSAnne_1F_Room5_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room5_EventScript_Arthur::
	trainerbattle_single TRAINER_GENTLEMAN_ARTHUR, SSAnne_1F_Room5_Text_ArthurIntro, SSAnne_1F_Room5_Text_ArthurDefeat
	msgbox SSAnne_1F_Room5_Text_ArthurPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room5_Text_ArthurIntro::
	.string "Ugh, a street kid.\p"
	.string "Let's get this over with.$"

SSAnne_1F_Room5_Text_ArthurDefeat::
	.string "My outfit! It's ruined!$"

SSAnne_1F_Room5_Text_ArthurPostBattle::
	.string "Do tell ARCHER I said hello.$"

