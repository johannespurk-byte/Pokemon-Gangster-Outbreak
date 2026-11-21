ViridianCity_Gym_Frlg_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, ViridianCity_Gym_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, ViridianCity_Gym_OnFrame
	.byte 0

@ Gangster Outbreak: the game starts here, in Giovanni's hideout.
@ VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB is 0 only before the opening scene.
ViridianCity_Gym_OnWarp::
	map_script_2 VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 0, ViridianCity_Gym_EventScript_FaceGiovanni
	.2byte 0

ViridianCity_Gym_EventScript_FaceGiovanni::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

ViridianCity_Gym_OnFrame::
	map_script_2 VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 0, ViridianCity_Gym_EventScript_GangIntro
	.2byte 0

ViridianCity_Gym_EventScript_GangIntro::
	lockall
	setrespawn HEAL_LOCATION_VIRIDIAN_CITY
	setvar VAR_MAP_SCENE_PALLET_TOWN_PLAYERS_HOUSE_2F, 1
	call EventScript_SetViridianGymTrainers
	delay 30
	textcolor NPC_TEXT_COLOR_MALE
	msgbox ViridianCity_Gym_Text_GangIntroWelcome
	closemessage
	setflag FLAG_SYS_POKEMON_GET
	special GangIntro_GiveRandomStarter
	bufferspeciesname STR_VAR_1, VAR_0x8004
	copyvar VAR_0x800A, VAR_0x8005
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message ViridianCity_Gym_Text_GangIntroReceivedMon
	waitmessage
	waitfanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_Gym_EventScript_GangIntroSilver
	setvar VAR_0x8004, 0
	call Common_EventScript_NameReceivedPartyMon
ViridianCity_Gym_EventScript_GangIntroSilver::
	bufferspeciesname STR_VAR_2, VAR_0x800A
	textcolor NPC_TEXT_COLOR_MALE
	msgbox ViridianCity_Gym_Text_GangIntroGiovanniSilver
	closemessage
	turnobject LOCALID_PLAYER, DIR_EAST
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
	msgbox ViridianCity_Gym_Text_GangIntroSilverChallenge
	closemessage
	switch VAR_STARTER_MON
	case 1, ViridianCity_Gym_EventScript_GangIntroBattleBulbasaur
	case 2, ViridianCity_Gym_EventScript_GangIntroBattleSquirtle
	trainerbattle_earlyrival TRAINER_RIVAL_OAKS_LAB_CHARMANDER, RIVAL_BATTLE_TUTORIAL, ViridianCity_Gym_Text_GangIntroSilverDefeat, ViridianCity_Gym_Text_GangIntroSilverVictory
	goto ViridianCity_Gym_EventScript_GangIntroAfterBattle
	end

ViridianCity_Gym_EventScript_GangIntroBattleBulbasaur::
	trainerbattle_earlyrival TRAINER_RIVAL_OAKS_LAB_BULBASAUR, RIVAL_BATTLE_TUTORIAL, ViridianCity_Gym_Text_GangIntroSilverDefeat, ViridianCity_Gym_Text_GangIntroSilverVictory
	goto ViridianCity_Gym_EventScript_GangIntroAfterBattle
	end

ViridianCity_Gym_EventScript_GangIntroBattleSquirtle::
	trainerbattle_earlyrival TRAINER_RIVAL_OAKS_LAB_SQUIRTLE, RIVAL_BATTLE_TUTORIAL, ViridianCity_Gym_Text_GangIntroSilverDefeat, ViridianCity_Gym_Text_GangIntroSilverVictory
	goto ViridianCity_Gym_EventScript_GangIntroAfterBattle
	end

