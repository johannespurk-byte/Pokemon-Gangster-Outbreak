ViridianForest_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianForest_OnTransition
	.byte 0

ViridianForest_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_VIRIDIAN_FOREST
	end

ViridianForest_EventScript_Youngster::
	msgbox ViridianForest_Text_FriendsItchingToBattle, MSGBOX_NPC
	end

ViridianForest_EventScript_Boy::
	msgbox ViridianForest_Text_RanOutOfPokeBalls, MSGBOX_NPC
	end

ViridianForest_EventScript_TrainerTips1::
	msgbox ViridianForest_Text_AvoidGrassyAreasWhenWeak, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips2::
	msgbox ViridianForest_Text_UseAntidoteForPoison, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips3::
	msgbox ViridianForest_Text_ContactOakViaPCToRatePokedex, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips4::
	msgbox ViridianForest_Text_CantCatchOwnedMons, MSGBOX_SIGN
	end

ViridianForest_EventScript_TrainerTips5::
	msgbox ViridianForest_Text_WeakenMonsBeforeCapture, MSGBOX_SIGN
	end

ViridianForest_EventScript_ExitSign::
	msgbox ViridianForest_Text_LeavingViridianForest, MSGBOX_SIGN
	end

ViridianForest_EventScript_Rick::
	trainerbattle_single TRAINER_BUG_CATCHER_RICK, ViridianForest_Text_RickIntro, ViridianForest_Text_RickDefeat
	msgbox ViridianForest_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Doug::
	trainerbattle_single TRAINER_BUG_CATCHER_DOUG, ViridianForest_Text_DougIntro, ViridianForest_Text_DougDefeat
	msgbox ViridianForest_Text_DougPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Sammy::
	trainerbattle_single TRAINER_BUG_CATCHER_SAMMY, ViridianForest_Text_SammyIntro, ViridianForest_Text_SammyDefeat
	msgbox ViridianForest_Text_SammyPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Anthony::
	trainerbattle_single TRAINER_BUG_CATCHER_ANTHONY, ViridianForest_Text_AnthonyIntro, ViridianForest_Text_AnthonyDefeat
	msgbox ViridianForest_Text_AnthonyPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_EventScript_Charlie::
	trainerbattle_single TRAINER_BUG_CATCHER_CHARLIE, ViridianForest_Text_CharlieIntro, ViridianForest_Text_CharlieDefeat
	msgbox ViridianForest_Text_CharliePostBattle, MSGBOX_AUTOCLOSE
	end

ViridianForest_Text_FriendsItchingToBattle::
	.string "I came here with friends to catch\n"
	.string "BUG POKéMON!\p"
	.string "We're hiding from TEAM ROCKET. And\n"
	.string "itching to battle!$"

ViridianForest_Text_RickIntro::
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

ViridianForest_Text_RickDefeat::
	.string "No fair! You're way too strong!$"

ViridianForest_Text_RickPostBattle::
	.string "Can I join your crew when I'm\n"
	.string "older?$"

ViridianForest_Text_DougIntro::
	.string "My big brother rides with TEAM\n"
	.string "ROCKET! I'm gonna be just like\p"
	.string "him!$"

ViridianForest_Text_DougDefeat::
	.string "Aww… There goes my lunch money.$"

ViridianForest_Text_DougPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "ROCKET.\p"
	.string "Or maybe your crew!$"

ViridianForest_Text_SammyIntro::
	.string "GIOVANNI says kids can't fight.\n"
	.string "I'll prove him wrong!$"

ViridianForest_Text_SammyDefeat::
	.string "Okay, okay! You win!$"

ViridianForest_Text_SammyPostBattle::
	.string "Everybody's scared of GIOVANNI.\p"
	.string "Are you scared?$"

ViridianForest_Text_AnthonyIntro::
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

ViridianForest_Text_AnthonyDefeat::
	.string "Waaah! I'm telling my brother!$"

ViridianForest_Text_AnthonyPostBattle::
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

ViridianForest_Text_CharlieIntro::
	.string "I'm the toughest kid on this road.\n"
	.string "Ask anyone!$"

ViridianForest_Text_CharlieDefeat::
	.string "No fair! You're way too strong!$"

ViridianForest_Text_CharliePostBattle::
	.string "The road to VIRIDIAN isn't safe.\p"
	.string "Watch out!$"

ViridianForest_Text_RanOutOfPokeBalls::
	.string "I ran out of POKé BALLS.\p"
	.string "A ROCKET took the rest. Never\n"
	.string "carry too few!$"

ViridianForest_Text_AvoidGrassyAreasWhenWeak::
	.string "TRAINER TIPS\p"
	.string "If your POKéMON are weak and you\n"
	.string "want to avoid battles, stay away\l"
	.string "from grassy areas!$"

ViridianForest_Text_UseAntidoteForPoison::
	.string "For poison, use ANTIDOTE! The\n"
	.string "vending machines sell them.$"

ViridianForest_Text_ContactOakViaPCToRatePokedex::
	.string "TRAINER TIPS\p"
	.string "Contact PROF. OAK via a PC to\n"
	.string "get your POKéDEX evaluated!$"

ViridianForest_Text_CantCatchOwnedMons::
	.string "TRAINER TIPS\p"
	.string "You can't catch a POKéMON that\n"
	.string "belongs to someone else.\p"
	.string "Throw POKé BALLS only at wild\n"
	.string "POKéMON to catch them!$"

ViridianForest_Text_WeakenMonsBeforeCapture::
	.string "TRAINER TIPS\p"
	.string "Weaken POKéMON before attempting\n"
	.string "capture!\p"
	.string "When healthy, they may escape!$"

ViridianForest_Text_LeavingViridianForest::
	.string "LEAVING VIRIDIAN FOREST\p"
	.string "PEWTER CITY AHEAD - TEAM YELL\n"
	.string "TURF$"

