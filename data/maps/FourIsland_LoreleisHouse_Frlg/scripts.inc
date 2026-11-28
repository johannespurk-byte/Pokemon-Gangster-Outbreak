FourIsland_LoreleisHouse_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FourIsland_LoreleisHouse_OnTransition
	.byte 0

FourIsland_LoreleisHouse_OnTransition::
	special UpdateLoreleiDollCollection
	end

FourIsland_LoreleisHouse_EventScript_Lorelei::
	lock
	faceplayer
	goto_if_set FLAG_TALKED_TO_LORELEI_AFTER_WAREHOUSE, FourIsland_LoreleisHouse_EventScript_Lorelei3
	goto_if_set FLAG_DEFEATED_ROCKETS_IN_WAREHOUSE, FourIsland_LoreleisHouse_EventScript_Lorelei2
	msgbox FourIsland_LoreleisHouse_Text_IfAnythingWereToHappenToIsland
	release
	end

FourIsland_LoreleisHouse_EventScript_Lorelei2::
	setflag FLAG_TALKED_TO_LORELEI_AFTER_WAREHOUSE
	msgbox FourIsland_LoreleisHouse_Text_IllReturnToLeagueInShortWhile
	release
	end

FourIsland_LoreleisHouse_EventScript_Lorelei3::
	msgbox FourIsland_LoreleisHouse_Text_WillDoWhatICanHereAndNow
	release
	end

FourIsland_LoreleisHouse_EventScript_Doll::
	lock
	msgbox FourIsland_LoreleisHouse_Text_StuffedMonDollsGalore
	release
	end

FourIsland_LoreleisHouse_Text_IfAnythingWereToHappenToIsland::
	.string "LORELEI: Something weighs on my\n"
	.string "mind.\p"
	.string "If the gangs come to the island\n"
	.string "where I was born…\p"
	.string "I wouldn't know about it at the\n"
	.string "LEAGUE.\p"
	.string "Maybe I've neglected my home for\n"
	.string "too long…$"

FourIsland_LoreleisHouse_Text_IllReturnToLeagueInShortWhile::
	.string "LORELEI: You solved all the\n"
	.string "problems here?\p"
	.string "Wonderful. Then I can go back to\n"
	.string "KANTO.\p"
	.string "The LEAGUE is in the hands of\n"
	.string "strangers now. I want my place\p"
	.string "back.$"

FourIsland_LoreleisHouse_Text_WillDoWhatICanHereAndNow::
	.string "I don't know what the future\n"
	.string "holds, but…\p"
	.string "I'll protect this island here and\n"
	.string "now. That's all I can do.$"

FourIsland_LoreleisHouse_Text_StuffedMonDollsGalore::
	.string "Stuffed POKéMON dolls galore!$"

