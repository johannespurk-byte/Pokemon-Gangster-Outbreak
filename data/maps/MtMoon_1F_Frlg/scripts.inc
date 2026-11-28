MtMoon_1F_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MtMoon_1F_OnTransition
	.byte 0

MtMoon_1F_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_MT_MOON_1F
	end

MtMoon_1F_EventScript_ZubatSign::
	msgbox MtMoon_1F_Text_ZubatIsABloodsucker, MSGBOX_SIGN
	end

MtMoon_1F_EventScript_Marcos::
	trainerbattle_single TRAINER_HIKER_MARCOS, MtMoon_1F_Text_MarcosIntro, MtMoon_1F_Text_MarcosDefeat
	msgbox MtMoon_1F_Text_MarcosPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Josh::
	trainerbattle_single TRAINER_YOUNGSTER_JOSH, MtMoon_1F_Text_JoshIntro, MtMoon_1F_Text_JoshDefeat
	msgbox MtMoon_1F_Text_JoshPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Miriam::
	trainerbattle_single TRAINER_LASS_MIRIAM, MtMoon_1F_Text_MiriamIntro, MtMoon_1F_Text_MiriamDefeat
	msgbox MtMoon_1F_Text_MiriamPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Iris::
	trainerbattle_single TRAINER_LASS_IRIS, MtMoon_1F_Text_IrisIntro, MtMoon_1F_Text_IrisDefeat
	msgbox MtMoon_1F_Text_IrisPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Jovan::
	trainerbattle_single TRAINER_SUPER_NERD_JOVAN, MtMoon_1F_Text_JovanIntro, MtMoon_1F_Text_JovanDefeat
	msgbox MtMoon_1F_Text_JovanPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Kent::
	trainerbattle_single TRAINER_BUG_CATCHER_KENT, MtMoon_1F_Text_KentIntro, MtMoon_1F_Text_KentDefeat
	msgbox MtMoon_1F_Text_KentPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_EventScript_Robby::
	trainerbattle_single TRAINER_BUG_CATCHER_ROBBY, MtMoon_1F_Text_RobbyIntro, MtMoon_1F_Text_RobbyDefeat
	msgbox MtMoon_1F_Text_RobbyPostBattle, MSGBOX_AUTOCLOSE
	end

MtMoon_1F_Text_MarcosIntro::
	.string "Gangs come and go. The mountains\n"
	.string "stay!$"

MtMoon_1F_Text_MarcosDefeat::
	.string "Like a rockslide!$"

MtMoon_1F_Text_MarcosPostBattle::
	.string "Gangs never climb this far. Too\n"
	.string "lazy.$"

MtMoon_1F_Text_JoshIntro::
	.string "I found a loot bag yesterday!\p"
	.string "Wanna fight for it?$"

MtMoon_1F_Text_JoshDefeat::
	.string "Aww… There goes my lunch money.$"

MtMoon_1F_Text_JoshPostBattle::
	.string "Don't tell my mom I battled you.$"

MtMoon_1F_Text_MiriamIntro::
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

MtMoon_1F_Text_MiriamDefeat::
	.string "Okay, okay! You win!$"

MtMoon_1F_Text_MiriamPostBattle::
	.string "Can I join your crew when I'm\n"
	.string "older?$"

MtMoon_1F_Text_JovanIntro::
	.string "I sell info to every gang.\p"
	.string "Info on you is worth a lot!$"

MtMoon_1F_Text_JovanDefeat::
	.string "Impossible! I calculated\n"
	.string "everything!$"

MtMoon_1F_Text_JovanPostBattle::
	.string "Data says you'll beat PIERS.\p"
	.string "Probably.$"

MtMoon_1F_Text_IrisIntro::
	.string "My big brother rides with TEAM\n"
	.string "YELL! I'm gonna be just like him!$"

MtMoon_1F_Text_IrisDefeat::
	.string "Waaah! I'm telling my brother!$"

MtMoon_1F_Text_IrisPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "YELL.\p"
	.string "Or maybe your crew!$"

MtMoon_1F_Text_KentIntro::
	.string "PIERS says kids can't fight. I'll\n"
	.string "prove him wrong!$"

MtMoon_1F_Text_KentDefeat::
	.string "No fair! You're way too strong!$"

MtMoon_1F_Text_KentPostBattle::
	.string "Everybody's scared of PIERS.\p"
	.string "Are you scared?$"

MtMoon_1F_Text_RobbyIntro::
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

MtMoon_1F_Text_RobbyDefeat::
	.string "Aww… There goes my lunch money.$"

MtMoon_1F_Text_RobbyPostBattle::
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

MtMoon_1F_Text_ZubatIsABloodsucker::
	.string "Beware!\n"
	.string "ZUBAT is a bloodsucker!$"

MtMoon_1F_Text_BrockHelpsExcavateFossils::
	.string "Hi, I'm excavating for fossils here\n"
	.string "under MT. MOON.\p"
	.string "Sometimes, BROCK of PEWTER GYM\n"
	.string "lends me a hand.$"
