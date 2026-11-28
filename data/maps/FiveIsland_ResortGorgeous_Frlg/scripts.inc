FiveIsland_ResortGorgeous_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, FiveIsland_ResortGorgeous_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, FiveIsland_ResortGorgeous_OnFrame
	.byte 0

FiveIsland_ResortGorgeous_OnWarp::
	map_script_2 VAR_MAP_SCENE_FIVE_ISLAND_RESORT_GORGEOUS, 1, FiveIsland_ResortGorgeous_EventScript_TurnPlayerNorth
	.2byte 0

FiveIsland_ResortGorgeous_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

FiveIsland_ResortGorgeous_OnFrame::
	map_script_2 VAR_MAP_SCENE_FIVE_ISLAND_RESORT_GORGEOUS, 1, FiveIsland_ResortGorgeous_EventScript_SelphyReturnHomeScene
	.2byte 0

FiveIsland_ResortGorgeous_EventScript_SelphyReturnHomeScene::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox FiveIsland_ResortGorgeous_Text_SelphyThanksYouMayGoNow
	closemessage
	applymovement LOCALID_RESORT_GORGEOUS_SELPHY, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	opendoor 39, 8
	waitdooranim
	applymovement LOCALID_RESORT_GORGEOUS_SELPHY, FiveIsland_ResortGorgeous_Movement_SelphyEnterHome
	waitmovement 0
	closedoor 39, 8
	waitdooranim
	removeobject LOCALID_RESORT_GORGEOUS_SELPHY
	clearflag FLAG_HIDE_RESORT_GORGEOUS_INSIDE_SELPHY
	setvar VAR_MAP_SCENE_FIVE_ISLAND_RESORT_GORGEOUS, 2
	releaseall
	end

FiveIsland_ResortGorgeous_Movement_SelphyEnterHome::
	walk_up
	set_invisible
	step_end

FiveIsland_ResortGorgeous_EventScript_SelphysHouseSign::
	msgbox FiveIsland_ResortGorgeous_Text_SelphysHouse, MSGBOX_SIGN
	end

FiveIsland_ResortGorgeous_Text_DaisyIntro::
	.string "Even in a gang war, one must look\n"
	.string "fabulous.$"

FiveIsland_ResortGorgeous_Text_DaisyDefeat::
	.string "My outfit! It's ruined!$"

FiveIsland_ResortGorgeous_Text_DaisyPostBattle::
	.string "Money buys safety from TEAM\n"
	.string "ROCKET.\p"
	.string "Usually.$"

FiveIsland_ResortGorgeous_Text_CelinaIntro::
	.string "ARCHER invited me to his parties.\p"
	.string "Jealous?$"

FiveIsland_ResortGorgeous_Text_CelinaDefeat::
	.string "How… vulgar of you to win.$"

FiveIsland_ResortGorgeous_Text_CelinaPostBattle::
	.string "You fight like a true boss.\p"
	.string "How charming.$"

FiveIsland_ResortGorgeous_Text_RaynaIntro::
	.string "Ugh, a street kid.\p"
	.string "Let's get this over with.$"

FiveIsland_ResortGorgeous_Text_RaynaDefeat::
	.string "Oh my. You're rather strong.$"

FiveIsland_ResortGorgeous_Text_RaynaPostBattle::
	.string "Do tell ARCHER I said hello.$"

FiveIsland_ResortGorgeous_Text_JackiIntro::
	.string "Style is a weapon too, darling!$"

FiveIsland_ResortGorgeous_Text_JackiDefeat::
	.string "I broke a nail! Unforgivable!$"

FiveIsland_ResortGorgeous_Text_JackiPostBattle::
	.string "Dirty roads, dirty fights.\p"
	.string "Such is life.$"

FiveIsland_ResortGorgeous_Text_GillianIntro::
	.string "Darling, this road is far too dirty\n"
	.string "for you.$"

FiveIsland_ResortGorgeous_Text_GillianDefeat::
	.string "My outfit! It's ruined!$"

FiveIsland_ResortGorgeous_Text_GillianPostBattle::
	.string "The parties in the SEVII ISLANDS\n"
	.string "are wild these days.$"

FiveIsland_ResortGorgeous_Text_DestinIntro::
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

FiveIsland_ResortGorgeous_Text_DestinDefeat::
	.string "Aww… There goes my lunch money.$"

FiveIsland_ResortGorgeous_Text_DestinPostBattle::
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

FiveIsland_ResortGorgeous_Text_TobyIntro::
	.string "You'll sink like the last punk who\n"
	.string "came here!$"

FiveIsland_ResortGorgeous_Text_TobyDefeat::
	.string "Glub… I'm all washed up.$"

FiveIsland_ResortGorgeous_Text_TobyPostBattle::
	.string "Swim fast if you see a boat from\n"
	.string "TEAM ROCKET.$"

FiveIsland_ResortGorgeous_Text_SelphyThanksYouMayGoNow::
	.string "SELPHY: Thank you.\p"
	.string "You may go now.$"

FiveIsland_ResortGorgeous_Text_SelphysHouse::
	.string "SELPHY'S HOUSE$"

