CeladonCity_DepartmentStore_1F_Frlg_MapScripts::
	.byte 0

CeladonCity_DepartmentStore_1F_EventScript_Receptionist::
	msgbox CeladonCity_DepartmentStore_1F_Text_WelcomeToDeptStore, MSGBOX_NPC
	end

CeladonCity_DepartmentStore_1F_EventScript_LayoutSign::
	msgbox CeladonCity_DepartmentStore_1F_Text_FloorDescriptions, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_1F_EventScript_FloorSign::
	msgbox CeladonCity_DepartmentStore_1F_Text_ServiceCounter, MSGBOX_SIGN
	end

CeladonCity_DepartmentStore_1F_Text_WelcomeToDeptStore::
	.string "Welcome to the CELADON DEPT.\n"
	.string "STORE. What's left of it.\p"
	.string "TEAM AQUA and TEAM MAGMA fight\n"
	.string "over every floor.\p"
	.string "The board shows who runs what.$"

CeladonCity_DepartmentStore_1F_Text_FloorDescriptions::
	.string "1F: SERVICE COUNTER (looted)\p"
	.string "2F: TRAINER'S MARKET (AQUA)\p"
	.string "3F: TV GAME SHOP (MAGMA)\p"
	.string "4F: WISE MAN GIFTS (AQUA)\p"
	.string "5F: DRUGSTORE (MAGMA)\p"
	.string "ROOFTOP: VENDING MACHINES\n"
	.string "(still working!)$"

CeladonCity_DepartmentStore_1F_Text_ServiceCounter::
	.string "1F: SERVICE COUNTER$"