ViridianCity_Gym_EventScript_GangIntroAfterBattle::
	special HealPlayerParty
	textcolor NPC_TEXT_COLOR_MALE
	msgbox ViridianCity_Gym_Text_GangIntroSilverLeaves
	closemessage
	playbgm MUS_RG_RIVAL_EXIT, 0
	fadescreen FADE_TO_BLACK
	setflag FLAG_BEAT_RIVAL_IN_OAKS_LAB
	removeobject LOCALID_VIRIDIAN_GYM_SILVER
	playse SE_EXIT
	fadescreen FADE_FROM_BLACK
	fadedefaultbgm
	turnobject LOCALID_PLAYER, DIR_NORTH
	msgbox ViridianCity_Gym_Text_GangIntroGiovanniDex
	closemessage
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_RG_OBTAIN_KEY_ITEM
	message ViridianCity_Gym_Text_GangIntroReceivedDex
	waitmessage
	waitfanfare
	call EventScript_RestorePrevTextColor
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	special EnableNationalPokedex
	textcolor NPC_TEXT_COLOR_MALE
	giveitem_msg ViridianCity_Gym_Text_GangIntroReceivedBalls, ITEM_POKE_BALL, 5
	msgbox ViridianCity_Gym_Text_GangIntroGiovanniCandies
	giveitem_msg ViridianCity_Gym_Text_GangIntroReceivedCandies, ITEM_RARE_CANDY, 30
	msgbox ViridianCity_Gym_Text_GangIntroGiovanniGadgets
	giveitem ITEM_POKE_VIAL
	giveitem ITEM_AUTO_REPEL
	special GangItems_Recharge
	msgbox ViridianCity_Gym_Text_GangIntroGiovanniGadgets2
	msgbox ViridianCity_Gym_Text_GangIntroMission
	closemessage
	setvar VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 6
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_MART, 2
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 1
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 1
	setvar VAR_MAP_SCENE_ROUTE22, 1
	setvar VAR_MAP_SCENE_POKEMON_CENTER_TEALA, 1
	setvar VAR_MAP_SCENE_PALLET_TOWN_SIGN_LADY, 1
	setvar VAR_MAP_SCENE_PALLET_TOWN_OAK, 1
	setflag FLAG_PALLET_LADY_NOT_BLOCKING_SIGN
	releaseall
	end

@ Before the player has 7 badges, Giovanni only talks.
ViridianCity_Gym_EventScript_GiovanniHQ::
	msgbox ViridianCity_Gym_Text_GiovanniHQ, MSGBOX_NPC
	end

ViridianCity_Gym_EventScript_Giovanni::
	goto_if_unset FLAG_BADGE07_GET, ViridianCity_Gym_EventScript_GiovanniHQ
	famechecker FAMECHECKER_GIOVANNI, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_GIOVANNI, ViridianCity_Gym_Text_GiovanniIntro, ViridianCity_Gym_Text_GiovanniDefeat, ViridianCity_Gym_EventScript_DefeatedGiovanni, NO_MUSIC
	goto_if_unset FLAG_GOT_TM26_FROM_GIOVANNI, ViridianCity_Gym_EventScript_GiveTM26
	msgbox ViridianCity_Gym_Text_GiovanniPostBattle
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_GIOVANNI
	fadescreen FADE_FROM_BLACK
	release
	end

ViridianCity_Gym_EventScript_DefeatedGiovanni::
	famechecker FAMECHECKER_GIOVANNI, 3
	setflag FLAG_HIDE_MISC_KANTO_ROCKETS
	setflag FLAG_DEFEATED_LEADER_GIOVANNI
	setflag FLAG_BADGE08_GET
	setvar VAR_MAP_SCENE_ROUTE22, 3
	set_gym_trainers_frlg 8
	goto ViridianCity_Gym_EventScript_GiveTM26
	end

ViridianCity_Gym_EventScript_GiveTM26::
	msgbox ViridianCity_Gym_Text_ExplainEarthBadgeTakeThis
	checkitemspace ITEM_TM26
	goto_if_eq VAR_RESULT, FALSE, ViridianCity_Gym_EventScript_NoRoomForTM26
	giveitem_msg ViridianCity_Gym_Text_ReceivedTM26FromGiovanni, ITEM_TM26
	setflag FLAG_GOT_TM26_FROM_GIOVANNI
	msgbox ViridianCity_Gym_Text_ExplainTM26
	release
	end

ViridianCity_Gym_EventScript_NoRoomForTM26::
	msgbox ViridianCity_Gym_Text_YouDoNotHaveSpace
	release
	end

