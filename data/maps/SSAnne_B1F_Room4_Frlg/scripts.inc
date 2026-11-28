SSAnne_B1F_Room4_Frlg_MapScripts::
	.byte 0

SSAnne_B1F_Room4_EventScript_Leonard::
	trainerbattle_single TRAINER_SAILOR_LEONARD, SSAnne_B1F_Room4_Text_LeonardIntro, SSAnne_B1F_Room4_Text_LeonardDefeat
	msgbox SSAnne_B1F_Room4_Text_LeonardPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room4_EventScript_Duncan::
	trainerbattle_single TRAINER_SAILOR_DUNCAN, SSAnne_B1F_Room4_Text_DuncanIntro, SSAnne_B1F_Room4_Text_DuncanDefeat
	msgbox SSAnne_B1F_Room4_Text_DuncanPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room4_Text_LeonardIntro::
	.string "You'll sink like the last punk who\n"
	.string "came here!$"

SSAnne_B1F_Room4_Text_LeonardDefeat::
	.string "Glub… I'm all washed up.$"

SSAnne_B1F_Room4_Text_LeonardPostBattle::
	.string "Swim fast if you see a boat from\n"
	.string "TEAM ROCKET.$"

SSAnne_B1F_Room4_Text_DuncanIntro::
	.string "Out here, only the strong stay\n"
	.string "afloat!$"

SSAnne_B1F_Room4_Text_DuncanDefeat::
	.string "Sunk! Totally sunk!$"

SSAnne_B1F_Room4_Text_DuncanPostBattle::
	.string "The water's the only free place\n"
	.string "left.$"

