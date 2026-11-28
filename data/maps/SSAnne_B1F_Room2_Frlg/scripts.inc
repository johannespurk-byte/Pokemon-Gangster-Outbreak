SSAnne_B1F_Room2_Frlg_MapScripts::
	.byte 0

SSAnne_B1F_Room2_EventScript_Huey::
	trainerbattle_single TRAINER_SAILOR_HUEY, SSAnne_B1F_Room2_Text_HueyIntro, SSAnne_B1F_Room2_Text_HueyDefeat
	msgbox SSAnne_B1F_Room2_Text_HueyPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room2_Text_HueyIntro::
	.string "You'll sink like the last punk who\n"
	.string "came here!$"

SSAnne_B1F_Room2_Text_HueyDefeat::
	.string "Glub… I'm all washed up.$"

SSAnne_B1F_Room2_Text_HueyPostBattle::
	.string "Swim fast if you see a boat from\n"
	.string "TEAM ROCKET.$"

