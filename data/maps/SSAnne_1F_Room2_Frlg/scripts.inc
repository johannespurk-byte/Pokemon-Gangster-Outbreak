SSAnne_1F_Room2_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room2_EventScript_Woman::
	msgbox SSAnne_1F_Room2_Text_CruisingAroundWorld, MSGBOX_NPC
	end

SSAnne_1F_Room2_EventScript_Ann::
	trainerbattle_single TRAINER_LASS_ANN, SSAnne_1F_Room2_Text_AnnIntro, SSAnne_1F_Room2_Text_AnnDefeat
	msgbox SSAnne_1F_Room2_Text_AnnPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room2_EventScript_Tyler::
	trainerbattle_single TRAINER_YOUNGSTER_TYLER, SSAnne_1F_Room2_Text_TylerIntro, SSAnne_1F_Room2_Text_TylerDefeat
	msgbox SSAnne_1F_Room2_Text_TylerPostBattle, MSGBOX_AUTOCLOSE
	end

SSAnne_1F_Room2_Text_TylerIntro::
	.string "ARCHER says kids can't fight. I'll\n"
	.string "prove him wrong!$"

SSAnne_1F_Room2_Text_TylerDefeat::
	.string "No fair! You're way too strong!$"

SSAnne_1F_Room2_Text_TylerPostBattle::
	.string "Everybody's scared of ARCHER.\p"
	.string "Are you scared?$"

SSAnne_1F_Room2_Text_AnnIntro::
	.string "My big brother rides with TEAM\n"
	.string "ROCKET! I'm gonna be just like\p"
	.string "him!$"

SSAnne_1F_Room2_Text_AnnDefeat::
	.string "Waaah! I'm telling my brother!$"

SSAnne_1F_Room2_Text_AnnPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "ROCKET.\p"
	.string "Or maybe your crew!$"

SSAnne_1F_Room2_Text_CruisingAroundWorld::
	.string "We're sailing away from KANTO, my\n"
	.string "children and I. Far from the gangs.$"

