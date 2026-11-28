Route12_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, Route12_OnResume
	.byte 0

Route12_OnResume::
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, Route12_EventScript_TryRemoveSnorlax
	end

Route12_EventScript_TryRemoveSnorlax::
	removeobject VAR_LAST_TALKED
	return

Route12_EventScript_Snorlax::
	lock
	faceplayer
	goto_if_unset FLAG_GOT_POKE_FLUTE, Route12_EventScript_SnorlaxNoPokeFlute
	msgbox Text_WantToUsePokeFlute, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route12_EventScript_DontUsePokeFlute
	call EventScript_AwakenSnorlax
	setwildbattle SPECIES_SNORLAX, 30
	waitse
	playmoncry SPECIES_SNORLAX, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_HIDE_ROUTE_12_SNORLAX
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	setflag FLAG_WOKE_UP_ROUTE_12_SNORLAX
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route12_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, Route12_EventScript_FoughtSnorlax
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, Route12_EventScript_FoughtSnorlax
	release
	end

Route12_EventScript_DontUsePokeFlute::
	release
	end

Route12_EventScript_FoughtSnorlax::
	msgbox Text_SnorlaxReturnedToMountains
	release
	end

Route12_EventScript_SnorlaxNoPokeFlute::
	msgbox Route12_Text_MonSprawledOutInSlumber
	release
	end

Route12_EventScript_RouteSign::
	msgbox Route12_Text_RouteSign, MSGBOX_SIGN
	end

Route12_EventScript_FishingSign::
	msgbox Route12_Text_SportfishingArea, MSGBOX_SIGN
	end

Route12_Text_MonSprawledOutInSlumber::
	.string "A POKéMON is sprawled out in\n"
	.string "a deep and comfortable slumber.$"

Text_SnorlaxWokeUp::
	.string "SNORLAX woke up!\p"
	.string "It attacked in a grumpy rage!$"

Text_SnorlaxReturnedToMountains::
	.string "SNORLAX calmed down.\n"
	.string "It gave a huge yawn…\l"
	.string "And returned to the mountains.$"

Text_WantToUsePokeFlute::
	.string "Want to use the POKé FLUTE?$"

Text_PlayedPokeFlute::
	.string "{PLAYER} played the POKé FLUTE.$"

Route12_Text_NedIntro::
	.string "The sea doesn't care about gangs.\p"
	.string "But I do!$"

Route12_Text_NedDefeat::
	.string "Gah! Swallowed water!$"

Route12_Text_NedPostBattle::
	.string "Smugglers use these waters at\n"
	.string "night. Stay sharp.$"

Route12_Text_ChipIntro::
	.string "You'll sink like the last punk who\n"
	.string "came here!$"

Route12_Text_ChipDefeat::
	.string "Glub… I'm all washed up.$"

Route12_Text_ChipPostBattle::
	.string "Swim fast if you see a boat from\n"
	.string "TEAM ROCKET.$"

Route12_Text_JustinIntro::
	.string "I found a loot bag yesterday!\p"
	.string "Wanna fight for it?$"

Route12_Text_JustinDefeat::
	.string "Aww… There goes my lunch money.$"

Route12_Text_JustinPostBattle::
	.string "Don't tell my mom I battled you.$"

Route12_Text_LucaIntro::
	.string "ARCHER said nobody passes. That\n"
	.string "means you!$"

Route12_Text_LucaDefeat::
	.string "Fine! Keep your cash!$"

Route12_Text_LucaPostBattle::
	.string "Tell your crew I fought fair.\n"
	.string "Mostly.$"

Route12_Text_HankIntro::
	.string "Out here, only the strong stay\n"
	.string "afloat!$"

Route12_Text_HankDefeat::
	.string "Sunk! Totally sunk!$"

Route12_Text_HankPostBattle::
	.string "The water's the only free place\n"
	.string "left.$"

Route12_Text_ElliotIntro::
	.string "Splash! Nobody swims past me!$"

Route12_Text_ElliotDefeat::
	.string "The tide turned on me…$"

Route12_Text_ElliotPostBattle::
	.string "The currents carry loot bags. Keep\n"
	.string "your eyes open.$"

Route12_Text_AndrewIntro::
	.string "I patrol these waves for TEAM\n"
	.string "ROCKET!$"

Route12_Text_AndrewDefeat::
	.string "Gah! Swallowed water!$"

Route12_Text_AndrewPostBattle::
	.string "Ships from VERMILION pass here\n"
	.string "every night.$"

Route12_Text_RouteSign::
	.string "ROUTE 12\p"
	.string "North to LAVENDER - ROCKET\n"
	.string "CHECKPOINT$"

Route12_Text_SportfishingArea::
	.string "SPORTFISHING AREA\p"
	.string "Closed by order of TEAM ROCKET.$"

Route12_Text_JesIntro::
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

Route12_Text_JesDefeat::
	.string "Okay, okay! You win!$"

Route12_Text_JesPostBattle::
	.string "Can I join your crew when I'm\n"
	.string "older?$"

Route12_Text_JesNotEnoughMons::
	.string "JES: GIA and I, we'll be\n"
	.string "together forever.\p"
	.string "We won't battle unless you have\n"
	.string "two POKéMON of your own.$"

Route12_Text_GiaIntro::
	.string "My big brother rides with TEAM\n"
	.string "ROCKET! I'm gonna be just like\p"
	.string "him!$"

Route12_Text_GiaDefeat::
	.string "Waaah! I'm telling my brother!$"

Route12_Text_GiaPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "ROCKET.\p"
	.string "Or maybe your crew!$"

Route12_Text_GiaNotEnoughMons::
	.string "GIA: I can't bear to battle\n"
	.string "without my JES!\p"
	.string "Don't you have one more POKéMON?$"
