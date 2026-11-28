RockTunnel_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RockTunnel_1F_OnTransition
	.byte 0

RockTunnel_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_ROCK_TUNNEL_1F
	end

RockTunnel_1F_EventScript_RouteSign::
	msgbox RockTunnel_1F_Text_RouteSign, MSGBOX_SIGN
	end

RockTunnel_1F_EventScript_Lenny::
	trainerbattle_single TRAINER_HIKER_LENNY, RockTunnel_1F_Text_LennyIntro, RockTunnel_1F_Text_LennyDefeat
	msgbox RockTunnel_1F_Text_LennyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Oliver::
	trainerbattle_single TRAINER_HIKER_OLIVER, RockTunnel_1F_Text_OliverIntro, RockTunnel_1F_Text_OliverDefeat
	msgbox RockTunnel_1F_Text_OliverPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Lucas::
	trainerbattle_single TRAINER_HIKER_LUCAS, RockTunnel_1F_Text_LucasIntro, RockTunnel_1F_Text_LucasDefeat
	msgbox RockTunnel_1F_Text_LucasPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Ashton::
	trainerbattle_single TRAINER_POKEMANIAC_ASHTON, RockTunnel_1F_Text_AshtonIntro, RockTunnel_1F_Text_AshtonDefeat
	msgbox RockTunnel_1F_Text_AshtonPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Leah::
	trainerbattle_single TRAINER_PICNICKER_LEAH, RockTunnel_1F_Text_LeahIntro, RockTunnel_1F_Text_LeahDefeat
	msgbox RockTunnel_1F_Text_LeahPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Ariana::
	trainerbattle_single TRAINER_PICNICKER_ARIANA, RockTunnel_1F_Text_ArianaIntro, RockTunnel_1F_Text_ArianaDefeat
	msgbox RockTunnel_1F_Text_ArianaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_EventScript_Dana::
	trainerbattle_single TRAINER_PICNICKER_DANA, RockTunnel_1F_Text_DanaIntro, RockTunnel_1F_Text_DanaDefeat
	msgbox RockTunnel_1F_Text_DanaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_1F_Text_LennyIntro::
	.string "Out here, nobody pays the tolls of\n"
	.string "GUZMA!$"

RockTunnel_1F_Text_LennyDefeat::
	.string "My birds flew off…$"

RockTunnel_1F_Text_LennyPostBattle::
	.string "Rest a bit. The road ahead is\n"
	.string "rough.$"

RockTunnel_1F_Text_OliverIntro::
	.string "Gangs come and go. The mountains\n"
	.string "stay!$"

RockTunnel_1F_Text_OliverDefeat::
	.string "Like a rockslide!$"

RockTunnel_1F_Text_OliverPostBattle::
	.string "Gangs never climb this far. Too\n"
	.string "lazy.$"

RockTunnel_1F_Text_LucasIntro::
	.string "My POKéMON is as tough as the\n"
	.string "rocks here!$"

RockTunnel_1F_Text_LucasDefeat::
	.string "Heh, you're tough as stone.$"

RockTunnel_1F_Text_LucasPostBattle::
	.string "Watch the skies. Even the birds\n"
	.string "avoid TEAM SKULL.$"

RockTunnel_1F_Text_AshtonIntro::
	.string "Science doesn't stop for a gang\n"
	.string "war!$"

RockTunnel_1F_Text_AshtonDefeat::
	.string "Fine… I'll recalculate.$"

RockTunnel_1F_Text_AshtonPostBattle::
	.string "Knowledge is the only thing they\n"
	.string "can't steal.$"

RockTunnel_1F_Text_LeahIntro::
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

RockTunnel_1F_Text_LeahDefeat::
	.string "Okay, okay! You win!$"

RockTunnel_1F_Text_LeahPostBattle::
	.string "Can I join your crew when I'm\n"
	.string "older?$"

RockTunnel_1F_Text_DanaIntro::
	.string "GUZMA says kids can't fight. I'll\n"
	.string "prove him wrong!$"

RockTunnel_1F_Text_DanaDefeat::
	.string "No fair! You're way too strong!$"

RockTunnel_1F_Text_DanaPostBattle::
	.string "You looked cute and harmless.\n"
	.string "Was I ever wrong!$"

RockTunnel_1F_Text_ArianaIntro::
	.string "My big brother rides with TEAM\n"
	.string "SKULL! I'm gonna be just like him!$"

RockTunnel_1F_Text_ArianaDefeat::
	.string "Waaah! I'm telling my brother!$"

RockTunnel_1F_Text_ArianaPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "SKULL.\p"
	.string "Or maybe your crew!$"

RockTunnel_1F_Text_RouteSign::
	.string "ROCK TUNNEL\n"
	.string "CERULEAN CITY - LAVENDER TOWN$"

