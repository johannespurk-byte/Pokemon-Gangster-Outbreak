SSAnne_B1F_Room3_Frlg_MapScripts::
	.byte 0

SSAnne_B1F_Room3_EventScript_Dylan::
	trainerbattle_single TRAINER_SAILOR_DYLAN, SSAnne_B1F_Room3_Text_DylanIntro, SSAnne_B1F_Room3_Text_DylanDefeat
	msgbox SSAnne_B1F_Room3_Text_DylanPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_B1F_Room3_Text_DylanIntro::
	.string "I patrol these waves for TEAM\n"
	.string "ROCKET!$"

SSAnne_B1F_Room3_Text_DylanDefeat::
	.string "Sunk! Totally sunk!$"

SSAnne_B1F_Room3_Text_DylanPostBattle::
	.string "Ships from VERMILION pass here\n"
	.string "every night.$"

