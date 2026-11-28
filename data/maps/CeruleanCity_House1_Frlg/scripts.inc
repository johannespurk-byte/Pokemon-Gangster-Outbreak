CeruleanCity_House1_Frlg_MapScripts::
	.byte 0

CeruleanCity_House1_EventScript_BadgeGuy::
	lock
	faceplayer
	msgbox CeruleanCity_House1_Text_BadgesHaveAmazingSecrets
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeAnotherBadge::
	message CeruleanCity_House1_Text_DescribeWhichBadge
	waitmessage
	setvar VAR_0x8004, SCROLL_MULTI_BADGES
	special ShowScrollableMultichoice
	switch VAR_RESULT
	case 0, CeruleanCity_House1_EventScript_DescribeBoulderBadge
	case 1, CeruleanCity_House1_EventScript_DescribeCascadeBadge
	case 2, CeruleanCity_House1_EventScript_DescribeThunderBadge
	case 3, CeruleanCity_House1_EventScript_DescribeRainbowBadge
	case 4, CeruleanCity_House1_EventScript_DescribeSoulBadge
	case 5, CeruleanCity_House1_EventScript_DescribeMarshBadge
	case 6, CeruleanCity_House1_EventScript_DescribeVolcanoBadge
	case 7, CeruleanCity_House1_EventScript_DescribeEarthBadge
	case 8, CeruleanCity_House1_EventScript_StopDescribingBadges
	case 127, CeruleanCity_House1_EventScript_StopDescribingBadges
	end

CeruleanCity_House1_EventScript_DescribeBoulderBadge::
	msgbox CeruleanCity_House1_Text_AttackStatFlash
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeCascadeBadge::
	msgbox CeruleanCity_House1_Text_ObeyLv30Cut
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeThunderBadge::
	msgbox CeruleanCity_House1_Text_SpeedStatFly
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeRainbowBadge::
	msgbox CeruleanCity_House1_Text_ObeyLv50Strength
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeSoulBadge::
	msgbox CeruleanCity_House1_Text_DefenseStatSurf
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeMarshBadge::
	msgbox CeruleanCity_House1_Text_ObeyLv70RockSmash
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeVolcanoBadge::
	msgbox CeruleanCity_House1_Text_SpStatsWaterfall
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_DescribeEarthBadge::
	msgbox CeruleanCity_House1_Text_AllMonsWillObeyYou
	goto CeruleanCity_House1_EventScript_DescribeAnotherBadge
	end

CeruleanCity_House1_EventScript_StopDescribingBadges::
	msgbox CeruleanCity_House1_Text_ComeVisitAnytime
	release
	end

CeruleanCity_House1_Text_BadgesHaveAmazingSecrets::
	.string "Only tough TRAINERS can take\n"
	.string "BADGES from the gang bosses.\p"
	.string "I see you have at least one.\p"
	.string "Those BADGES have amazing secrets,\n"
	.string "did you know?$"

CeruleanCity_House1_Text_DescribeWhichBadge::
	.string "Now then…\p"
	.string "Which of the eight gang BADGES\n"
	.string "should I describe?$"

CeruleanCity_House1_Text_ComeVisitAnytime::
	.string "Come visit me anytime you wish.$"

CeruleanCity_House1_Text_AttackStatFlash::
	.string "The ATTACK stat of all your\n"
	.string "POKéMON increases a little bit.\p"
	.string "It also lets you use the move\n"
	.string "FLASH outside of battle.$"

CeruleanCity_House1_Text_ObeyLv30Cut::
	.string "This BADGE raises the level cap\n"
	.string "of your crew to Lv. 32.\p"
	.string "Your POKéMON always follow you,\n"
	.string "no matter where they came from.\p"
	.string "It also lets you use\n"
	.string "CUT outside of battle.$"

CeruleanCity_House1_Text_SpeedStatFly::
	.string "It raises the SPEED of your\n"
	.string "POKéMON a little.\p"
	.string "With the HM in your BAG, your\n"
	.string "POKéMON can FLY outside of\p"
	.string "battle.$"

CeruleanCity_House1_Text_ObeyLv50Strength::
	.string "This BADGE raises the level cap\n"
	.string "of your crew to Lv. 44.\p"
	.string "Your POKéMON always follow you,\n"
	.string "no matter where they came from.\p"
	.string "It also lets you use\n"
	.string "STRENGTH outside of battle.$"

CeruleanCity_House1_Text_DefenseStatSurf::
	.string "The DEFENSE stat of all your\n"
	.string "POKéMON increases a little bit.\p"
	.string "It also lets you use the move\n"
	.string "SURF outside of battle.$"

CeruleanCity_House1_Text_ObeyLv70RockSmash::
	.string "This BADGE raises the level cap\n"
	.string "of your crew to Lv. 54.\p"
	.string "Your POKéMON always follow you,\n"
	.string "no matter where they came from.\p"
	.string "It also lets you use\n"
	.string "ROCK SMASH outside of battle.$"

CeruleanCity_House1_Text_SpStatsWaterfall::
	.string "It raises SP. ATK and SP. DEF of\n"
	.string "your POKéMON a little.\p"
	.string "With the HM in your BAG, your\n"
	.string "POKéMON can use WATERFALL\p"
	.string "outside of battle.$"

CeruleanCity_House1_Text_AllMonsWillObeyYou::
	.string "With all eight BADGES, KANTO\n"
	.string "is yours. Only the LEAGUE is left!$"

