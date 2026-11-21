#include "global.h"
#include "battle.h"
#include "gang_random.h"
#include "pokemon.h"
#include "random.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/moves.h"
#include "constants/pokedex.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/opponents.h"
#include "data.h"

// Gangster Outbreak: per-save randomizer (see gang_random.h)

#define LEGENDARY_MIN_LEVEL       50
#define LEGENDARY_ACCEPT_CHANCE   4   // a drawn legendary is kept 1 in this many times
#define STAGE1_FALLBACK_LEVEL     25  // first evolution without a level (stone, trade, ...)
#define STAGE2_FALLBACK_LEVEL     40  // second evolution without a level
#define MAX_PICK_TRIES            64
#define MIN_TRAINER_PARTY_SIZE    3

enum
{
    SALT_WILD = 1,
    SALT_TRAINER,
    SALT_PICK,
    SALT_BRANCH,
    SALT_LEGEND,
    SALT_EXTRA_MON,
};

static u32 GetSeed(void)
{
    const u8 *s = gSaveBlock2Ptr->pokedex.gangsterRandomSeed;
    return s[0] | (s[1] << 8) | (s[2] << 16) | ((u32)s[3] << 24);
}

void GangRandom_NewSeed(void)
{
    u32 seed = Random32();
    u8 *s = gSaveBlock2Ptr->pokedex.gangsterRandomSeed;

    s[0] = seed;
    s[1] = seed >> 8;
    s[2] = seed >> 16;
    s[3] = seed >> 24;
}

// Small deterministic hash (murmur3 finalizer)
static u32 Hash(u32 a, u32 b, u32 c, u32 d)
{
    u32 h = GetSeed() ^ 0x9E3779B9;
    u32 parts[4] = {a, b, c, d};
    u32 i;

    for (i = 0; i < ARRAY_COUNT(parts); i++)
    {
        h ^= parts[i] * 0x85EBCA6B;
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xE6546B64;
    }
    h ^= h >> 16;
    h *= 0x85EBCA6B;
    h ^= h >> 13;
    h *= 0xC2B2AE35;
    h ^= h >> 16;
    return h;
}

static bool32 IsLegendaryLike(enum Species species)
{
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    return info->isRestrictedLegendary || info->isSubLegendary || info->isMythical || info->isUltraBeast || info->isParadox;
}

// A species that may be drawn: a normal form of Gen 1-9 (no battle-only, cosmetic or special forms)
static bool32 IsCandidate(enum Species species)
{
    const struct SpeciesInfo *info;
    u32 dexNum;

    if (species == SPECIES_NONE || species >= NUM_SPECIES || !IsSpeciesEnabled(species))
        return FALSE;
    dexNum = SpeciesToNationalPokedexNum(species);
    if (dexNum == 0 || dexNum > NATIONAL_DEX_COUNT)
        return FALSE;
    info = &gSpeciesInfo[species];
    if (info->isTotem || info->isMegaEvolution || info->isPrimalReversion || info->isUltraBurst
     || info->isGigantamax || info->isTeraForm)
        return FALSE;
    // the main form of its species, or a regional form
    return GET_BASE_SPECIES_ID(species) == species
        || info->isAlolanForm || info->isGalarianForm || info->isHisuianForm || info->isPaldeanForm;
}

static enum Species GetFirstStage(enum Species species)
{
    u32 i;

    for (i = 0; i < 3; i++)
    {
        enum Species pre = GetSpeciesPreEvolution(species);
        if (pre == SPECIES_NONE || !IsSpeciesEnabled(pre))
            break;
        species = pre;
    }
    return species;
}

// Walk up the evolution line as far as the level allows; branches are picked by the hash
static enum Species EvolveForLevel(enum Species species, u8 level, u32 key)
{
    u32 stage;

    for (stage = 0; stage < 2; stage++)
    {
        const struct Evolution *evos = GetSpeciesEvolutions(species);
        enum Species options[16];
        u16 thresholds[16];
        u32 i, count = 0;

        if (evos == NULL)
            break;
        for (i = 0; evos[i].method != EVOLUTIONS_END && count < ARRAY_COUNT(options); i++)
        {
            enum Species target = evos[i].targetSpecies;
            if (target == SPECIES_NONE || !IsSpeciesEnabled(target) || gSpeciesInfo[target].isMegaEvolution
             || gSpeciesInfo[target].isGigantamax || gSpeciesInfo[target].isTotem)
                continue;
            options[count] = target;
            if (evos[i].method == EVO_LEVEL && evos[i].param > 0)
                thresholds[count] = evos[i].param;
            else
                thresholds[count] = (stage == 0) ? STAGE1_FALLBACK_LEVEL : STAGE2_FALLBACK_LEVEL;
            count++;
        }
        if (count == 0)
            break;
        i = Hash(SALT_BRANCH, key, species, stage) % count;
        if (level < thresholds[i])
            break;
        species = options[i];
    }
    return species;
}

