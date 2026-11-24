Common_EventScript_FindItem::
	callnative GetItemBallIdAndAmountFromTemplate
	finditem VAR_RESULT VAR_0x8009
	end

@ Gangster Outbreak: extra loot bags on the maps (tools/apocalypse/loot.py); contents are random (GangItems_RandomizeFoundItem)
GangLoot_EventScript_Bag::
	finditem ITEM_POTION
	end
