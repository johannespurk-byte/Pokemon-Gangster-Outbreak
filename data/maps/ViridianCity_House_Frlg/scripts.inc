ViridianCity_House_Frlg_MapScripts::
	.byte 0

ViridianCity_House_EventScript_BaldingMan::
	msgbox ViridianCity_House_Text_NicknamingIsFun, MSGBOX_NPC
	end

ViridianCity_House_EventScript_LittleGirl::
	msgbox ViridianCity_House_Text_MyDaddyLovesMonsToo, MSGBOX_NPC
	end

ViridianCity_House_EventScript_Speary::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SPEAROW, CRY_MODE_NORMAL
	msgbox ViridianCity_House_Text_Speary
	waitmoncry
	release
	end

ViridianCity_House_EventScript_NicknameSign::
	msgbox ViridianCity_House_Text_SpearowNameSpeary, MSGBOX_SIGN
	end

ViridianCity_House_Text_NicknamingIsFun::
	.string "Everybody in VIRIDIAN works for\n"
	.string "GIOVANNI now. Even me.\p"
	.string "Give your POKéMON a tough name.\n"
	.string "Names matter on the street.$"

ViridianCity_House_Text_MyDaddyLovesMonsToo::
	.string "My daddy drives a truck for\n"
	.string "the ROCKETS. He says it's safe.$"

ViridianCity_House_Text_Speary::
	.string "SPEARY: Tetweet!$"

ViridianCity_House_Text_SpearowNameSpeary::
	.string "SPEAROW\n"
	.string "Name: SPEARY$"

