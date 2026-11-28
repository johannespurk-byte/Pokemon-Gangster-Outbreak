VermilionCity_House3_Frlg_MapScripts::
	.byte 0

VermilionCity_House3_EventScript_Boy::
	msgbox VermilionCity_House3_Text_PidgeyFlyLetterToSaffron, MSGBOX_NPC
	end

VermilionCity_House3_EventScript_Lass::
	msgbox VermilionCity_House3_Text_SendMyPidgeyToUnionRoom, MSGBOX_NPC
	end

VermilionCity_House3_EventScript_Pidgey::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_PIDGEY, CRY_MODE_NORMAL
	msgbox VermilionCity_House3_Text_Pidgey
	waitmoncry
	release
	end

VermilionCity_House3_EventScript_Letter::
	msgbox VermilionCity_House3_Text_DearPippiLetter, MSGBOX_SIGN
	end

VermilionCity_House3_Text_PidgeyFlyLetterToSaffron::
	.string "My PIDGEY flies letters past\n"
	.string "the ROCKET checkpoints.\p"
	.string "Faster than any mailman!$"

VermilionCity_House3_Text_Pidgey::
	.string "PIDGEY: Kurukkoo!$"

VermilionCity_House3_Text_DearPippiLetter::
	.string "Dear PIPPI,\n"
	.string "I hope you're safe.\p"
	.string "They say TEAM FLARE owns SAFFRON\n"
	.string "now. VERMILION belongs to ARCHER.\p"
	.string "Stay inside after dark!$"

VermilionCity_House3_Text_SendMyPidgeyToUnionRoom::
	.string "I want to exchange MAIL with all\n"
	.string "sorts of people.\p"
	.string "I send my PIDGEY to a UNION ROOM\n"
	.string "to exchange MAIL for me.$"

