SSAnne_2F_Room2_Frlg_MapScripts::
	.byte 0

SSAnne_2F_Room2_EventScript_Dale::
	trainerbattle_single TRAINER_FISHERMAN_DALE, SSAnne_2F_Room2_Text_DaleIntro, SSAnne_2F_Room2_Text_DaleDefeat
	msgbox SSAnne_2F_Room2_Text_DalePostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_2F_Room2_EventScript_Brooks::
	trainerbattle_single TRAINER_GENTLEMAN_BROOKS, SSAnne_2F_Room2_Text_BrooksIntro, SSAnne_2F_Room2_Text_BrooksDefeat
	msgbox SSAnne_2F_Room2_Text_BrooksPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_2F_Room2_Text_BrooksIntro::
	.string "Even in a gang war, one must look\n"
	.string "fabulous.$"

SSAnne_2F_Room2_Text_BrooksDefeat::
	.string "Oh my. You're rather strong.$"

SSAnne_2F_Room2_Text_BrooksPostBattle::
	.string "Money buys safety from TEAM\n"
	.string "ROCKET.\p"
	.string "Usually.$"

SSAnne_2F_Room2_Text_DaleIntro::
	.string "The sea doesn't care about gangs.\p"
	.string "But I do!$"

SSAnne_2F_Room2_Text_DaleDefeat::
	.string "Sunk! Totally sunk!$"

SSAnne_2F_Room2_Text_DalePostBattle::
	.string "Smugglers use these waters at\n"
	.string "night. Stay sharp.$"

