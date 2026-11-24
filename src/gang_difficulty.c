#include "global.h"
#include "gang_difficulty.h"
#include "load_save.h"
#include "malloc.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
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

u16 GangDifficulty_GetFittingItem(struct Pokemon *mon)
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

// Levels every trainer's Pokémon gain on top of their normal level
u32 GangDifficulty_GetLevelBonus(void)
{
    static const u8 sLevelBonus[GANG_DIFFICULTY_COUNT] = {2, 4, 6, 8};
    enum GangDifficulty difficulty = GangDifficulty_Get();
    return sLevelBonus[difficulty < GANG_DIFFICULTY_COUNT ? difficulty : GANG_DIFFICULTY_BOSS];
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
            item = GangDifficulty_GetFittingItem(mon);
        break;
    }
    SetMonData(mon, MON_DATA_HELD_ITEM, &item);
    CalculateMonStats(mon);
}

//------------------------------------------------------------------------------
// NEW GAME+
//------------------------------------------------------------------------------

#define NG_PLUS_UNLOCKED      0x80
#define NG_PLUS_CLEARED_MASK  0x07

EWRAM_DATA bool8 gNewGamePlusPending = FALSE;

bool32 NewGamePlus_IsUnlocked(void)
{
    return (gSaveBlock2Ptr->pokedex.gangsterNewGamePlus & NG_PLUS_UNLOCKED) != 0;
}

static enum GangDifficulty NewGamePlus_GetClearedDifficulty(void)
{
    u32 cleared = gSaveBlock2Ptr->pokedex.gangsterNewGamePlus & NG_PLUS_CLEARED_MASK;
    return cleared < GANG_DIFFICULTY_COUNT ? cleared : GANG_DIFFICULTY_RECRUIT;
}

void NewGamePlus_OnHallOfFame(void)
{
    enum GangDifficulty cleared = GangDifficulty_Get();

    if (NewGamePlus_IsUnlocked() && NewGamePlus_GetClearedDifficulty() > cleared)
        cleared = NewGamePlus_GetClearedDifficulty();
    gSaveBlock2Ptr->pokedex.gangsterNewGamePlus = NG_PLUS_UNLOCKED | cleared;
}

enum GangDifficulty GangDifficulty_GetMinSelectable(void)
{
    return gNewGamePlusPending ? NewGamePlus_GetClearedDifficulty() : GANG_DIFFICULTY_RECRUIT;
}

enum GangDifficulty GangDifficulty_GetDefaultSelection(void)
{
    if (gNewGamePlusPending)
    {
        enum GangDifficulty next = NewGamePlus_GetClearedDifficulty() + 1;
        return next < GANG_DIFFICULTY_COUNT ? next : GANG_DIFFICULTY_COUNT - 1;
    }
    return GANG_DIFFICULTY_RECOMMENDED;
}

// Everything from SaveBlock1 that survives into the NEW GAME+ (ClearSav1() wipes the block)
struct NewGamePlusBackup
{
    u8 partyCount;
    struct Pokemon party[PARTY_SIZE];
    u32 money;
    u16 registeredItem;
    struct ItemSlot pcItems[PC_ITEMS_COUNT];
    struct Bag bag;
    u8 dexSeen[NUM_DEX_FLAG_BYTES];
    u8 dexCaught[NUM_DEX_FLAG_BYTES];
};

void *NewGamePlus_BackupBeforeReset(void)
{
    struct NewGamePlusBackup *backup = AllocZeroed(sizeof(*backup));

    backup->partyCount = gSaveBlock1Ptr->playerPartyCount;
    memcpy(backup->party, gSaveBlock1Ptr->playerParty, sizeof(backup->party));
    backup->money = gSaveBlock1Ptr->money; // still encrypted with the kept encryption key
    backup->registeredItem = gSaveBlock1Ptr->registeredItem;
    memcpy(backup->pcItems, gSaveBlock1Ptr->pcItems, sizeof(backup->pcItems));
    memcpy(&backup->bag, &gSaveBlock1Ptr->bag, sizeof(backup->bag));
    memcpy(backup->dexSeen, gSaveBlock1Ptr->dexSeen, sizeof(backup->dexSeen));
    memcpy(backup->dexCaught, gSaveBlock1Ptr->dexCaught, sizeof(backup->dexCaught));
    return backup;
}

void NewGamePlus_RestoreAfterReset(void *data)
{
    struct NewGamePlusBackup *backup = data;

    gSaveBlock1Ptr->playerPartyCount = backup->partyCount;
    memcpy(gSaveBlock1Ptr->playerParty, backup->party, sizeof(backup->party));
    gSaveBlock1Ptr->money = backup->money;
    gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    memcpy(gSaveBlock1Ptr->pcItems, backup->pcItems, sizeof(backup->pcItems));
    memcpy(&gSaveBlock1Ptr->bag, &backup->bag, sizeof(backup->bag));
    // key items would let the player skip or break story events
    memset(gSaveBlock1Ptr->bag.keyItems, 0, sizeof(gSaveBlock1Ptr->bag.keyItems));
    memcpy(gSaveBlock1Ptr->dexSeen, backup->dexSeen, sizeof(backup->dexSeen));
    memcpy(gSaveBlock1Ptr->dexCaught, backup->dexCaught, sizeof(backup->dexCaught));
    Free(backup);
    LoadPlayerParty();
}

// The player keeps their trainer ID; their own Pokémon get the new name as original trainer so they obey
void NewGamePlus_UpdateOriginalTrainerNames(void)
{
    u32 trainerId = gSaveBlock2Ptr->playerTrainerId[0]
                  | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
                  | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
                  | (gSaveBlock2Ptr->playerTrainerId[3] << 24);
    u32 i, box, pos;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SANITY_HAS_SPECIES) && GetMonData(mon, MON_DATA_OT_ID) == trainerId)
            SetMonData(mon, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
    }
    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (pos = 0; pos < IN_BOX_COUNT; pos++)
        {
            if (GetBoxMonDataAt(box, pos, MON_DATA_SANITY_HAS_SPECIES) && GetBoxMonDataAt(box, pos, MON_DATA_OT_ID) == trainerId)
                SetBoxMonDataAt(box, pos, MON_DATA_OT_NAME, gSaveBlock2Ptr->playerName);
        }
    }
}
