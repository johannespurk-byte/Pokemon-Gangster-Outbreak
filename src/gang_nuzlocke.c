#include "global.h"
#include "battle.h"
#include "gang_difficulty.h"
#include "gang_nuzlocke.h"
#include "pokemon.h"
#include "constants/battle.h"
#include "constants/region_map_sections.h"

// The dead marker lives in an otherwise unused bit of the BoxPokemon, so it survives the PC and trades.
#define DEAD_BIT unused_13

static EWRAM_DATA bool8 sCatchAllowedThisBattle = FALSE;

bool32 GangNuzlocke_IsEnabled(void)
{
    return gSaveBlock2Ptr->pokedex.gangsterNuzlocke && GangDifficulty_Get() == GANG_DIFFICULTY_BOSS;
}

void GangNuzlocke_Set(bool32 enabled)
{
    gSaveBlock2Ptr->pokedex.gangsterNuzlocke = enabled ? TRUE : FALSE;
    memset(gSaveBlock2Ptr->pokedex.gangsterNuzlockeAreas, 0, sizeof(gSaveBlock2Ptr->pokedex.gangsterNuzlockeAreas));
}

void GangNuzlocke_OnBattleInit(void)
{
    u32 mapsec = gMapHeader.regionMapSectionId;
    u8 *areas = gSaveBlock2Ptr->pokedex.gangsterNuzlockeAreas;

    sCatchAllowedThisBattle = FALSE;
    if (!GangNuzlocke_IsEnabled())
        return;
    if (gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED
                          | BATTLE_TYPE_FIRST_BATTLE | BATTLE_TYPE_CATCH_TUTORIAL))
        return;
    if (mapsec / 8 >= sizeof(gSaveBlock2Ptr->pokedex.gangsterNuzlockeAreas))
        return;
    if (!(areas[mapsec / 8] & (1 << (mapsec % 8))))
    {
        sCatchAllowedThisBattle = TRUE;     // first encounter in this area
        areas[mapsec / 8] |= 1 << (mapsec % 8);
    }
}

static bool32 IsWildMonShiny(u32 slot)
{
    struct Pokemon *mon = &gParties[B_TRAINER_OPPONENT_A][slot];
    // an empty slot has personality and OT ID 0, which would count as shiny
    return GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && IsMonShiny(mon);
}

bool32 GangNuzlocke_IsCatchBlocked(void)
{
    if (!GangNuzlocke_IsEnabled() || sCatchAllowedThisBattle)
        return FALSE;
    if (IsWildMonShiny(0) || IsWildMonShiny(1))
        return FALSE;                       // shiny clause
    return TRUE;
}

void GangNuzlocke_MarkFaintedPartyMons(void)
{
    u32 i;

    if (!GangNuzlocke_IsEnabled())
        return;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG)
         && GetMonData(mon, MON_DATA_HP) == 0)
            mon->box.DEAD_BIT = TRUE;
    }
}

bool32 GangNuzlocke_IsBoxMonDead(const struct BoxPokemon *boxMon)
{
    return GangNuzlocke_IsEnabled() && boxMon->DEAD_BIT;
}
