FiveIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FiveIsland_OnTransition
	.byte 0

FiveIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_FIVE_ISLAND
	end

FiveIsland_EventScript_IslandSign::
	msgbox FiveIsland_Text_IslandSign, MSGBOX_SIGN
	end

FiveIsland_EventScript_BugCatcher::
	msgbox FiveIsland_Text_WeirdBuildingInMeadow, MSGBOX_NPC
	end

FiveIsland_EventScript_Fisher::
	msgbox FiveIsland_Text_BeenGettingBusierAroundHere, MSGBOX_NPC
	end

FiveIsland_Text_IslandSign::
	.string "FIVE ISLAND\p"
	.string "Chrono Island. Strange Ships at\n"
	.string "Night.$"

FiveIsland_Text_WeirdBuildingInMeadow::
	.string "Oh, this stinks.\p"
	.string "TEAM ROCKET built a warehouse on\n"
	.string "the MEADOW. I can't play there\p"
	.string "anymore!$"

FiveIsland_Text_BeenGettingBusierAroundHere::
	.string "It's been getting busier around\n"
	.string "here.\p"
	.string "Strange ships land at night.\n"
	.string "ROCKET ships, if you ask me.$"

