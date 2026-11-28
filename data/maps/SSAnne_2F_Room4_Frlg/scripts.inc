SSAnne_2F_Room4_Frlg_MapScripts::
	.byte 0

SSAnne_2F_Room4_EventScript_Lamar::
	trainerbattle_single TRAINER_GENTLEMAN_LAMAR, SSAnne_2F_Room4_Text_LamarIntro, SSAnne_2F_Room4_Text_LamarDefeat
	msgbox SSAnne_2F_Room4_Text_LamarPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_2F_Room4_EventScript_Dawn::
	trainerbattle_single TRAINER_LASS_DAWN, SSAnne_2F_Room4_Text_DawnIntro, SSAnne_2F_Room4_Text_DawnDefeat
	msgbox SSAnne_2F_Room4_Text_DawnPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_2F_Room4_Text_LamarIntro::
	.string "Darling, this road is far too dirty\n"
	.string "for you.$"

SSAnne_2F_Room4_Text_LamarDefeat::
	.string "Oh my. You're rather strong.$"

SSAnne_2F_Room4_Text_LamarPostBattle::
	.string "The parties in VERMILION are wild\n"
	.string "these days.$"

SSAnne_2F_Room4_Text_DawnIntro::
	.string "ARCHER says kids can't fight. I'll\n"
	.string "prove him wrong!$"

SSAnne_2F_Room4_Text_DawnDefeat::
	.string "No fair! You're way too strong!$"

SSAnne_2F_Room4_Text_DawnPostBattle::
	.string "Everybody's scared of ARCHER.\p"
	.string "Are you scared?$"

