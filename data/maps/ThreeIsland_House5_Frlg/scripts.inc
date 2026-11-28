ThreeIsland_House5_Frlg_MapScripts::
	.byte 0

ThreeIsland_House5_EventScript_LittleGirl::
	lock
	faceplayer
	goto_if_set FLAG_RESCUED_LOSTELLE, ThreeIsland_House5_EventScript_LittleGirlLostelleFound
	msgbox ThreeIsland_House5_Text_ImNotLostelle
	release
	end

ThreeIsland_House5_EventScript_LittleGirlLostelleFound::
	msgbox ThreeIsland_House5_Text_IllGoPlayWithLostelle
	release
	end

ThreeIsland_House5_Text_ImNotLostelle::
	.string "Me? I'm not LOSTELLE.\p"
	.string "She probably went to the BERRY\n"
	.string "FOREST.\p"
	.string "She picks BERRIES there for lunch.$"

ThreeIsland_House5_Text_IllGoPlayWithLostelle::
	.string "I think I'll go play with LOSTELLE\n"
	.string "today.$"

