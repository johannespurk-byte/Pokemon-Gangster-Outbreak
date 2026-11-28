RockTunnel_B1F_Frlg_MapScripts::
	.byte 0

RockTunnel_B1F_EventScript_Sofia::
	trainerbattle_single TRAINER_PICNICKER_SOFIA, RockTunnel_B1F_Text_SofiaIntro, RockTunnel_B1F_Text_SofiaDefeat
	msgbox RockTunnel_B1F_Text_SofiaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Martha::
	trainerbattle_single TRAINER_PICNICKER_MARTHA, RockTunnel_B1F_Text_MarthaIntro, RockTunnel_B1F_Text_MarthaDefeat
	msgbox RockTunnel_B1F_Text_MarthaPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Dudley::
	trainerbattle_single TRAINER_HIKER_DUDLEY, RockTunnel_B1F_Text_DudleyIntro, RockTunnel_B1F_Text_DudleyDefeat
	msgbox RockTunnel_B1F_Text_DudleyPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Allen::
	trainerbattle_single TRAINER_HIKER_ALLEN, RockTunnel_B1F_Text_AllenIntro, RockTunnel_B1F_Text_AllenDefeat
	msgbox RockTunnel_B1F_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Eric::
	trainerbattle_single TRAINER_HIKER_ERIC, RockTunnel_B1F_Text_EricIntro, RockTunnel_B1F_Text_EricDefeat
	msgbox RockTunnel_B1F_Text_EricPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Cooper::
	trainerbattle_single TRAINER_POKEMANIAC_COOPER, RockTunnel_B1F_Text_CooperIntro, RockTunnel_B1F_Text_CooperDefeat
	msgbox RockTunnel_B1F_Text_CooperPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Steve::
	trainerbattle_single TRAINER_POKEMANIAC_STEVE, RockTunnel_B1F_Text_SteveIntro, RockTunnel_B1F_Text_SteveDefeat
	msgbox RockTunnel_B1F_Text_StevePostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_EventScript_Winston::
	trainerbattle_single TRAINER_POKEMANIAC_WINSTON, RockTunnel_B1F_Text_WinstonIntro, RockTunnel_B1F_Text_WinstonDefeat
	msgbox RockTunnel_B1F_Text_WinstonPostBattle, MSGBOX_AUTOCLOSE
	end

RockTunnel_B1F_Text_SofiaIntro::
	.string "My big brother rides with TEAM\n"
	.string "SKULL! I'm gonna be just like him!$"

RockTunnel_B1F_Text_SofiaDefeat::
	.string "Aww… There goes my lunch money.$"

RockTunnel_B1F_Text_SofiaPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "SKULL.\p"
	.string "Or maybe your crew!$"

RockTunnel_B1F_Text_DudleyIntro::
	.string "Out here, nobody pays the tolls of\n"
	.string "GUZMA!$"

RockTunnel_B1F_Text_DudleyDefeat::
	.string "Heh, you're tough as stone.$"

RockTunnel_B1F_Text_DudleyPostBattle::
	.string "Rest a bit. The road ahead is\n"
	.string "rough.$"

RockTunnel_B1F_Text_CooperIntro::
	.string "My calculations say you lose.\p"
	.string "Let's test it!$"

RockTunnel_B1F_Text_CooperDefeat::
	.string "Fine… I'll recalculate.$"

RockTunnel_B1F_Text_CooperPostBattle::
	.string "I'll sell your battle data to TEAM\n"
	.string "SKULL.\p"
	.string "Kidding! Mostly.$"

RockTunnel_B1F_Text_SteveIntro::
	.string "I sell info to every gang.\p"
	.string "Info on you is worth a lot!$"

RockTunnel_B1F_Text_SteveDefeat::
	.string "Error! That wasn't in my data!$"

RockTunnel_B1F_Text_StevePostBattle::
	.string "Data says you'll beat GUZMA.\p"
	.string "Probably.$"

RockTunnel_B1F_Text_AllenIntro::
	.string "Gangs come and go. The mountains\n"
	.string "stay!$"

RockTunnel_B1F_Text_AllenDefeat::
	.string "Whoa! Rolled right over me!$"

RockTunnel_B1F_Text_AllenPostBattle::
	.string "Gangs never climb this far. Too\n"
	.string "lazy.$"

RockTunnel_B1F_Text_MarthaIntro::
	.string "GUZMA says kids can't fight. I'll\n"
	.string "prove him wrong!$"

RockTunnel_B1F_Text_MarthaDefeat::
	.string "Okay, okay! You win!$"

RockTunnel_B1F_Text_MarthaPostBattle::
	.string "Everybody's scared of GUZMA.\p"
	.string "Are you scared?$"

RockTunnel_B1F_Text_EricIntro::
	.string "My POKéMON is as tough as the\n"
	.string "rocks here!$"

RockTunnel_B1F_Text_EricDefeat::
	.string "My birds flew off…$"

RockTunnel_B1F_Text_EricPostBattle::
	.string "Watch the skies. Even the birds\n"
	.string "avoid TEAM SKULL.$"

RockTunnel_B1F_Text_WinstonIntro::
	.string "Science doesn't stop for a gang\n"
	.string "war!$"

RockTunnel_B1F_Text_WinstonDefeat::
	.string "My theory… failed!$"

RockTunnel_B1F_Text_WinstonPostBattle::
	.string "Knowledge is the only thing they\n"
	.string "can't steal.$"

