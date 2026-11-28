SSAnne_B1F_Room1_Frlg_MapScripts::
	.byte 0

SSAnne_B1F_Room1_EventScript_Barny::
	trainerbattle_single TRAINER_FISHERMAN_BARNY, SSAnne_B1F_Room1_Text_BarnyIntro, SSAnne_B1F_Room1_Text_BarnyDefeat
	msgbox SSAnne_B1F_Room1_Text_BarnyPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room1_EventScript_Phillip::
	trainerbattle_single TRAINER_SAILOR_PHILLIP, SSAnne_B1F_Room1_Text_PhillipIntro, SSAnne_B1F_Room1_Text_PhillipDefeat
	msgbox SSAnne_B1F_Room1_Text_PhillipPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room1_Text_PhillipIntro::
	.string "Out here, only the strong stay\n"
	.string "afloat!$"

SSAnne_B1F_Room1_Text_PhillipDefeat::
	.string "Sunk! Totally sunk!$"

SSAnne_B1F_Room1_Text_PhillipPostBattle::
	.string "The water's the only free place\n"
	.string "left.$"

SSAnne_B1F_Room1_Text_BarnyIntro::
	.string "You'll sink like the last punk who\n"
	.string "came here!$"

SSAnne_B1F_Room1_Text_BarnyDefeat::
	.string "Glub… I'm all washed up.$"

SSAnne_B1F_Room1_Text_BarnyPostBattle::
	.string "Swim fast if you see a boat from\n"
	.string "TEAM ROCKET.$"

