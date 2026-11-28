SSAnne_2F_Room1_Frlg_MapScripts::
	.byte 0

SSAnne_2F_Room1_EventScript_Gentleman::
	lock
	faceplayer
	message SSAnne_2F_Room1_Text_SleepingMonLookedLikeThis
	waitmessage
	setvar VAR_0x8004, SPECIES_SNORLAX
	special SetSeenMon
	showmonpic SPECIES_SNORLAX, 10, 3
	waitbuttonpress
	hidemonpic
	release
	end

SSAnne_2F_Room1_Text_SleepingMonLookedLikeThis::
	.string "I've traveled far, but I've never\n"
	.string "seen a POKéMON sleep like this\p"
	.string "one!\p"
	.string "The ROCKETS drugged it, I bet. It\n"
	.string "looked like this!$"

