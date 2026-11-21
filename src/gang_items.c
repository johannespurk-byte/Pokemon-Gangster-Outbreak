#include "global.h"
#include "event_data.h"
#include "gang_random.h"
#include "constants/flags.h"

// Gangster Outbreak: the Poké Vial and the Auto-Repel hold 1 charge plus one per Gym Badge.
// HEAL-BOTs (the Pokémon Center nurses) refill them.

static u32 GetMaxCharges(void)
{
    u32 flag, charges = 1;

    for (flag = FLAG_BADGE01_GET; flag <= FLAG_BADGE08_GET; flag++)
    {
        if (FlagGet(flag))
            charges++;
    }
    return charges;
}

u32 GangItems_GetVialCharges(void)
{
    return gSaveBlock2Ptr->pokedex.gangsterVialCharges;
}

u32 GangItems_GetRepelCharges(void)
{
    return gSaveBlock2Ptr->pokedex.gangsterRepelCharges;
}

void GangItems_UseVialCharge(void)
{
    if (gSaveBlock2Ptr->pokedex.gangsterVialCharges > 0)
        gSaveBlock2Ptr->pokedex.gangsterVialCharges--;
}

void GangItems_UseRepelCharge(void)
{
    if (gSaveBlock2Ptr->pokedex.gangsterRepelCharges > 0)
        gSaveBlock2Ptr->pokedex.gangsterRepelCharges--;
}

// Special, also used when the items are handed over
void GangItems_Recharge(void)
{
    gSaveBlock2Ptr->pokedex.gangsterVialCharges = GetMaxCharges();
    gSaveBlock2Ptr->pokedex.gangsterRepelCharges = GetMaxCharges();
}
