ThreeIsland_House2_Frlg_MapScripts::
	.byte 0

ThreeIsland_House2_EventScript_Man::
	lock
	faceplayer
	famechecker FAMECHECKER_SABRINA, 1
	msgbox ThreeIsland_House2_Text_IAdmireSabrina
	release
	end

ThreeIsland_House2_EventScript_Rocker::
	msgbox ThreeIsland_House2_Text_CantMakeThisSpoonBend, MSGBOX_NPC
	end

ThreeIsland_House2_Text_CantMakeThisSpoonBend::
	.string "Urrrrrrrrgggghhhh… Kwaaaaah!\p"
	.string "…Useless! I can't bend this spoon\n"
	.string "with my mind.\p"
	.string "How does TEAM FLARE's psychic do\n"
	.string "it?$"

ThreeIsland_House2_Text_IAdmireSabrina::
	.string "Have you heard of LYSANDRE in\n"
	.string "SAFFRON?\p"
	.string "He took over the GYM with TEAM\n"
	.string "FLARE. Scary, but so stylish!$"