ViridianCity_Gym_EventScript_Jason::
	trainerbattle_single TRAINER_TAMER_JASON, ViridianCity_Gym_Text_JasonIntro, ViridianCity_Gym_Text_JasonDefeat
	msgbox ViridianCity_Gym_Text_JasonPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Cole::
	trainerbattle_single TRAINER_TAMER_COLE, ViridianCity_Gym_Text_ColeIntro, ViridianCity_Gym_Text_ColeDefeat
	msgbox ViridianCity_Gym_Text_ColePostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Atsushi::
	trainerbattle_single TRAINER_BLACK_BELT_ATSUSHI, ViridianCity_Gym_Text_AtsushiIntro, ViridianCity_Gym_Text_AtsushiDefeat
	msgbox ViridianCity_Gym_Text_AtsushiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Kiyo::
	trainerbattle_single TRAINER_BLACK_BELT_KIYO, ViridianCity_Gym_Text_KiyoIntro, ViridianCity_Gym_Text_KiyoDefeat
	msgbox ViridianCity_Gym_Text_KiyoPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Takashi::
	trainerbattle_single TRAINER_BLACK_BELT_TAKASHI, ViridianCity_Gym_Text_TakashiIntro, ViridianCity_Gym_Text_TakashiDefeat
	msgbox ViridianCity_Gym_Text_TakashiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Samuel::
	trainerbattle_single TRAINER_COOLTRAINER_SAMUEL, ViridianCity_Gym_Text_SamuelIntro, ViridianCity_Gym_Text_SamuelDefeat
	msgbox ViridianCity_Gym_Text_SamuelPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Yuji::
	trainerbattle_single TRAINER_COOLTRAINER_YUJI, ViridianCity_Gym_Text_YujiIntro, ViridianCity_Gym_Text_YujiDefeat
	msgbox ViridianCity_Gym_Text_YujiPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_Warren::
	trainerbattle_single TRAINER_COOLTRAINER_WARREN, ViridianCity_Gym_Text_WarrenIntro, ViridianCity_Gym_Text_WarrenDefeat
	msgbox ViridianCity_Gym_Text_WarrenPostBattle, MSGBOX_AUTOCLOSE
	end

ViridianCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_LEADER_GIOVANNI, ViridianCity_Gym_EventScript_GymGuyPostVictory
	msgbox ViridianCity_Gym_Text_GymGuyAdvice
	release
	end

ViridianCity_Gym_EventScript_GymGuyPostVictory::
	famechecker FAMECHECKER_GIOVANNI, 4
	msgbox ViridianCity_Gym_Text_GymGuyPostVictory
	release
	end

ViridianCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE08_GET, ViridianCity_Gym_EventScript_GymStatuePostVictory
	msgbox ViridianCity_Gym_Text_GymStatue
	releaseall
	end

ViridianCity_Gym_EventScript_GymStatuePostVictory::
	msgbox ViridianCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

ViridianCity_Gym_Text_GiovanniIntro::
	.string "Fwahahaha!\n"
	.string "Welcome to my hideout!\p"
	.string "It shall be so until I can restore\n"
	.string "TEAM ROCKET to its former glory.\p"
	.string "But, you have found me again.\n"
	.string "So be it.\l"
	.string "This time, I'm not holding back!\p"
	.string "Once more, you shall face\n"
	.string "GIOVANNI, the greatest TRAINER!{PLAY_BGM}{MUS_RG_ENCOUNTER_ROCKET}$"

ViridianCity_Gym_Text_GiovanniDefeat::
	.string "Ha!\n"
	.string "That was a truly intense fight.\l"
	.string "You have won!\p"
	.string "Take my place - the KINGPIN RANK!\n"
	.string "{PAUSE_MUSIC}{PLAY_BGM}{MUS_OBTAIN_BADGE}{PAUSE 0xFE}{PAUSE 0x56}{RESUME_MUSIC}$"

ViridianCity_Gym_Text_GiovanniPostBattle::
	.string "Having lost in this fashion, \n"
	.string "I can't face my followers.\l"
	.string "I have betrayed their trust.\p"
	.string "As of today, TEAM ROCKET is\n"
	.string "finished forever!\p"
	.string "As for myself, I shall dedicate\n"
	.string "my life to training again.\p"
	.string "Let us meet again someday!\n"
	.string "Farewell!$"

ViridianCity_Gym_Text_ExplainEarthBadgeTakeThis::
	.string "The KINGPIN RANK makes POKéMON of\n"
	.string "any level obey without question.\p"
	.string "It is evidence of your mastery as\n"
	.string "a POKéMON TRAINER.\p"
	.string "With it, you can challenge the\n"
	.string "POKéMON LEAGUE.\p"
	.string "Also, take this TM.\p"
	.string "Consider it a gift for your POKéMON\n"
	.string "LEAGUE challenge.$"

ViridianCity_Gym_Text_ReceivedTM26FromGiovanni::
	.string "{PLAYER} received TM26\n"
	.string "from GIOVANNI.$"

