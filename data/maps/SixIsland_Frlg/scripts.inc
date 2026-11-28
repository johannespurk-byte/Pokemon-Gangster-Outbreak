SixIsland_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_OnTransition
	.byte 0

SixIsland_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_SIX_ISLAND
	end

SixIsland_EventScript_Hiker::
	msgbox SixIsland_Text_ThatWayToWaterPathRuinValley, MSGBOX_NPC
	end

SixIsland_EventScript_Boy::
	msgbox SixIsland_Text_SkyAtNightIsFantastic, MSGBOX_NPC
	end

SixIsland_EventScript_IslandSign::
	msgbox SixIsland_Text_IslandSign, MSGBOX_SIGN
	end

SixIsland_Text_IslandSign::
	.string "SIX ISLAND\p"
	.string "Old and Wise. Nobody Bothers Us.$"

SixIsland_Text_ThatWayToWaterPathRuinValley::
	.string "Keep going this way to reach the\n"
	.string "WATER PATH. It leads to RUIN\p"
	.string "VALLEY.\p"
	.string "TEAM ROCKET has been sniffing\n"
	.string "around there too.$"

SixIsland_Text_SkyAtNightIsFantastic::
	.string "Far from the burning cities, the\n"
	.string "sky at night is fantastic.$"

