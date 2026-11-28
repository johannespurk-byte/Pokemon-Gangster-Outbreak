SSAnne_1F_Room6_Frlg_MapScripts::
	.byte 0

SSAnne_1F_Room6_EventScript_Woman::
	lock
	faceplayer
	msgbox SSAnne_1F_Room6_Text_TakeAShortRest, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, SSAnne_1F_Room6_EventScript_DeclineHeal
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox SSAnne_1F_Room6_Text_GladEveryoneIsRefreshed
	release
	end

SSAnne_1F_Room6_EventScript_DeclineHeal::
	checkplayergender
	goto_if_eq VAR_RESULT, MALE, SSAnne_1F_Room6_EventScript_DeclineHealMale
	msgbox SSAnne_1F_Room6_Text_SorryYouLookLikeMySister
	release
	end

SSAnne_1F_Room6_EventScript_DeclineHealMale::
	msgbox SSAnne_1F_Room6_Text_SorryYouLookLikeMyBrother
	release
	end

SSAnne_1F_Room6_Text_TakeAShortRest::
	.string "You look tired. Rest here. The\n"
	.string "ROCKETS don't check this room.$"

SSAnne_1F_Room6_Text_GladEveryoneIsRefreshed::
	.string "I'm glad everyone looks\n"
	.string "refreshed. Now be careful out\p"
	.string "there.$"

SSAnne_1F_Room6_Text_SorryYouLookLikeMyBrother::
	.string "Sorry for fussing. You look like my\n"
	.string "little brother. He joined a gang…$"

SSAnne_1F_Room6_Text_SorryYouLookLikeMySister::
	.string "Sorry for fussing. You look like my\n"
	.string "little sister. She joined a gang…$"