static enum Species PickSpecies(u8 level, u32 key1, u32 key2)
{
    u32 try;

    for (try = 0; try < MAX_PICK_TRIES; try++)
    {
        enum Species species = 1 + Hash(SALT_PICK, key1, key2, try) % (NUM_SPECIES - 1);

        if (!IsCandidate(species))
            continue;
        if (IsLegendaryLike(species))
        {
            if (level < LEGENDARY_MIN_LEVEL || Hash(SALT_LEGEND, key1, key2, try) % LEGENDARY_ACCEPT_CHANCE != 0)
                continue;
            return species; // legendaries keep their own form
        }
        return EvolveForLevel(GetFirstStage(species), level, key1 ^ key2);
    }
    return SPECIES_NONE;
}

// The same route and table species always give the same random line in a save file
enum Species GangRandom_WildSpecies(enum Species tableSpecies, u8 level)
{
    u32 map, key;
    enum Species species;

    if (TESTING || tableSpecies == SPECIES_NONE)
        return tableSpecies;
    map = (gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum;
    key = Hash(SALT_WILD, map, tableSpecies, 0);
    species = PickSpecies(level, key, map);
    return species != SPECIES_NONE ? species : tableSpecies;
}

static bool32 IsRivalStarterLine(enum Species species)
{
    switch (GetFirstStage(species))
    {
    case SPECIES_BULBASAUR:
    case SPECIES_CHARMANDER:
    case SPECIES_SQUIRTLE:
        return TRUE;
    default:
        return FALSE;
    }
}

static bool32 IsRivalClass(u32 trainerClass)
{
    return trainerClass == TRAINER_CLASS_RIVAL
        || trainerClass == TRAINER_CLASS_RIVAL_EARLY_FRLG
        || trainerClass == TRAINER_CLASS_RIVAL_LATE_FRLG
        || trainerClass == TRAINER_CLASS_CHAMPION_FRLG;
}

static void ChangeSpecies(struct Pokemon *mon, enum Species species)
{
    u32 i, move = MOVE_NONE, pp = 0;
    u8 nickname[POKEMON_NAME_LENGTH + 1];
    enum Species original = GetMonData(mon, MON_DATA_SPECIES);

    // trainer Pokémon without a real nickname carry their species name: rename them
    GetMonData(mon, MON_DATA_NICKNAME, nickname);
    if (StringCompare(nickname, GetSpeciesName(original)) == 0)
        SetMonData(mon, MON_DATA_NICKNAME, GetSpeciesName(species));
    SetMonData(mon, MON_DATA_SPECIES, &species);
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        SetMonData(mon, MON_DATA_MOVE1 + i, &move);
        SetMonData(mon, MON_DATA_PP1 + i, &pp);
    }
    GiveMonInitialMoveset(mon);
    CalculateMonStats(mon);
}

// Trainers with fewer than MIN_TRAINER_PARTY_SIZE Pokémon get random extra ones at their strongest level
static void FillUpTrainerParty(struct Pokemon *party, u16 trainerNum)
{
    u32 i, count = 0;
    u8 level = 1;

    if (trainerNum == TRAINER_RIVAL_OAKS_LAB_SQUIRTLE || trainerNum == TRAINER_RIVAL_OAKS_LAB_BULBASAUR
     || trainerNum == TRAINER_RIVAL_OAKS_LAB_CHARMANDER)
        return; // the tutorial battle against Silver stays one on one
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&party[i], MON_DATA_SPECIES) == SPECIES_NONE)
            break;
        level = max(level, GetMonData(&party[i], MON_DATA_LEVEL));
        count++;
    }
    for (i = count; i < MIN_TRAINER_PARTY_SIZE; i++)
    {
        enum Species species = PickSpecies(level, Hash(SALT_EXTRA_MON, trainerNum, i, 0), trainerNum);
        if (species == SPECIES_NONE)
            species = SPECIES_RATTATA;
        CreateMon(&party[i], species, level, Random32(), OTID_STRUCT_RANDOM_NO_SHINY);
    }
}

void GangRandom_RandomizeTrainerParty(struct Pokemon *party, u16 trainerNum)
{
    u32 i, trainerClass;

    if (TESTING || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_RECORDED)))
        return;
    trainerClass = GetTrainerClassFromId(trainerNum);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        enum Species original = GetMonData(&party[i], MON_DATA_SPECIES);
        enum Species species;
        u8 level;

        if (original == SPECIES_NONE || GetMonData(&party[i], MON_DATA_IS_EGG))
            continue;
        // Silver keeps the starter he picked in front of Giovanni
        if (IsRivalClass(trainerClass) && IsRivalStarterLine(original))
            continue;
        level = GetMonData(&party[i], MON_DATA_LEVEL);
        species = PickSpecies(level, Hash(SALT_TRAINER, trainerNum, i, 0), trainerNum);
        if (species != SPECIES_NONE)
            ChangeSpecies(&party[i], species);
    }
    FillUpTrainerParty(party, trainerNum);
}
