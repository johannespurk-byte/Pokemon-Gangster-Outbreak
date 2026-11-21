#include "global.h"
#include "gang_difficulty.h"
#include "pokemon.h"
#include "constants/abilities.h"
#include "constants/items.h"

// Gangster Outbreak difficulty levels. Stored in the save in SaveBlock2's Pokédex filler.
// Applied to every NPC trainer Pokémon right after it has been generated.

static const u8 *const sNames[GANG_DIFFICULTY_COUNT] =
{
    [GANG_DIFFICULTY_RECRUIT]        = COMPOUND_STRING("RECRUIT"),
    [GANG_DIFFICULTY_PETTY_CROOK]    = COMPOUND_STRING("PETTY CROOK"),
    [GANG_DIFFICULTY_ELITE_GANGSTER] = COMPOUND_STRING("ELITE GANGSTER"),
    [GANG_DIFFICULTY_BOSS]           = COMPOUND_STRING("BOSS"),
};

static const u8 *const sDescriptions[GANG_DIFFICULTY_COUNT] =
{
    [GANG_DIFFICULTY_RECRUIT]        = COMPOUND_STRING("Enemy POKéMON have no IVs and EVs\nand hold no items. Learn the ropes."),
    [GANG_DIFFICULTY_PETTY_CROOK]    = COMPOUND_STRING("Enemy POKéMON use their best\nAbility. Still no IVs, EVs or items."),
    [GANG_DIFFICULTY_ELITE_GANGSTER] = COMPOUND_STRING("Best Abilities, perfect IVs, full EVs.\nUnlocks the POKéMON editor."),
    [GANG_DIFFICULTY_BOSS]           = COMPOUND_STRING("Like ELITE GANGSTER, and every\nPOKéMON holds a fitting item."),
};

enum GangDifficulty GangDifficulty_Get(void)
{
    u32 difficulty = gSaveBlock2Ptr->pokedex.gangsterDifficulty;
    return difficulty < GANG_DIFFICULTY_COUNT ? difficulty : GANG_DIFFICULTY_RECRUIT;
}

void GangDifficulty_Set(enum GangDifficulty difficulty)
{
    gSaveBlock2Ptr->pokedex.gangsterDifficulty = difficulty < GANG_DIFFICULTY_COUNT ? difficulty : GANG_DIFFICULTY_RECRUIT;
}

bool32 GangDifficulty_IsStatEditorUnlocked(void)
{
    return GangDifficulty_Get() >= GANG_DIFFICULTY_ELITE_GANGSTER;
}

const u8 *GangDifficulty_GetName(enum GangDifficulty difficulty)
{
    return sNames[difficulty < GANG_DIFFICULTY_COUNT ? difficulty : 0];
}

const u8 *GangDifficulty_GetDescription(enum GangDifficulty difficulty)
{
    return sDescriptions[difficulty < GANG_DIFFICULTY_COUNT ? difficulty : 0];
}

static void SetAllIVs(struct Pokemon *mon, u8 value)
{
    u32 stat;
    for (stat = 0; stat < NUM_STATS; stat++)
        SetMonData(mon, MON_DATA_HP_IV + stat, &value);
}

static void SetEVs(struct Pokemon *mon, const u8 *evs)
{
    u32 stat;
    for (stat = 0; stat < NUM_STATS; stat++)
        SetMonData(mon, MON_DATA_HP_EV + stat, &evs[stat]);
}

// The ability slot with the best AI rating (hidden ability included)
static void SetBestAbility(struct Pokemon *mon)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u8 bestSlot = GetMonData(mon, MON_DATA_ABILITY_NUM);
    s32 bestRating = -128;
    u8 slot;

    for (slot = 0; slot < NUM_ABILITY_SLOTS; slot++)
    {
        enum Ability ability = GetSpeciesAbility(species, slot);
        if (ability == ABILITY_NONE)
            continue;
        if (gAbilitiesInfo[ability].aiRating > bestRating)
        {
            bestRating = gAbilitiesInfo[ability].aiRating;
            bestSlot = slot;
        }
    }
    SetMonData(mon, MON_DATA_ABILITY_NUM, &bestSlot);
}

// 510 EVs: offensive mons get attack stat + speed, slow ones get HP + attack stat
static void SetFullEVs(struct Pokemon *mon)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u8 evs[NUM_STATS] = {0};
    u32 atk = GetSpeciesBaseStat(species, STAT_ATK);
    u32 spAtk = GetSpeciesBaseStat(species, STAT_SPATK);
    u32 speed = GetSpeciesBaseStat(species, STAT_SPEED);
    u32 offense = (atk >= spAtk) ? STAT_ATK : STAT_SPATK;

    evs[offense] = 252;
    if (speed >= 60)
    {
        evs[STAT_SPEED] = 252;
        evs[STAT_HP] = 6;
    }
    else
    {
        evs[STAT_HP] = 252;
        evs[GetSpeciesBaseStat(species, STAT_DEF) >= GetSpeciesBaseStat(species, STAT_SPDEF) ? STAT_DEF : STAT_SPDEF] = 6;
    }
    SetEVs(mon, evs);
}

static bool32 CanSpeciesEvolve(enum Species species)
{
    const struct Evolution *evolutions = GetSpeciesEvolutions(species);
    return evolutions != NULL && evolutions[0].method != EVOLUTIONS_END && evolutions[0].method != EVO_NONE;
}

static u16 GetFittingItem(struct Pokemon *mon)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u32 atk = GetSpeciesBaseStat(species, STAT_ATK);
    u32 spAtk = GetSpeciesBaseStat(species, STAT_SPATK);
    u32 speed = GetSpeciesBaseStat(species, STAT_SPEED);
    u32 bulk = GetSpeciesBaseStat(species, STAT_HP) + GetSpeciesBaseStat(species, STAT_DEF) + GetSpeciesBaseStat(species, STAT_SPDEF);

    if (CanSpeciesEvolve(species))
        return ITEM_EVIOLITE;
    if (max(atk, spAtk) >= 100 && speed >= 80)
        return ITEM_LIFE_ORB;
    if (bulk >= 260)
        return ITEM_LEFTOVERS;
    return ITEM_SITRUS_BERRY;
}

void GangDifficulty_ApplyToTrainerMon(struct Pokemon *mon)
{
    enum GangDifficulty difficulty = GangDifficulty_Get();
    u8 zero[NUM_STATS] = {0};
    u16 item = ITEM_NONE;

    if (GetMonData(mon, MON_DATA_SPECIES) == SPECIES_NONE)
        return;

    switch (difficulty)
    {
    case GANG_DIFFICULTY_RECRUIT:
        SetAllIVs(mon, 0);
        SetEVs(mon, zero);
        break;
    case GANG_DIFFICULTY_PETTY_CROOK:
        SetBestAbility(mon);
        SetAllIVs(mon, 0);
        SetEVs(mon, zero);
        break;
    case GANG_DIFFICULTY_ELITE_GANGSTER:
        SetBestAbility(mon);
        SetAllIVs(mon, MAX_PER_STAT_IVS);
        SetFullEVs(mon);
        break;
    case GANG_DIFFICULTY_BOSS:
    default:
        SetBestAbility(mon);
        SetAllIVs(mon, MAX_PER_STAT_IVS);
        SetFullEVs(mon);
        item = GetMonData(mon, MON_DATA_HELD_ITEM); // keep an item the trainer already has
        if (item == ITEM_NONE)
            item = GetFittingItem(mon);
        break;
    }
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    CalculateMonStats(mon);
}
