SafariZone_East_RestHouse_Frlg_MapScripts::
	.byte 0

SafariZone_East_RestHouse_EventScript_Rocker::
	msgbox SafariZone_East_Text_CaughtChanseyAllWorthwhile, MSGBOX_NPC
	end

SafariZone_East_RestHouse_EventScript_Scientist::
	msgbox SafariZone_East_Text_HowManyDidYouCatch, MSGBOX_NPC
	end

SafariZone_East_RestHouse_EventScript_BaldingMan::
	msgbox SafariZone_East_Text_TiredFromAllTheFun, MSGBOX_NPC
	end

SafariZone_East_Text_HowManyDidYouCatch::
	.string "How many did you catch? I'm\n"
	.string "bushed! PLASMA chased me twice!$"

SafariZone_East_Text_CaughtChanseyAllWorthwhile::
	.string "I caught a CHANSEY! Before TEAM\n"
	.string "PLASMA could set it “free”!$"

SafariZone_East_Text_TiredFromAllTheFun::
	.string "Whew! I'm tired! At least there\n"
	.string "are no gangs in here.$"

