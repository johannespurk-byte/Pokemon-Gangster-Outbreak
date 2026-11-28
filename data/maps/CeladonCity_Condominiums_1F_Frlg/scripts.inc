CeladonCity_Condominiums_1F_Frlg_MapScripts::
	.byte 0

CeladonCity_Condominiums_1F_EventScript_TeaWoman::
	lock
	faceplayer
	goto_if_set FLAG_TALKED_TO_TEA_LADY_AFTER_HOF, CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea
	goto_if_set FLAG_SYS_GAME_CLEAR, CeladonCity_Condominiums_1F_EventScript_TeaWomanMentionDaisy
	goto_if_set FLAG_GOT_TEA, CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea
	msgbox CeladonCity_Condominiums_1F_Text_TryThisDrinkInstead
	setflag FLAG_GOT_TEA
	giveitem ITEM_TEA
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	msgbox CeladonCity_Condominiums_1F_Text_NothingBeatsThirstLikeTea
	release
	end

CeladonCity_Condominiums_1F_EventScript_TeaWomanAfterTea::
	msgbox CeladonCity_Condominiums_1F_Text_MyDearMonsKeepMeCompany
	release
	end

CeladonCity_Condominiums_1F_EventScript_TeaWomanMentionDaisy::
	famechecker FAMECHECKER_DAISY, 4
	setflag FLAG_TALKED_TO_TEA_LADY_AFTER_HOF
	msgbox CeladonCity_Condominiums_1F_Text_DaisyComesToBuyTea
	release
	end

CeladonCity_Condominiums_1F_EventScript_Meowth::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_MEOWTH, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Meowth
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_Clefairy::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_CLEFAIRY, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Clefairy
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_Nidoran::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_NIDORAN_F, CRY_MODE_NORMAL
	msgbox CeladonCity_Condominiums_1F_Text_Nidoran
	waitmoncry
	release
	end

CeladonCity_Condominiums_1F_EventScript_SuiteSign::
	msgbox CeladonCity_Condominiums_1F_Text_ManagersSuite, MSGBOX_SIGN
	end

CeladonCity_Condominiums_1F_Text_Meowth::
	.string "MEOWTH: Meow!$"

CeladonCity_Condominiums_1F_Text_TryThisDrinkInstead::
	.string "Don't waste your money on drinks.\n"
	.string "The gangs mark them up.\p"
	.string "Try this instead.$"

CeladonCity_Condominiums_1F_Text_NothingBeatsThirstLikeTea::
	.string "Nothing beats a hot TEA.\p"
	.string "Even AQUA and MAGMA stop fighting\n"
	.string "for my TEA.$"

CeladonCity_Condominiums_1F_Text_MyDearMonsKeepMeCompany::
	.string "My POKéMON keep me company.\p"
	.string "MEOWTH even brings money home.\n"
	.string "From the gangs, I think.$"

CeladonCity_Condominiums_1F_Text_DaisyComesToBuyTea::
	.string "Oh, hello, dearie. Did you enjoy my\n"
	.string "TEA?\p"
	.string "Are you, by any chance, from\n"
	.string "PALLET TOWN?\p"
	.string "A girl from PALLET, DAISY, drinks\n"
	.string "TEA every day.\p"
	.string "She sneaks into the DEPT. STORE\n"
	.string "to buy it, right under AQUA's nose.$"

CeladonCity_Condominiums_1F_Text_Clefairy::
	.string "CLEFAIRY: Pi pippippi!$"

CeladonCity_Condominiums_1F_Text_Nidoran::
	.string "NIDORAN♀: Kya kyaoo!$"

CeladonCity_Condominiums_1F_Text_ManagersSuite::
	.string "CELADON MANSION\n"
	.string "Manager's Suite$"

