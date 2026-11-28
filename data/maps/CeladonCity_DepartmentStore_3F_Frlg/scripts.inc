CeladonCity_DepartmentStore_3F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_3F_EventScript_CounterTutor::
	goto EventScript_CounterTutor
	end

CeladonCity_DepartmentStore_3F_EventScript_GBAKid1::
	msgbox CeladonCity_DepartmentStore_3F_Text_OTStandsForOriginalTrainer, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_GBAKid2::
	msgbox CeladonCity_DepartmentStore_3F_Text_BuddyTradingKangaskhanForHaunter, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_GBAKid3::
	msgbox CeladonCity_DepartmentStore_3F_Text_HaunterEvolvedOnTrade, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_LittleGirl::
	msgbox CeladonCity_DepartmentStore_3F_Text_CanIdentifyTradeMonsByID, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_3F_EventScript_SuperNES::
	msgbox CeladonCity_DepartmentStore_3F_Text_ItsSuperNES, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV1::
	msgbox CeladonCity_DepartmentStore_3F_Text_AnRPG, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV2::
	msgbox CeladonCity_DepartmentStore_3F_Text_SportsGame, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV3::
	msgbox CeladonCity_DepartmentStore_3F_Text_PuzzleGame, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_TV4::
	msgbox CeladonCity_DepartmentStore_3F_Text_FightingGame, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_3F_Text_TVGameShop, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_EventScript_Poster::
	msgbox CeladonCity_DepartmentStore_3F_Text_RedGreenBothArePokemon, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_3F_Text_OTStandsForOriginalTrainer::
	.string "OT stands for original TRAINER.\p"
	.string "Out here, it means the first\n"
	.string "one who stole it.$"

CeladonCity_DepartmentStore_3F_Text_BuddyTradingKangaskhanForHaunter::
	.string "All right!\p"
	.string "My buddy's going to trade me his\n"
	.string "KANGASKHAN for my HAUNTER!$"

CeladonCity_DepartmentStore_3F_Text_HaunterEvolvedOnTrade::
	.string "Come on, HAUNTER!\p"
	.string "I love HAUNTER!\n"
	.string "I collect them!\p"
	.string "Huh?\p"
	.string "HAUNTER turned into a different\n"
	.string "POKéMON while being traded!$"

CeladonCity_DepartmentStore_3F_Text_CanIdentifyTradeMonsByID::
	.string "TEAM MAGMA sells “used”\n"
	.string "POKéMON in the back.\p"
	.string "Check the ID No. before you\n"
	.string "pay anything!$"

CeladonCity_DepartmentStore_3F_Text_ItsSuperNES::
	.string "It's a Super NES.$"

CeladonCity_DepartmentStore_3F_Text_AnRPG::
	.string "An RPG!\n"
	.string "Who has time for that now?$"

CeladonCity_DepartmentStore_3F_Text_SportsGame::
	.string "A sports game!\n"
	.string "The case is smashed.$"

CeladonCity_DepartmentStore_3F_Text_PuzzleGame::
	.string "A puzzle game!\n"
	.string "Somebody took the cartridge.$"

CeladonCity_DepartmentStore_3F_Text_FightingGame::
	.string "A fighting game!\n"
	.string "Real life is tougher.$"

CeladonCity_DepartmentStore_3F_Text_TVGameShop::
	.string "3F: TV GAME SHOP$"

CeladonCity_DepartmentStore_3F_Text_RedGreenBothArePokemon::
	.string "Red and Green!\n"
	.string "Both are POKéMON!$"

Text_CounterTeach::
	.string "Oh, hi!\n"
	.string "I finally finished POKéMON.\p"
	.string "Not done yet? How about I teach\n"
	.string "you a good move?\p"
	.string "The move I have in mind is\n"
	.string "COUNTER.\p"
	.string "Not like the one I'm leaning on,\n"
	.string "mind you!$"

Text_CounterDeclined::
	.string "You're not interested? Come see\n"
	.string "me if you change your mind.$"

Text_CounterWhichMon::
	.string "Which POKéMON should I teach\n"
	.string "COUNTER to?$"

Text_CounterTaught::
	.string "Are you using that COUNTER move\n"
	.string "I taught your POKéMON?$"
