SaffronCity_Gym_Frlg_MapScripts::
	.byte 0

SaffronCity_Gym_EventScript_Sabrina::
	famechecker FAMECHECKER_SABRINA, FCPICKSTATE_COLORED, UpdatePickStateFromSpecialVar8005
	trainerbattle_single TRAINER_LEADER_SABRINA, SaffronCity_Gym_Text_SabrinaIntro, SaffronCity_Gym_Text_SabrinaDefeat, SaffronCity_Gym_EventScript_DefeatedSabrina, NO_MUSIC
	goto_if_unset FLAG_GOT_TM04_FROM_SABRINA, SaffronCity_Gym_EventScript_GiveTM04
	call EventScript_GangGiveTeraOrb
	msgbox SaffronCity_Gym_Text_ExplainTM04
	release
	end

SaffronCity_Gym_EventScript_DefeatedSabrina::
	famechecker FAMECHECKER_SABRINA, 2
	famechecker FAMECHECKER_SABRINA, 3
	clearflag FLAG_HIDE_SAFFRON_CITY_POKECENTER_SABRINA_JOURNALS
	setflag FLAG_DEFEATED_SABRINA
	setflag FLAG_BADGE06_GET
	set_gym_trainers_frlg 6
	goto SaffronCity_Gym_EventScript_GiveTM04
	end

SaffronCity_Gym_EventScript_GiveTM04::
	msgbox SaffronCity_Gym_Text_ExplainMarshBadgeTakeThis
	checkitemspace ITEM_TM04
	goto_if_eq VAR_RESULT, FALSE, SaffronCity_Gym_EventScript_NoRoomForTM04
	giveitem_msg SaffronCity_Gym_Text_ReceivedTM04FromSabrina, ITEM_TM04
	setflag FLAG_GOT_TM04_FROM_SABRINA
	call EventScript_GangGiveTeraOrb
	msgbox SaffronCity_Gym_Text_SabrinaPostBattle
	release
	end

SaffronCity_Gym_EventScript_NoRoomForTM04::
	msgbox SaffronCity_Gym_Text_BagFullOfOtherItems
	release
	end

SaffronCity_Gym_EventScript_Johan::
	trainerbattle_single TRAINER_PSYCHIC_JOHAN, SaffronCity_Gym_Text_JohanIntro, SaffronCity_Gym_Text_JohanDefeat
	msgbox SaffronCity_Gym_Text_JohanPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Tyron::
	trainerbattle_single TRAINER_PSYCHIC_TYRON, SaffronCity_Gym_Text_TyronIntro, SaffronCity_Gym_Text_TyronDefeat
	famechecker FAMECHECKER_SABRINA, 5
	msgbox SaffronCity_Gym_Text_TyronPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Cameron::
	trainerbattle_single TRAINER_PSYCHIC_CAMERON, SaffronCity_Gym_Text_CameronIntro, SaffronCity_Gym_Text_CameronDefeat
	msgbox SaffronCity_Gym_Text_CameronPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Preston::
	trainerbattle_single TRAINER_PSYCHIC_PRESTON, SaffronCity_Gym_Text_PrestonIntro, SaffronCity_Gym_Text_PrestonDefeat
	msgbox SaffronCity_Gym_Text_PrestonPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Amanda::
	trainerbattle_single TRAINER_CHANNELER_AMANDA, SaffronCity_Gym_Text_AmandaIntro, SaffronCity_Gym_Text_AmandaDefeat
	msgbox SaffronCity_Gym_Text_AmandaPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Stacy::
	trainerbattle_single TRAINER_CHANNELER_STACY, SaffronCity_Gym_Text_StacyIntro, SaffronCity_Gym_Text_StacyDefeat
	msgbox SaffronCity_Gym_Text_StacyPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_Tasha::
	trainerbattle_single TRAINER_CHANNELER_TASHA, SaffronCity_Gym_Text_TashaIntro, SaffronCity_Gym_Text_TashaDefeat
	msgbox SaffronCity_Gym_Text_TashaPostBattle, MSGBOX_AUTOCLOSE
	end

SaffronCity_Gym_EventScript_GymGuy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_SABRINA, SaffronCity_Gym_EventScript_GymGuyPostVictory
	msgbox SaffronCity_Gym_Text_GymGuyAdvice
	release
	end

SaffronCity_Gym_EventScript_GymGuyPostVictory::
	msgbox SaffronCity_Gym_Text_GymGuyPostVictory
	release
	end

SaffronCity_Gym_EventScript_GymStatue::
	lockall
	goto_if_set FLAG_BADGE06_GET, SaffronCity_Gym_EventScript_GymStatuePostVictory
	msgbox SaffronCity_Gym_Text_GymStatue
	releaseall
	end

SaffronCity_Gym_EventScript_GymStatuePostVictory::
	msgbox SaffronCity_Gym_Text_GymStatuePlayerWon
	releaseall
	end

SaffronCity_Gym_Text_SabrinaIntro::
	.string "I am LYSANDRE of TEAM FLARE.\p"
	.string "This world is ugly. Only the chosen\n"
	.string "deserve to see its beautiful end.\p"
	.string "Prove that you are one of them!{PLAY_BGM}{MUS_RG_ENCOUNTER_GYM_LEADER}$"

