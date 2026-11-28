.set REWARD_TYPE, VAR_0x8004
.set REWARD_ITEM, VAR_0x8008

CeruleanCity_House4_Frlg_MapScripts::
	.byte 0

CeruleanCity_House4_EventScript_WonderNewsBerryMan::
	lock
	specialvar REWARD_TYPE, WonderNews_GetRewardInfo
	copyvar REWARD_ITEM, VAR_RESULT
	goto_if_eq REWARD_TYPE, NEWS_REWARD_NONE,       CeruleanCity_House4_EventScript_NoNews
	goto_if_eq REWARD_TYPE, NEWS_REWARD_RECV_SMALL, CeruleanCity_House4_EventScript_Reward_RecvSmall
	goto_if_eq REWARD_TYPE, NEWS_REWARD_RECV_BIG,   CeruleanCity_House4_EventScript_Reward_RecvBig
	goto_if_eq REWARD_TYPE, NEWS_REWARD_WAITING,    CeruleanCity_House4_EventScript_Waiting
	goto_if_eq REWARD_TYPE, NEWS_REWARD_SENT_SMALL, CeruleanCity_House4_EventScript_Reward_SentSmall
	goto_if_eq REWARD_TYPE, NEWS_REWARD_SENT_BIG,   CeruleanCity_House4_EventScript_Reward_SentBig
	goto_if_eq REWARD_TYPE, NEWS_REWARD_AT_MAX,     CeruleanCity_House4_EventScript_AtMax
	end

@ Mystery Gift is not enabled, or the player has no saved Wonder News
CeruleanCity_House4_EventScript_NoNews::
	msgbox CeruleanCity_House4_Text_NothingEntertaining
	release
	end

@ Small reward for receiving Wonder News from friend
CeruleanCity_House4_EventScript_Reward_RecvSmall::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_NewNewsInformativeHaveThis
	giveitem REWARD_ITEM
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Big reward for receiving Wonder News from non-friend source
CeruleanCity_House4_EventScript_Reward_RecvBig::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_IncredibleNewsHaveBerries
	giveitem REWARD_ITEM, 4
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Player has not recently sent Wonder News
CeruleanCity_House4_EventScript_Waiting::
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox CeruleanCity_House4_Text_WishCouldShareNewsWithOthers
	release
	end

@ Small reward for sending Wonder News every 1-3 times
CeruleanCity_House4_EventScript_Reward_SentSmall::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_ThanksForSpreadingNewsTakeThis
	giveitem REWARD_ITEM
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Big reward for sending Wonder News every 4th time
CeruleanCity_House4_EventScript_Reward_SentBig::
	call CeruleanCity_House4_EventScript_MovementReactionToNews
	msgbox CeruleanCity_House4_Text_MagnificentNewsSpreadHaveBerries
	giveitem REWARD_ITEM, 4
	goto_if_eq VAR_RESULT, FALSE, CeruleanCity_House4_EventScript_NoRoomForBerries
	release
	end

@ Player has hit reward limit and must wait to receive new rewards
CeruleanCity_House4_EventScript_AtMax::
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_FacePlayer
	waitmovement 0
	msgbox CeruleanCity_House4_Text_EnjoyingMyselfWithAllSortsOfNews
	release
	end

CeruleanCity_House4_EventScript_MovementReactionToNews::
	playse SE_PIN
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_FacePlayer
	waitmovement 0
	applymovement LOCALID_WONDER_NEWS_BERRY_MAN, Common_Movement_Delay48
	waitmovement 0
	return

CeruleanCity_House4_EventScript_NoRoomForBerries::
	msgbox CeruleanCity_House4_Text_YourBerryPouchIsFull
	release
	end

CeruleanCity_House4_Text_NothingEntertaining::
	.string "Sigh…\n"
	.string "Since TEAM SKULL came, nobody\p"
	.string "goes out anymore. Is there any\n"
	.string "news from outside?$"

CeruleanCity_House4_Text_NewNewsInformativeHaveThis::
	.string "Ah! A new news report!\p"
	.string "Hmm…\p"
	.string "I see! Very informative!\p"
	.string "Kids your age usually only care\n"
	.string "about gang gossip.\p"
	.string "As thanks for the news, I want you\n"
	.string "to have this.$"

CeruleanCity_House4_Text_IncredibleNewsHaveBerries::
	.string "Oh, oh! Th-this news report!\p"
	.string "Hmm…\p"
	.string "Amazing! Incredible!\p"
	.string "I haven't heard news this good\n"
	.string "since before the gangs came!\p"
	.string "Thanks for sharing it. Have a\n"
	.string "bunch of BERRIES!$"

CeruleanCity_House4_Text_WishCouldShareNewsWithOthers::
	.string "It's a shame to keep this news to\n"
	.string "myself…\p"
	.string "But if I go out, TEAM SKULL will\n"
	.string "mug me.$"

CeruleanCity_House4_Text_ThanksForSpreadingNewsTakeThis::
	.string "Ah, you've spread the news for\n"
	.string "me?\p"
	.string "Good news is rare these days. It\n"
	.string "should travel.\p"
	.string "As thanks, take this!$"

CeruleanCity_House4_Text_MagnificentNewsSpreadHaveBerries::
	.string "You've spread the news even\n"
	.string "further? Magnificent!\p"
	.string "The news must be delighted to\n"
	.string "travel so far.\p"
	.string "Yes, indeed!\p"
	.string "Here, extra BERRIES. You've\n"
	.string "earned them!$"

CeruleanCity_House4_Text_EnjoyingMyselfWithAllSortsOfNews::
	.string "I'm enjoying all this news.\p"
	.string "It beats listening to TEAM SKULL\n"
	.string "rap outside my window.$"

CeruleanCity_House4_Text_YourBerryPouchIsFull::
	.string "Hm? Your BERRY POUCH is full.\n"
	.string "My gift remains ungiven, then.$"