ViridianCity_Gym_Text_ExplainTM26::
	.string "TM26 contains EARTHQUAKE.\p"
	.string "It is a powerful attack that causes\n"
	.string "a massive tremor.\p"
	.string "I made it when I ran the GYM here,\n"
	.string "far too long ago… $"

ViridianCity_Gym_Text_YouDoNotHaveSpace::
	.string "You do not have space for this!$"

ViridianCity_Gym_Text_YujiIntro::
	.string "Heh!\n"
	.string "You must be running out of steam\l"
	.string "by now!$"

ViridianCity_Gym_Text_YujiDefeat::
	.string "I ran out of gas!$"

ViridianCity_Gym_Text_YujiPostBattle::
	.string "You'll need power to keep up with\n"
	.string "our GYM LEADER.$"

ViridianCity_Gym_Text_AtsushiIntro::
	.string "Rrrroar!\n"
	.string "I'm working myself into a rage!$"

ViridianCity_Gym_Text_AtsushiDefeat::
	.string "Wargh!$"

ViridianCity_Gym_Text_AtsushiPostBattle::
	.string "I'm still not worthy!$"

ViridianCity_Gym_Text_JasonIntro::
	.string "POKéMON and I, we make wonderful\n"
	.string "music together!$"

ViridianCity_Gym_Text_JasonDefeat::
	.string "You are in perfect harmony!$"

ViridianCity_Gym_Text_JasonPostBattle::
	.string "Do you know the identity of our\n"
	.string "GYM LEADER?$"

ViridianCity_Gym_Text_KiyoIntro::
	.string "Karate is the ultimate form of\n"
	.string "martial arts!$"

ViridianCity_Gym_Text_KiyoDefeat::
	.string "Aiyah!$"

ViridianCity_Gym_Text_KiyoPostBattle::
	.string "If my POKéMON were as good at\n"
	.string "karate as I…$"

ViridianCity_Gym_Text_WarrenIntro::
	.string "The truly talented win with style.$"

ViridianCity_Gym_Text_WarrenDefeat::
	.string "I lost my grip!$"

ViridianCity_Gym_Text_WarrenPostBattle::
	.string "The LEADER will scold me for\n"
	.string "losing this way…$"

ViridianCity_Gym_Text_TakashiIntro::
	.string "I'm the KARATE KING!\n"
	.string "Your fate rests with me!$"

ViridianCity_Gym_Text_TakashiDefeat::
	.string "Ayah!$"

ViridianCity_Gym_Text_TakashiPostBattle::
	.string "The POKéMON LEAGUE?\n"
	.string "You? Don't get cocky!$"

ViridianCity_Gym_Text_ColeIntro::
	.string "Your POKéMON will cower at the\n"
	.string "crack of my whip!$"

ViridianCity_Gym_Text_ColeDefeat::
	.string "Yowch!\n"
	.string "Whiplash!$"

ViridianCity_Gym_Text_ColePostBattle::
	.string "Wait!\n"
	.string "I was just careless!$"

ViridianCity_Gym_Text_SamuelIntro::
	.string "VIRIDIAN GYM was closed for a\n"
	.string "long time.\p"
	.string "But now, our LEADER is back!$"

ViridianCity_Gym_Text_SamuelDefeat::
	.string "I was beaten?$"

ViridianCity_Gym_Text_SamuelPostBattle::
	.string "You can go on to the POKéMON\n"
	.string "LEAGUE only by defeating our GYM\l"
	.string "LEADER!$"

ViridianCity_Gym_Text_GymGuyAdvice::
	.string "Yo!\n"
	.string "Champ in the making!\p"
	.string "Even I don't know the VIRIDIAN\n"
	.string "LEADER's identity.\p"
	.string "But one thing's certain.\n"
	.string "This will be the toughest of all\l"
	.string "the GYM LEADERS.\p"
	.string "Also, I heard that the TRAINERS\n"
	.string "here like GROUND-type POKéMON.$"

ViridianCity_Gym_Text_GymGuyPostVictory::
	.string "Blow me away! GIOVANNI was the\n"
	.string "GYM LEADER of VIRIDIAN?$"

ViridianCity_Gym_Text_GymStatue::
	.string "VIRIDIAN POKéMON GYM\n"
	.string "LEADER: ?\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"

