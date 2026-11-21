@ Gangster Outbreak: Silver hands over the HMs one by one after his battles

EventScript_GangRivalGivesHM01::
	checkitem ITEM_HM01
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangRivalHMDone
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GangRivalHM01
	giveitem_msg Text_GangReceivedHM01, ITEM_HM01, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

Text_GangRivalHM01:
	.string "{RIVAL}: Tch. Father wants you to\n"
	.string "have this. HM01 - CUT.\p"
	.string "Trees are no problem for us.\n"
	.string "Don't make me regret it.$"

Text_GangReceivedHM01:
	.string "{PLAYER} received HM01\n"
	.string "from {RIVAL}!$"

EventScript_GangRivalGivesHM02::
	checkitem ITEM_HM02
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangRivalHMDone
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GangRivalHM02
	giveitem_msg Text_GangReceivedHM02, ITEM_HM02, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

Text_GangRivalHM02:
	.string "{RIVAL}: Tch. Father wants you to\n"
	.string "have this. HM02 - FLY.\p"
	.string "Fly wherever you need to go.\n"
	.string "Don't make me regret it.$"

Text_GangReceivedHM02:
	.string "{PLAYER} received HM02\n"
	.string "from {RIVAL}!$"

EventScript_GangRivalGivesHM03::
	checkitem ITEM_HM03
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangRivalHMDone
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GangRivalHM03
	giveitem_msg Text_GangReceivedHM03, ITEM_HM03, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

Text_GangRivalHM03:
	.string "{RIVAL}: Tch. Father wants you to\n"
	.string "have this. HM03 - SURF.\p"
	.string "The sea belongs to us too.\n"
	.string "Don't make me regret it.$"

Text_GangReceivedHM03:
	.string "{PLAYER} received HM03\n"
	.string "from {RIVAL}!$"

EventScript_GangRivalGivesHM04::
	checkitem ITEM_HM04
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangRivalHMDone
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GangRivalHM04
	giveitem_msg Text_GangReceivedHM04, ITEM_HM04, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

Text_GangRivalHM04:
	.string "{RIVAL}: Tch. Father wants you to\n"
	.string "have this. HM04 - STRENGTH.\p"
	.string "Push anything out of your way.\n"
	.string "Don't make me regret it.$"

Text_GangReceivedHM04:
	.string "{PLAYER} received HM04\n"
	.string "from {RIVAL}!$"

EventScript_GangRivalGivesHM05::
	checkitem ITEM_HM05
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangRivalHMDone
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GangRivalHM05
	giveitem_msg Text_GangReceivedHM05, ITEM_HM05, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

Text_GangRivalHM05:
	.string "{RIVAL}: Tch. Father wants you to\n"
	.string "have this. HM05 - FLASH.\p"
	.string "Light up the dark tunnels.\n"
	.string "Don't make me regret it.$"

Text_GangReceivedHM05:
	.string "{PLAYER} received HM05\n"
	.string "from {RIVAL}!$"

EventScript_GangRivalGivesHM06::
	checkitem ITEM_HM06
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangRivalHMDone
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GangRivalHM06
	giveitem_msg Text_GangReceivedHM06, ITEM_HM06, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

Text_GangRivalHM06:
	.string "{RIVAL}: Tch. Father wants you to\n"
	.string "have this. HM06 - ROCK SMASH.\p"
	.string "Smash the rocks in your way.\n"
	.string "Don't make me regret it.$"

Text_GangReceivedHM06:
	.string "{PLAYER} received HM06\n"
	.string "from {RIVAL}!$"

EventScript_GangRivalGivesHM07::
	checkitem ITEM_HM07
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangRivalHMDone
	textcolor NPC_TEXT_COLOR_MALE
	msgbox Text_GangRivalHM07
	giveitem_msg Text_GangReceivedHM07, ITEM_HM07, 1, MUS_RG_OBTAIN_KEY_ITEM
	return

Text_GangRivalHM07:
	.string "{RIVAL}: Tch. Father wants you to\n"
	.string "have this. HM07 - WATERFALL.\p"
	.string "Climb up any waterfall.\n"
	.string "Don't make me regret it.$"

Text_GangReceivedHM07:
	.string "{PLAYER} received HM07\n"
	.string "from {RIVAL}!$"

EventScript_GangRivalHMDone::
	return
