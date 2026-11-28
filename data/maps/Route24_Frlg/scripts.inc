Route24_Frlg_MapScripts::
	.byte 0

Route24_EventScript_Rocket::
	lock
	faceplayer
	goto_if_eq VAR_MAP_SCENE_ROUTE24, 1, Route24_EventScript_RocketPostBattle
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNugget
	call Route24_EventScript_BattleRocket
	release
	end

Route24_EventScript_NoRoomForNugget::
	msgbox Route24_Text_YouDontHaveAnyRoom
	release
	end

Route24_EventScript_RocketPostBattle::
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	release
	end

Route24_EventScript_RocketTriggerLeft::
	lockall
	setvar VAR_TEMP_1, 0
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTriggerRight::
	lockall
	setvar VAR_TEMP_1, 1
	goto Route24_EventScript_RocketTrigger
	end

Route24_EventScript_RocketTrigger::
	textcolor NPC_TEXT_COLOR_MALE
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketApproachPlayer
	call_if_eq VAR_TEMP_1, 1, Route24_EventScript_RocketMotionToPlayer
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox Route24_Text_JustEarnedFabulousPrize
	checkitemspace ITEM_NUGGET
	goto_if_eq VAR_RESULT, FALSE, Route24_EventScript_NoRoomForNuggetTrigger
	call Route24_EventScript_BattleRocket
	releaseall
	end

Route24_EventScript_BattleRocket::
	giveitem_msg Route24_Text_ReceivedNuggetFromMysteryTrainer, ITEM_NUGGET
	message Route24_Text_JoinTeamRocket
	waitmessage
	playbgm MUS_RG_ENCOUNTER_ROCKET, 0
	waitbuttonpress
	setvar VAR_LAST_TALKED, LOCALID_ROUTE24_ROCKET
	trainerbattle_no_intro TRAINER_TEAM_ROCKET_GRUNT_6, Route24_Text_RocketDefeat
	msgbox Route24_Text_YoudBecomeTopRocketLeader
	setvar VAR_MAP_SCENE_ROUTE24, 1
	return

Route24_EventScript_RocketApproachPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketApproachPlayer
	waitmovement 0
	return

Route24_EventScript_RocketMotionToPlayer::
	applymovement LOCALID_ROUTE24_ROCKET, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	return

Route24_EventScript_NoRoomForNuggetTrigger::
	msgbox Route24_Text_YouDontHaveAnyRoom
	closemessage
	applymovement LOCALID_PLAYER, Route24_Movement_WalkDown
	waitmovement 0
	call_if_eq VAR_TEMP_1, 0, Route24_EventScript_RocketWalkBackToPos
	release
	end

Route24_EventScript_RocketWalkBackToPos::
	applymovement LOCALID_ROUTE24_ROCKET, Route24_Movement_RocketWalkBackToPos
	waitmovement 0
	return

Route24_Movement_RocketApproachPlayer::
	walk_left
	step_end

Route24_Movement_RocketWalkBackToPos::
	walk_right
	walk_in_place_faster_left
	step_end

Route24_Movement_WalkDown::
	walk_down
	step_end

Route24_Text_JustEarnedFabulousPrize::
	.string "Congratulations! You beat TEAM\n"
	.string "SKULL's five bridge thugs!\p"
	.string "That earns you a fabulous prize!\n"
	.string "Or a target on your back!$"

Route24_Text_ReceivedNuggetFromMysteryTrainer::
	.string "{PLAYER} received a NUGGET\n"
	.string "from the mystery TRAINER!$"

Route24_Text_YouDontHaveAnyRoom::
	.string "You don't have any room!$"

Route24_Text_JoinTeamRocket::
	.string "By the way, how would you like to\n"
	.string "join TEAM ROCKET?\p"
	.string "Oh wait, you're already in\n"
	.string "GIOVANNI's crew.\p"
	.string "Then how about switching to\n"
	.string "ARCHER's side?\p"
	.string "Are you sure?\p"
	.string "Fine. Then I'll take you down!$"

Route24_Text_RocketDefeat::
	.string "Arrgh!\n"
	.string "You are good!$"

Route24_Text_YoudBecomeTopRocketLeader::
	.string "With your ability, you'd be a top\n"
	.string "boss anywhere.\p"
	.string "Come on, think about it!$"

Route24_Text_ShaneIntro::
	.string "Mom says stay inside.\p"
	.string "But battles are way more fun!$"

Route24_Text_ShaneDefeat::
	.string "Waaah! I'm telling my brother!$"

Route24_Text_ShanePostBattle::
	.string "I hide when the gangs come by.\p"
	.string "Not from you, though!$"

Route24_Text_EthanIntro::
	.string "I'm the toughest kid on this road.\n"
	.string "Ask anyone!$"

Route24_Text_EthanDefeat::
	.string "No fair! You're way too strong!$"

Route24_Text_EthanPostBattle::
	.string "The road to CERULEAN isn't safe.\p"
	.string "Watch out!$"

Route24_Text_ReliIntro::
	.string "My big brother rides with TEAM\n"
	.string "SKULL! I'm gonna be just like him!$"

Route24_Text_ReliDefeat::
	.string "Aww… There goes my lunch money.$"

Route24_Text_ReliPostBattle::
	.string "When I grow up, I'm joining TEAM\n"
	.string "SKULL.\p"
	.string "Or maybe your crew!$"

Route24_Text_TimmyIntro::
	.string "I found a loot bag yesterday!\p"
	.string "Wanna fight for it?$"

Route24_Text_TimmyDefeat::
	.string "Waaah! I'm telling my brother!$"

Route24_Text_TimmyPostBattle::
	.string "Don't tell my mom I battled you.$"

Route24_Text_AliIntro::
	.string "GUZMA says kids can't fight. I'll\n"
	.string "prove him wrong!$"

Route24_Text_AliDefeat::
	.string "Okay, okay! You win!$"

Route24_Text_AliPostBattle::
	.string "Everybody's scared of GUZMA.\p"
	.string "Are you scared?$"

Route24_Text_CaleIntro::
	.string "My POKéMON is the scariest on the\n"
	.string "block!$"

Route24_Text_CaleDefeat::
	.string "No fair! You're way too strong!$"

Route24_Text_CalePostBattle::
	.string "Can I join your crew when I'm\n"
	.string "older?$"
