@ Gangster Outbreak: gym leaders hand out the battle gimmicks in release order
@ Badge 2 -> MEGA RING, badge 4 -> Z-POWER RING (+ starter Z-Crystals), badge 6 -> TERA ORB.

EventScript_GangGiveMegaRing::
	checkitem ITEM_MEGA_RING
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangGimmickEnd
	msgbox Text_GangMegaRingIntro
	giveitem ITEM_MEGA_RING
	msgbox Text_GangMegaRingExplain
	return

EventScript_GangGiveZRing::
	checkitem ITEM_Z_POWER_RING
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangGimmickEnd
	msgbox Text_GangZRingIntro
	giveitem ITEM_Z_POWER_RING
	additem ITEM_NORMALIUM_Z
	additem ITEM_FIRIUM_Z
	additem ITEM_WATERIUM_Z
	additem ITEM_ELECTRIUM_Z
	additem ITEM_GRASSIUM_Z
	msgbox Text_GangZRingExplain
	return

EventScript_GangGiveTeraOrb::
	checkitem ITEM_TERA_ORB
	goto_if_eq VAR_RESULT, TRUE, EventScript_GangGimmickEnd
	msgbox Text_GangTeraOrbIntro
	giveitem ITEM_TERA_ORB
	msgbox Text_GangTeraOrbExplain
	return

EventScript_GangGimmickEnd::
	return

Text_GangMegaRingIntro::
	.string "One more thing. Every crew boss\n"
	.string "in KANTO wears one of these.\p"
	.string "Now you do too.$"

Text_GangMegaRingExplain::
	.string "With the MEGA RING in your BAG,\n"
	.string "a POKéMON holding its MEGA STONE\l"
	.string "can MEGA EVOLVE once per battle.\p"
	.string "Word is the GAME CORNER in\n"
	.string "CELADON trades legends WITH\l"
	.string "their stones. Go get rich.$"

Text_GangZRingIntro::
	.string "You hit like a real boss.\n"
	.string "So take the heavy hardware.$"

Text_GangZRingExplain::
	.string "That's a Z-POWER RING, plus a few\n"
	.string "Z-CRYSTALS to get you started.\p"
	.string "Let a POKéMON hold a Z-CRYSTAL\n"
	.string "and use a move of the same type:\p"
	.string "it turns into a Z-MOVE. One shot\n"
	.string "per battle, so make it count.$"

Text_GangTeraOrbIntro::
	.string "Last secret the big crews keep\n"
	.string "to themselves. It's yours now.$"

Text_GangTeraOrbExplain::
	.string "The TERA ORB lets one POKéMON per\n"
	.string "battle TERASTALLIZE and switch\l"
	.string "to its TERA TYPE.\p"
	.string "It never runs dry. Use it every\n"
	.string "fight you can.$"