ViridianCity_Gym_Text_GymStatuePlayerWon::
	.string "VIRIDIAN POKéMON GYM\n"
	.string "LEADER: GIOVANNI\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"


ViridianCity_Gym_Text_GangIntroWelcome::
	.string "GIOVANNI: So, you're awake.\n"
	.string "Welcome to what's left of my empire.\p"
	.string "TEAM ROCKET fell. The grunts ran,\n"
	.string "the hideouts were raided.\p"
	.string "But this GYM is still mine. From here,\n"
	.string "we rebuild. And you, {PLAYER}, are\l"
	.string "our newest recruit.\p"
	.string "A recruit needs a partner.\n"
	.string "Take this one. Don't disappoint me.$"

ViridianCity_Gym_Text_GangIntroReceivedMon::
	.string "{PLAYER} received\n"
	.string "{STR_VAR_1} from GIOVANNI!$"

ViridianCity_Gym_Text_GangIntroGiovanniSilver::
	.string "GIOVANNI: SILVER. You will train\n"
	.string "alongside the recruit.\p"
	.string "Pick your own partner.\n"
	.string "Choose wisely.$"

ViridianCity_Gym_Text_GangIntroSilverChallenge::
	.string "{RIVAL}: Hmph. I'll take {STR_VAR_2}.\n"
	.string "It's strong against yours.\p"
	.string "Father thinks you're special?\n"
	.string "Let's see about that!$"

ViridianCity_Gym_Text_GangIntroSilverDefeat::
	.string "{RIVAL}: Tch...\n"
	.string "Beginner's luck!$"

ViridianCity_Gym_Text_GangIntroSilverVictory::
	.string "{RIVAL}: Ha! Weak.\n"
	.string "Just like I thought.$"

ViridianCity_Gym_Text_GangIntroSilverLeaves::
	.string "{RIVAL}: Don't get comfortable.\n"
	.string "I'll be the one who takes over\l"
	.string "this family, not some recruit!$"

ViridianCity_Gym_Text_GangIntroGiovanniDex::
	.string "GIOVANNI: My son has fire.\n"
	.string "Good. You'll need it too.\p"
	.string "Take this. A POKéDEX - stolen from\n"
	.string "a certain professor in PALLET TOWN.\p"
	.string "Our people upgraded it. It records\n"
	.string "every POKéMON in the world.$"

ViridianCity_Gym_Text_GangIntroReceivedDex::
	.string "{PLAYER} received the\n"
	.string "NATIONAL POKéDEX from GIOVANNI!$"

ViridianCity_Gym_Text_GangIntroReceivedBalls::
	.string "{PLAYER} received\n"
	.string "five POKé BALLS!$"

ViridianCity_Gym_Text_GangIntroMission::
	.string "GIOVANNI: Your mission is simple.\p"
	.string "Go out into KANTO. Catch POKéMON.\n"
	.string "Beat every GYM LEADER you meet.\p"
	.string "Make the name of our family feared\n"
	.string "again. Beat the GYM LEADERS to climb\l"
	.string "the ranks. Reach UNDERBOSS,\l"
	.string "and I'll test you myself.\p"
	.string "ROUTE 2 lies north of VIRIDIAN.\n"
	.string "Now go.$"

ViridianCity_Gym_Text_GiovanniHQ::
	.string "GIOVANNI: What are you still doing\n"
	.string "here, recruit?\p"
	.string "Come back when you've reached\n"
	.string "the rank of UNDERBOSS. Not a\l"
	.string "moment sooner.$"

ViridianCity_Gym_Text_GangIntroGiovanniCandies::
	.string "GIOVANNI: And something to make\n"
	.string "your POKéMON grow up fast.$"

ViridianCity_Gym_Text_GangIntroReceivedCandies::
	.string "{PLAYER} received\n"
	.string "30 RARE CANDIES!$"

ViridianCity_Gym_Text_GangIntroGiovanniGadgets::
	.string "GIOVANNI: Two gadgets from our labs.\n"
	.string "Don't lose them.$"

ViridianCity_Gym_Text_GangIntroGiovanniGadgets2::
	.string "GIOVANNI: The POKé VIAL heals your\n"
	.string "team, the AUTO-REPEL keeps weak\l"
	.string "POKéMON off your back.\p"
	.string "They hold one charge for now. Every\n"
	.string "rank you climb adds another one.\p"
	.string "The HEAL-BOTs in the old POKéMON\n"
	.string "CENTERS recharge them.$"
