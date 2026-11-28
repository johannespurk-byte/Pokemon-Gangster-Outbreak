SaffronCity_CopycatsHouse_2F_Frlg_MapScripts::
	.byte 0

SaffronCity_CopycatsHouse_2F_EventScript_Doduo::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_DODUO, CRY_MODE_NORMAL
	msgbox SaffronCity_CopycatsHouse_2F_Text_Doduo
	waitmoncry
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_Doll::
	lock
	msgbox SaffronCity_CopycatsHouse_2F_Text_RareMonOnlyDoll
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_Copycat::
	lock
	faceplayer
	checkitem ITEM_POKE_DOLL
	goto_if_eq VAR_RESULT, TRUE, EventScript_MimicTutor
	checkplayergender
	call_if_eq VAR_RESULT, MALE, SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerMale
	call_if_eq VAR_RESULT, FEMALE, SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerFemale
	release
	end

SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerMale::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyMale
	return

SaffronCity_CopycatsHouse_2F_EventScript_MimicPlayerFemale::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyFemale
	return

SaffronCity_CopycatsHouse_2F_EventScript_Computer::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MySecrets, MSGBOX_SIGN
	end

SaffronCity_CopycatsHouse_2F_EventScript_Game::
	msgbox SaffronCity_CopycatsHouse_2F_Text_MarioWearingABucket, MSGBOX_SIGN
	end

SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyMale::
	.string "{PLAYER}: Hi! Do you like POKéMON?\p"
	.string "{PLAYER}: Uh, no, I just asked you.\p"
	.string "{PLAYER}: Huh? You're strange!\p"
	.string "COPYCAT: Hmm? Quit mimicking? But\n"
	.string "I'm practicing to fool the gangs!$"

SaffronCity_CopycatsHouse_2F_Text_MimickingFavoriteHobbyFemale::
	.string "{PLAYER}: Hi. You must like\n"
	.string "POKéMON.\p"
	.string "{PLAYER}: No, not me. I asked you.\p"
	.string "{PLAYER}: Pardon? You're weird!\p"
	.string "COPYCAT: Hmm? Quit mimicking? But\n"
	.string "I'm practicing to fool the gangs!$"

Text_MimicTeach::
	.string "Oh wow!\n"
	.string "A POKé DOLL!\p"
	.string "For me?\n"
	.string "Thank you!\p"
	.string "You know what?\n"
	.string "I can teach the move MIMIC.$"

Text_MimicDeclined::
	.string "Don't you like MIMIC?$"

Text_MimicWhichMon::
	.string "I really love MIMIC!\n"
	.string "Who's going to learn it?$"

Text_MimicTaughtMale::
	.string "{PLAYER}: Hi!\n"
	.string "Thanks for earlier!\p"
	.string "{PLAYER}: Pardon?\p"
	.string "{PLAYER}: Is it that fun to mimic my\n"
	.string "every move?\p"
	.string "COPYCAT: You bet!\n"
	.string "It's a scream!$"

Text_MimicTaughtFemale::
	.string "{PLAYER}: Hi!\n"
	.string "Thanks for what you did for us.\p"
	.string "{PLAYER}: Pardon?\p"
	.string "{PLAYER}: Is it really that fun to\n"
	.string "copy what I say and do?\p"
	.string "COPYCAT: You bet!\n"
	.string "It's a scream!$"

SaffronCity_CopycatsHouse_2F_Text_Doduo::
	.string "DODUO: Giiih!\p"
	.string "PRETTY MIRROR LOOK AT ME,\n"
	.string "SEE HOW PRETTY I CAN BE?$"

SaffronCity_CopycatsHouse_2F_Text_RareMonOnlyDoll::
	.string "A rare POKéMON! Huh? It's only a\n"
	.string "doll! Safer that way.$"

SaffronCity_CopycatsHouse_2F_Text_MarioWearingABucket::
	.string "A game with a plumber wearing a\n"
	.string "bucket on his head!$"

SaffronCity_CopycatsHouse_2F_Text_MySecrets::
	.string "…\p"
	.string "My Secrets!\p"
	.string "Skill: Mimicry!\n"
	.string "Hobby: Collecting dolls!\l"
	.string "Favorite POKéMON: CLEFAIRY!$"

