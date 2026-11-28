ViridianCity_School_Frlg_MapScripts::
	.byte 0

ViridianCity_School_EventScript_Lass::
	lock
	faceplayer
	msgbox ViridianCity_School_Text_TryingToMemorizeNotes
	closemessage
	applymovement LOCALID_SCHOOL_LASS, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	release
	end

ViridianCity_School_EventScript_Woman::
	lock
	faceplayer
	msgbox ViridianCity_School_Text_ReadBlackboardCarefully
	closemessage
	applymovement LOCALID_SCHOOL_WOMAN, Common_Movement_FaceOriginalDirection
	waitmovement 0
	release
	end

ViridianCity_School_EventScript_Notebook::
	lockall
	msgbox ViridianCity_School_Text_NotebookFirstPage
	msgbox ViridianCity_School_Text_TurnThePage, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_School_EventScript_StopReadingNotebook
	msgbox ViridianCity_School_Text_NotebookSecondPage
	msgbox ViridianCity_School_Text_TurnThePage, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_School_EventScript_StopReadingNotebook
	msgbox ViridianCity_School_Text_NotebookThirdPage
	msgbox ViridianCity_School_Text_TurnThePage, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ViridianCity_School_EventScript_StopReadingNotebook
	msgbox ViridianCity_School_Text_NotebookFourthPage
	applymovement LOCALID_SCHOOL_LASS, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox ViridianCity_School_Text_HeyDontLookAtMyNotes
	releaseall
	end

ViridianCity_School_EventScript_StopReadingNotebook::
	releaseall
	end

ViridianCity_School_EventScript_Blackboard::
	lockall
	msgbox ViridianCity_School_Text_BlackboardListsStatusProblems
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ChooseBlackboardTopic::
	message ViridianCity_School_Text_ReadWhichTopic
	waitmessage
	multichoicegrid 7, 1, MULTI_STATUS_INFO, 3, FALSE
	switch VAR_RESULT
	case 0, ViridianCity_School_EventScript_ReadSleep
	case 1, ViridianCity_School_EventScript_ReadPoison
	case 2, ViridianCity_School_EventScript_ReadParalysis
	case 3, ViridianCity_School_EventScript_ReadBurn
	case 4, ViridianCity_School_EventScript_ReadFreeze
	case 5, ViridianCity_School_EventScript_ExitBlackboard
	case 127, ViridianCity_School_EventScript_ExitBlackboard
	end

ViridianCity_School_EventScript_ReadSleep::
	msgbox ViridianCity_School_Text_ExplainSleep
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadBurn::
	msgbox ViridianCity_School_Text_ExplainBurn
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadPoison::
	msgbox ViridianCity_School_Text_ExplainPoison
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadFreeze::
	msgbox ViridianCity_School_Text_ExplainFreeze
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ReadParalysis::
	msgbox ViridianCity_School_Text_ExplainParalysis
	goto ViridianCity_School_EventScript_ChooseBlackboardTopic
	end

ViridianCity_School_EventScript_ExitBlackboard::
	releaseall
	end

ViridianCity_School_Text_TryingToMemorizeNotes::
	.string "The school is closed, but I\n"
	.string "still study here.\p"
	.string "Somebody has to know the rules\n"
	.string "when this is over.$"

ViridianCity_School_Text_ReadBlackboardCarefully::
	.string "Okay!\p"
	.string "Read the blackboard carefully. It\n"
	.string "might save your life out there!$"

ViridianCity_School_Text_NotebookFirstPage::
	.string "Let's check out the notebook.\p"
	.string "First page…\p"
	.string "POKé BALLS are used to catch\n"
	.string "POKéMON.\p"
	.string "Up to six POKéMON can be carried\n"
	.string "in your party.\p"
	.string "People who raise and battle\n"
	.string "POKéMON are called TRAINERS. Or\p"
	.string "gangsters.$"

ViridianCity_School_Text_NotebookSecondPage::
	.string "Second page…\p"
	.string "A healthy POKéMON is hard to\n"
	.string "catch, so weaken it first.\p"
	.string "Poison, burn, or another status\n"
	.string "problem helps.$"

ViridianCity_School_Text_NotebookThirdPage::
	.string "Third page…\p"
	.string "TRAINERS seek out battles. Gangs\n"
	.string "seek out turf.\p"
	.string "Every GYM in KANTO is run by a gang\n"
	.string "boss now.$"

ViridianCity_School_Text_NotebookFourthPage::
	.string "Fourth page…\p"
	.string "To rise in GIOVANNI's ranks, you\n"
	.string "must beat the eight gang bosses.\p"
	.string "Do so to earn the right to face…\p"
	.string "The ELITE FOUR and the CHAMPION!$"

ViridianCity_School_Text_TurnThePage::
	.string "Turn the page?$"

ViridianCity_School_Text_HeyDontLookAtMyNotes::
	.string "GIRL: Hey!\n"
	.string "Don't look at my notes!$"

ViridianCity_School_Text_BlackboardListsStatusProblems::
	.string "The blackboard lists STATUS\n"
	.string "problems. Gang fights cause\p"
	.string "plenty of them.$"

ViridianCity_School_Text_ReadWhichTopic::
	.string "Which topic do you want to read?$"

ViridianCity_School_Text_ExplainSleep::
	.string "A sleeping POKéMON can't attack.\p"
	.string "It stays asleep after battles,\n"
	.string "too. Use AWAKENING to wake it.$"

ViridianCity_School_Text_ExplainBurn::
	.string "A burn lowers ATTACK and drains HP\n"
	.string "every turn.\p"
	.string "It lasts after battles. Use BURN\n"
	.string "HEAL.$"

ViridianCity_School_Text_ExplainPoison::
	.string "Poison drains HP, even after\n"
	.string "battles.\p"
	.string "The gangs love poison. Carry\n"
	.string "ANTIDOTES!$"

ViridianCity_School_Text_ExplainFreeze::
	.string "A frozen POKéMON can't move at\n"
	.string "all.\p"
	.string "It stays frozen after battle. Use\n"
	.string "ICE HEAL to thaw it.$"

ViridianCity_School_Text_ExplainParalysis::
	.string "Paralysis lowers SPEED and may\n"
	.string "stop a POKéMON from moving.\p"
	.string "It lasts after battles. Use\n"
	.string "PARLYZ HEAL.$"

