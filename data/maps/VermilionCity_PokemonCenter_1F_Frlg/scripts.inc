VermilionCity_PokemonCenter_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermilionCity_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

VermilionCity_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_VERMILION_CITY
	end

VermilionCity_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse_Frlg
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_Man::
	msgbox VermilionCity_PokemonCenter_1F_Text_PoisonedMonFaintedWhileWalking, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Hiker::
	msgbox VermilionCity_PokemonCenter_1F_Text_TrainerMonsStrongerThanWild, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_Youngster::
	msgbox VermilionCity_PokemonCenter_1F_Text_AllMonWeakToSpecificTypes, MSGBOX_NPC
	end

VermilionCity_PokemonCenter_1F_EventScript_VSSeekerWoman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_VS_SEEKER, VermilionCity_PokemonCenter_1F_EventScript_ExplainVSSeeker
	msgbox VermilionCity_PokemonCenter_1F_Text_UrgeToBattleSomeoneAgain
	setflag FLAG_GOT_VS_SEEKER
	giveitem ITEM_VS_SEEKER
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	msgbox VermilionCity_PokemonCenter_1F_Text_UseDeviceForRematches
	release
	end

VermilionCity_PokemonCenter_1F_EventScript_ExplainVSSeeker::
	msgbox VermilionCity_PokemonCenter_1F_Text_ExplainVSSeeker
	release
	end

VermilionCity_PokemonCenter_1F_Text_TrainerMonsStrongerThanWild::
	.string "Same level, different stats.\p"
	.string "A POKéMON raised by a TRAINER is\n"
	.string "stronger than a wild one. That's\p"
	.string "why the gangs steal them.$"

VermilionCity_PokemonCenter_1F_Text_PoisonedMonFaintedWhileWalking::
	.string "My POKéMON got poisoned in a\n"
	.string "street fight!\p"
	.string "It fainted on the way here!$"

VermilionCity_PokemonCenter_1F_Text_AllMonWeakToSpecificTypes::
	.string "A higher level helps…\p"
	.string "But every POKéMON has a weak\n"
	.string "type.\p"
	.string "There's no unbeatable POKéMON.\n"
	.string "Not even GIOVANNI's.$"

VermilionCity_PokemonCenter_1F_Text_UrgeToBattleSomeoneAgain::
	.string "Ever want to fight someone again?\n"
	.string "To settle the score?\p"
	.string "Out here, everybody has a score to\n"
	.string "settle.\p"
	.string "I've been handing these out. Take\n"
	.string "one!$"

VermilionCity_PokemonCenter_1F_Text_UseDeviceForRematches::
	.string "Use that device to find TRAINERS\n"
	.string "who want a rematch.\p"
	.string "You have to charge its battery\n"
	.string "first.$"

VermilionCity_PokemonCenter_1F_Text_ExplainVSSeeker::
	.string "The VS SEEKER? Easy.\p"
	.string "Use it and nearby TRAINERS will\n"
	.string "notice you.\p"
	.string "Anyone who wants a rematch will\n"
	.string "let you know right away.\p"
	.string "Charge its battery and use it on a\n"
	.string "road.$"
