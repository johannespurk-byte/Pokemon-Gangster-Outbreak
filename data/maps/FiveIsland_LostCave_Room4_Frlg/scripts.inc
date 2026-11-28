FiveIsland_LostCave_Room4_Frlg_MapScripts::
	.byte 0

FiveIsland_LostCave_Room4_EventScript_Laura::
	trainerbattle_single TRAINER_PSYCHIC_LAURA, FiveIsland_LostCave_Room4_Text_LauraIntro, FiveIsland_LostCave_Room4_Text_LauraDefeat
	msgbox FiveIsland_LostCave_Room4_Text_LauraPostBattle, MSGBOX_AUTOCLOSE
	end

FiveIsland_LostCave_Room4_Text_LauraIntro::
	.string "The spirits whisper of ARCHER.\p"
	.string "They are afraid.$"

FiveIsland_LostCave_Room4_Text_LauraDefeat::
	.string "I did not foresee this!$"

FiveIsland_LostCave_Room4_Text_LauraPostBattle::
	.string "Even ghosts avoid TEAM ROCKET.$"

