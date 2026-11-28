CeruleanCity_House2_Frlg_MapScripts::
	.byte 0

CeruleanCity_House2_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_GOT_TM28_FROM_ROCKET, CeruleanCity_House2_EventScript_HikerGotTM28
	msgbox CeruleanCity_House2_Text_RocketsStoleTMForDig
	release
	end

CeruleanCity_House2_EventScript_HikerGotTM28::
	msgbox CeruleanCity_House2_Text_TeachDiglettDigWithoutTM
	release
	end

CeruleanCity_House2_EventScript_Lass::
	msgbox CeruleanCity_House2_Text_TeamRocketTryingToDigIntoNoGood, MSGBOX_NPC
	end

CeruleanCity_House2_EventScript_WallHole::
	msgbox CeruleanCity_House2_Text_TeamRocketLeftWayOut, MSGBOX_SIGN
	end

CeruleanCity_House2_Text_RocketsStoleTMForDig::
	.string "Those miserable SKULL punks!\p"
	.string "Look what they did to my house!\p"
	.string "They stole my TM for DIG!\p"
	.string "I was going to teach it to a\n"
	.string "SANDSHREW…\p"
	.string "That cost me a fortune!$"

CeruleanCity_House2_Text_TeachDiglettDigWithoutTM::
	.string "What's lost is lost.\p"
	.string "I taught my DIGLETT to DIG the\n"
	.string "old-fashioned way. TEAM SKULL\p"
	.string "can't steal that!$"

CeruleanCity_House2_Text_TeamRocketTryingToDigIntoNoGood::
	.string "The ROCKETS dug through my wall.\n"
	.string "TEAM SKULL took the rest!$"

CeruleanCity_House2_Text_TeamRocketLeftWayOut::
	.string "TEAM ROCKET left a way out!$"