SaffronCity_Gym_Text_SabrinaDefeat::
	.string "LYSANDRE: This loss… is beautiful\n"
	.string "in its own way.\p"
	.string "You have earned the CAPO RANK.$"

SaffronCity_Gym_Text_SabrinaPostBattle::
	.string "You are beautiful in your own way.\n"
	.string "TEAM FLARE salutes you.$"

SaffronCity_Gym_Text_ExplainMarshBadgeTakeThis::
	.string "The CAPO RANK raises your level\n"
	.string "cap to Lv. 54.\p"
	.string "With the HM in your BAG, your first\n"
	.string "POKéMON can use ROCK SMASH\p"
	.string "outside of battle.\p"
	.string "Please take this TM as well.$"

SaffronCity_Gym_Text_ReceivedTM04FromSabrina::
	.string "{PLAYER} received TM04 from\n"
	.string "LYSANDRE.$"

SaffronCity_Gym_Text_ExplainTM04::
	.string "TM04 is CALM MIND.\p"
	.string "It makes the POKéMON concentrate\n"
	.string "to raise both SP. ATK and SP. DEF.$"

SaffronCity_Gym_Text_BagFullOfOtherItems::
	.string "Your BAG is full of other items.$"

SaffronCity_Gym_Text_AmandaIntro::
	.string "Another challenger? LYSANDRE will\n"
	.string "love this.$"

SaffronCity_Gym_Text_AmandaDefeat::
	.string "Tch… You're the real thing.$"

SaffronCity_Gym_Text_AmandaPostBattle::
	.string "Every gang in KANTO is watching\n"
	.string "this GYM.$"

SaffronCity_Gym_Text_JohanIntro::
	.string "Nobody reaches LYSANDRE without\n"
	.string "going through me!$"

SaffronCity_Gym_Text_JohanDefeat::
	.string "Tch… You're the real thing.$"

SaffronCity_Gym_Text_JohanPostBattle::
	.string "LYSANDRE is waiting. Don't keep\n"
	.string "the boss waiting.$"

SaffronCity_Gym_Text_StacyIntro::
	.string "Welcome to our turf. Hope you\n"
	.string "brought REVIVES!$"

SaffronCity_Gym_Text_StacyDefeat::
	.string "No way! LYSANDRE is gonna be mad!$"

SaffronCity_Gym_Text_StacyPostBattle::
	.string "If you win, you're one step closer\n"
	.string "to the LEAGUE.$"

SaffronCity_Gym_Text_TyronIntro::
	.string "This is the GYM of TEAM FLARE.\n"
	.string "Turn back while you can!$"

SaffronCity_Gym_Text_TyronDefeat::
	.string "No way! LYSANDRE is gonna be mad!$"

SaffronCity_Gym_Text_TyronPostBattle::
	.string "LYSANDRE just wiped out the KARATE\n"
	.string "MASTER next door.$"

SaffronCity_Gym_Text_TashaIntro::
	.string "Nobody reaches LYSANDRE without\n"
	.string "going through me!$"

SaffronCity_Gym_Text_TashaDefeat::
	.string "I lost?! In my own GYM?!$"

SaffronCity_Gym_Text_TashaPostBattle::
	.string "LYSANDRE is waiting. Don't keep\n"
	.string "the boss waiting.$"

SaffronCity_Gym_Text_CameronIntro::
	.string "You want a RANK? Earn it the hard\n"
	.string "way!$"

SaffronCity_Gym_Text_CameronDefeat::
	.string "I lost?! In my own GYM?!$"

SaffronCity_Gym_Text_CameronPostBattle::
	.string "There used to be two POKéMON GYMS\n"
	.string "in SAFFRON.\p"
	.string "The FIGHTING DOJO next door lost\n"
	.string "its GYM status, though.\p"
	.string "We simply creamed them when we\n"
	.string "decided to have just one GYM.$"

SaffronCity_Gym_Text_PrestonIntro::
	.string "LYSANDRE doesn't waste time on\n"
	.string "rookies. Neither do I!$"

SaffronCity_Gym_Text_PrestonDefeat::
	.string "Fine… go on ahead.$"

SaffronCity_Gym_Text_PrestonPostBattle::
	.string "Go on. Show LYSANDRE what you've\n"
	.string "got.$"

SaffronCity_Gym_Text_GymGuyAdvice::
	.string "Yo! Boss in the making!\p"
	.string "LYSANDRE of TEAM FLARE uses FIRE,\n"
	.string "DARK and FLYING POKéMON.\p"
	.string "WATER, ROCK and ELECTRIC moves\n"
	.string "will burn him out!$"

SaffronCity_Gym_Text_GymGuyPostVictory::
	.string "You beat LYSANDRE! TEAM FLARE's\n"
	.string "fashion show is over!$"

SaffronCity_Gym_Text_GymStatue::
	.string "SAFFRON POKéMON GYM\n"
	.string "LEADER: LYSANDRE\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}$"

SaffronCity_Gym_Text_GymStatuePlayerWon::
	.string "SAFFRON POKéMON GYM\n"
	.string "LEADER: LYSANDRE\p"
	.string "WINNING TRAINERS:\n"
	.string "{RIVAL}, {PLAYER}$"

