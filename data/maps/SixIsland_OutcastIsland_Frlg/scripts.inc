SixIsland_OutcastIsland_Frlg_MapScripts::
	.byte 0

SixIsland_OutcastIsland_EventScript_Rocket::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_46, SixIsland_OutcastIsland_Text_RocketIntro, SixIsland_OutcastIsland_Text_RocketDefeat
	msgbox SixIsland_OutcastIsland_Text_RocketPostBattle, MSGBOX_AUTOCLOSE
	end

SixIsland_OutcastIsland_Text_RocketIntro::
	.string "Hand over your POKéMON. Orders\n"
	.string "from ARCHER!$"

SixIsland_OutcastIsland_Text_RocketDefeat::
	.string "I'm gonna be in so much trouble…$"

SixIsland_OutcastIsland_Text_RocketPostBattle::
	.string "Word is you're GIOVANNI's new\n"
	.string "star. Is that true?$"

SixIsland_OutcastIsland_Text_TylorIntro::
	.string "I patrol these waves for TEAM\n"
	.string "ROCKET!$"

SixIsland_OutcastIsland_Text_TylorDefeat::
	.string "Sunk! Totally sunk!$"

SixIsland_OutcastIsland_Text_TylorPostBattle::
	.string "Ships from the SEVII ISLANDS pass\n"
	.string "here every night.$"

SixIsland_OutcastIsland_Text_MymoIntro::
	.string "TEAM ROCKET took the docks, so I\n"
	.string "swim my own routes!$"

SixIsland_OutcastIsland_Text_MymoDefeat::
	.string "The tide turned on me…$"

SixIsland_OutcastIsland_Text_MymoPostBattle::
	.string "Even TEAM ROCKET can't control\n"
	.string "the sea.$"

SixIsland_OutcastIsland_Text_NicoleIntro::
	.string "The sea doesn't care about gangs.\p"
	.string "But I do!$"

SixIsland_OutcastIsland_Text_NicoleDefeat::
	.string "Gah! Swallowed water!$"

SixIsland_OutcastIsland_Text_NicolePostBattle::
	.string "Smugglers use these waters at\n"
	.string "night. Stay sharp.$"

SixIsland_OutcastIsland_Text_AvaIntro::
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

SixIsland_OutcastIsland_Text_AvaDefeat::
	.string "Aww… There goes my lunch money.$"

SixIsland_OutcastIsland_Text_AvaPostBattle::
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

SixIsland_OutcastIsland_Text_AvaNotEnoughMons::
	.string "AVA: You want to fight us?\p"
	.string "Then bring at least two POKéMON.$"

SixIsland_OutcastIsland_Text_GebIntro::
	.string "I'm the toughest kid on this road.\n"
	.string "Ask anyone!$"

SixIsland_OutcastIsland_Text_GebDefeat::
	.string "Okay, okay! You win!$"

SixIsland_OutcastIsland_Text_GebPostBattle::
	.string "The road to the SEVII ISLANDS\n"
	.string "isn't safe.\p"
	.string "Watch out!$"

SixIsland_OutcastIsland_Text_GebNotEnoughMons::
	.string "GEB: Fight me and my sister!\p"
	.string "…Oh, you don't have two POKéMON?$"
