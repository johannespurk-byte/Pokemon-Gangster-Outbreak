#include "global.h"
#include "battle.h"
#include "caps.h"
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
#include "event_data.h"
#include "gang_difficulty.h"
#include "constants/battle_move_effects.h"
#include "constants/flags.h"
#include "constants/items.h"

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
    SALT_RIVAL,
    SALT_LEAGUE,
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

u32 GangRandom_Hash(u32 a, u32 b, u32 c, u32 d)
{
    return Hash(a, b, c, d);
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

// Gym bosses (the villain teams and Giovanni) keep their signature teams and their Pokémon
bool32 GangRandom_IsBossTrainer(u16 trainerNum)
{
    switch (trainerNum)
    {
    case TRAINER_LEADER_BROCK:
    case TRAINER_LEADER_MISTY:
    case TRAINER_LEADER_LT_SURGE:
    case TRAINER_LEADER_ERIKA:
    case TRAINER_GANG_MAXIE:
    case TRAINER_LEADER_KOGA:
    case TRAINER_LEADER_SABRINA:
    case TRAINER_LEADER_BLAINE:
    case TRAINER_LEADER_GIOVANNI:
        return TRUE;
    default:
        return FALSE;
    }
}

// --- level scaling ------------------------------------------------------------------------------------------
// Opponents never fall too far behind the player's strongest Pokémon, but nobody is pushed past the level cap
// (the level of the next gym boss's strongest Pokémon). Levels are only ever raised, never lowered.

#define TRAINER_SCALE_BEHIND   2  // normal trainers: strongest Pokémon this many levels below the player's
#define WILD_SCALE_BEHIND      6  // wild Pokémon: at least this many levels below the player's ...
#define WILD_SCALE_SPREAD      4  // ... plus 0-3

static u32 GetPlayerScalingLevel(void)
{
    u32 i, level = 1;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_IS_EGG))
            level = max(level, GetMonData(mon, MON_DATA_LEVEL));
    }
    return min(level, GetCurrentLevelCap());
}

static void SetMonLevel(struct Pokemon *mon, u32 level)
{
    u32 exp = gExperienceTables[gSpeciesInfo[GetMonData(mon, MON_DATA_SPECIES)].growthRate][level];

    SetMonData(mon, MON_DATA_EXP, &exp);
    CalculateMonStats(mon);
}

u8 GangRandom_ScaleWildLevel(u8 level)
{
    u32 player = GetPlayerScalingLevel();
    u32 floor;

    if (TESTING || player <= WILD_SCALE_BEHIND)
        return level;
    floor = player - WILD_SCALE_BEHIND + Random() % WILD_SCALE_SPREAD;
    if (level < floor)
        level = min(floor, GetCurrentLevelCap());
    return level;
}

// Lifts the whole team so its strongest Pokémon is close to the player's (Silver: one level above)
static void ScaleTrainerParty(struct Pokemon *party, bool32 rival)
{
    u32 i, player = GetPlayerScalingLevel(), strongest = 0, target, shift, cap = GetCurrentLevelCap();

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&party[i], MON_DATA_SPECIES) != SPECIES_NONE)
            strongest = max(strongest, GetMonData(&party[i], MON_DATA_LEVEL));
    }
    target = rival ? player + 1 : (player > TRAINER_SCALE_BEHIND ? player - TRAINER_SCALE_BEHIND : 1);
    target = min(target, cap);
    if (strongest == 0 || target <= strongest)
        return;
    shift = target - strongest;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&party[i], MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(&party[i], MON_DATA_IS_EGG))
            SetMonLevel(&party[i], min(GetMonData(&party[i], MON_DATA_LEVEL) + shift, MAX_LEVEL));
    }
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


// --- Pokémon League ------------------------------------------------------------------------------------------
// The Elite Four are the heroes of other generations, two per room in a double battle, with Pokémon of their
// home region; the Champion is Silver. Their teams grow with the difficulty: more Pokémon, higher levels,
// stronger species (on BOSS each one brings a legendary), the best moves and more items.

#define LEAGUE_LEVEL          63  // first run: the level cap with eight badges
#define LEAGUE_REMATCH_LEVEL  75  // after becoming Champion
#define LEAGUE_ROOM_CHAMPION  5

struct LeagueMember
{
    u16 trainer;
    u8 room;            // 1-4 Elite Four, 5 Champion
    u16 dexFirst;       // home region (National Dex numbers)
    u16 dexLast;
    u16 ace;            // their partner Pokémon, always last (SPECIES_NONE: Silver's own random partner)
    u16 legend;         // their legendary on BOSS
};

static const struct LeagueMember sLeagueMembers[] =
{
    {TRAINER_LEAGUE_ETHAN,               1, 152, 251, SPECIES_TYPHLOSION, SPECIES_HO_OH},
    {TRAINER_LEAGUE_LYRA,                1, 152, 251, SPECIES_MEGANIUM,   SPECIES_LUGIA},
    {TRAINER_LEAGUE_LUCAS,               2, 387, 493, SPECIES_INFERNAPE,  SPECIES_DIALGA},
    {TRAINER_LEAGUE_DAWN,                2, 387, 493, SPECIES_EMPOLEON,   SPECIES_PALKIA},
    {TRAINER_LEAGUE_HILBERT,             3, 494, 649, SPECIES_SAMUROTT,   SPECIES_RESHIRAM},
    {TRAINER_LEAGUE_HILDA,               3, 494, 649, SPECIES_SERPERIOR,  SPECIES_ZEKROM},
    {TRAINER_LEAGUE_VICTOR,              4, 810, 898, SPECIES_CINDERACE,  SPECIES_ZACIAN},
    {TRAINER_LEAGUE_GLORIA,              4, 810, 898, SPECIES_RILLABOOM,  SPECIES_ZAMAZENTA},
    {TRAINER_CHAMPION_FIRST_SQUIRTLE,    LEAGUE_ROOM_CHAMPION, 1, NATIONAL_DEX_COUNT, SPECIES_NONE, SPECIES_MEWTWO},
    {TRAINER_CHAMPION_FIRST_BULBASAUR,   LEAGUE_ROOM_CHAMPION, 1, NATIONAL_DEX_COUNT, SPECIES_NONE, SPECIES_MEWTWO},
    {TRAINER_CHAMPION_FIRST_CHARMANDER,  LEAGUE_ROOM_CHAMPION, 1, NATIONAL_DEX_COUNT, SPECIES_NONE, SPECIES_MEWTWO},
    {TRAINER_CHAMPION_REMATCH_SQUIRTLE,  LEAGUE_ROOM_CHAMPION, 1, NATIONAL_DEX_COUNT, SPECIES_NONE, SPECIES_MEWTWO},
    {TRAINER_CHAMPION_REMATCH_BULBASAUR, LEAGUE_ROOM_CHAMPION, 1, NATIONAL_DEX_COUNT, SPECIES_NONE, SPECIES_MEWTWO},
    {TRAINER_CHAMPION_REMATCH_CHARMANDER,LEAGUE_ROOM_CHAMPION, 1, NATIONAL_DEX_COUNT, SPECIES_NONE, SPECIES_MEWTWO},
};

//                                                   RECRUIT PETTY ELITE BOSS
static const u8 sLeaguePartySize[GANG_DIFFICULTY_COUNT]    = {3,   4,    5,    6};
static const u8 sChampionPartySize[GANG_DIFFICULTY_COUNT]  = {5,   6,    6,    6};
static const u8 sLeagueLevelBonus[GANG_DIFFICULTY_COUNT]   = {0,   2,    4,    6};
static const u16 sLeagueMinBst[GANG_DIFFICULTY_COUNT]      = {430, 460,  500,  525};
static const u8 sLeagueFullRestores[GANG_DIFFICULTY_COUNT] = {1,   2,    3,    4};

static const struct LeagueMember *GetLeagueMember(u16 trainerNum)
{
    u32 i;

    if (!IS_FRLG)
        return NULL;
    for (i = 0; i < ARRAY_COUNT(sLeagueMembers); i++)
    {
        if (sLeagueMembers[i].trainer == trainerNum)
            return &sLeagueMembers[i];
    }
    return NULL;
}

static enum GangDifficulty GetLeagueDifficulty(void)
{
    enum GangDifficulty difficulty = GangDifficulty_Get();
    return difficulty < GANG_DIFFICULTY_COUNT ? difficulty : GANG_DIFFICULTY_BOSS;
}

static bool32 IsFullyEvolved(enum Species species)
{
    const struct Evolution *evos = GetSpeciesEvolutions(species);
    u32 i;

    if (evos == NULL)
        return TRUE;
    for (i = 0; evos[i].method != EVOLUTIONS_END; i++)
    {
        enum Species target = evos[i].targetSpecies;
        if (target != SPECIES_NONE && IsSpeciesEnabled(target) && !gSpeciesInfo[target].isMegaEvolution
         && !gSpeciesInfo[target].isGigantamax && !gSpeciesInfo[target].isTotem)
            return FALSE;
    }
    return TRUE;
}

static u32 GetBaseStatTotal(enum Species species)
{
    const struct SpeciesInfo *info = &gSpeciesInfo[species];
    return info->baseHP + info->baseAttack + info->baseDefense + info->baseSpeed + info->baseSpAttack + info->baseSpDefense;
}

static enum Species PickLeagueSpecies(const struct LeagueMember *member, u32 slot, u32 minBst, const u16 *taken, u32 takenCount)
{
    u32 try, i, range = member->dexLast - member->dexFirst + 1;

    for (try = 0; try < 400; try++)
    {
        u32 dexNum = member->dexFirst + Hash(SALT_LEAGUE, member->trainer, slot, try) % range;
        enum Species species = NationalPokedexNumToSpecies(dexNum);
        bool32 dup = FALSE;

        if (!IsCandidate(species) || IsLegendaryLike(species) || !IsFullyEvolved(species))
            continue;
        if (GetBaseStatTotal(species) < (try < 300 ? minBst : 0)) // relax if the region has too few
            continue;
        for (i = 0; i < takenCount; i++)
        {
            if (taken[i] == species)
                dup = TRUE;
        }
        if (!dup && species != member->ace)
            return species;
    }
    return SPECIES_GARCHOMP;
}

static bool32 IsUsableAttack(enum Move move)
{
    u32 power;

    if (move == MOVE_NONE || move >= MOVES_COUNT || GetMoveCategory(move) == DAMAGE_CATEGORY_STATUS)
        return FALSE;
    power = GetMovePower(move);
    if (power < 40 || power > 130)
        return FALSE; // no fixed-damage moves, no Hyper Beam / Explosion
    switch (GetMoveEffect(move))
    {
    case EFFECT_TWO_TURNS_ATTACK:
    case EFFECT_SOLAR_BEAM:
    case EFFECT_FOCUS_PUNCH:
    case EFFECT_FUTURE_SIGHT:
    case EFFECT_FIRST_TURN_ONLY:
    case EFFECT_SKY_DROP:
    case EFFECT_DREAM_EATER:
    case EFFECT_SNORE:
    case EFFECT_LAST_RESORT:
    case EFFECT_SYNCHRONOISE:
    case EFFECT_FINAL_GAMBIT:
        return FALSE;
    default:
        return TRUE;
    }
}

static u32 ScoreAttack(enum Species species, enum Move move)
{
    u32 accuracy = GetMoveAccuracy(move), score;
    enum Type type = GetMoveType(move);
    bool32 physical = GetMoveCategory(move) == DAMAGE_CATEGORY_PHYSICAL;
    u32 atk = GetSpeciesBaseStat(species, STAT_ATK), spAtk = GetSpeciesBaseStat(species, STAT_SPATK);

    score = GetMovePower(move) * GetMoveStrikeCount(move) * (accuracy == 0 ? 100 : accuracy);
    if (type == GetSpeciesType(species, 0) || type == GetSpeciesType(species, 1))
        score = score * 3 / 2;
    if (physical ? atk >= spAtk : spAtk >= atk)
        score = score * 5 / 4;
    else
        score = score * 3 / 4;
    return score;
}

static void TryAddAttack(enum Species species, enum Move move, enum Move *best, u32 *bestScore)
{
    enum Type type;
    u32 score;

    if (!IsUsableAttack(move))
        return;
    type = GetMoveType(move);
    if (type >= NUMBER_OF_MON_TYPES)
        return;
    score = ScoreAttack(species, move);
    if (score > bestScore[type])
    {
        bestScore[type] = score;
        best[type] = move;
    }
}

// The strongest attack of each type it can learn (level-up moves; from ELITE GANGSTER on also TMs and tutors),
// then the four best types. Free slots keep the moves it would know at its level.
static void GiveLeagueMoveset(struct Pokemon *mon, bool32 teachable)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    u32 level = GetMonData(mon, MON_DATA_LEVEL);
    enum Move best[NUMBER_OF_MON_TYPES] = {MOVE_NONE};
    u32 bestScore[NUMBER_OF_MON_TYPES] = {0};
    enum Move chosen[MAX_MON_MOVES] = {MOVE_NONE};
    const struct LevelUpMove *levelUp = GetSpeciesLevelUpLearnset(species);
    u32 i, j, count = 0;

    for (i = 0; levelUp != NULL && levelUp[i].move != LEVEL_UP_MOVE_END; i++)
    {
        if (levelUp[i].level <= level)
            TryAddAttack(species, levelUp[i].move, best, bestScore);
    }
    if (teachable)
    {
        const u16 *learnset = GetSpeciesTeachableLearnset(species);
        for (i = 0; learnset != NULL && learnset[i] != MOVE_UNAVAILABLE; i++)
            TryAddAttack(species, learnset[i], best, bestScore);
    }
    for (count = 0; count < MAX_MON_MOVES; count++)
    {
        u32 top = NUMBER_OF_MON_TYPES;
        for (i = 0; i < NUMBER_OF_MON_TYPES; i++)
        {
            if (best[i] != MOVE_NONE && (top == NUMBER_OF_MON_TYPES || bestScore[i] > bestScore[top]))
                top = i;
        }
        if (top == NUMBER_OF_MON_TYPES)
            break;
        chosen[count] = best[top];
        best[top] = MOVE_NONE;
    }
    // keep known moves that were not chosen in the free slots
    for (i = 0; i < MAX_MON_MOVES && count < MAX_MON_MOVES; i++)
    {
        enum Move known = GetMonData(mon, MON_DATA_MOVE1 + i);
        bool32 dup = (known == MOVE_NONE);
        for (j = 0; j < count; j++)
        {
            if (chosen[j] == known)
                dup = TRUE;
        }
        if (!dup)
            chosen[count++] = known;
    }
    for (i = 0; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(mon, chosen[i], i);
}

static void CreateLeagueMon(struct Pokemon *mon, enum Species species, u32 level, bool32 teachable)
{
    CreateMon(mon, species, min(level, MAX_LEVEL), Random32(), OTID_STRUCT_RANDOM_NO_SHINY);
    GiveLeagueMoveset(mon, teachable);
}

static bool32 BuildLeagueParty(struct Pokemon *party, u16 trainerNum)
{
    const struct LeagueMember *member = GetLeagueMember(trainerNum);
    enum GangDifficulty difficulty;
    bool32 champion, teachable;
    u32 size, level, i, slot = 0;
    u16 taken[PARTY_SIZE];
    enum Species ace;

    if (member == NULL)
        return FALSE;
    difficulty = GetLeagueDifficulty();
    champion = (member->room == LEAGUE_ROOM_CHAMPION);
    teachable = (difficulty >= GANG_DIFFICULTY_ELITE_GANGSTER);
    size = champion ? sChampionPartySize[difficulty] : sLeaguePartySize[difficulty];
    level = FlagGet(FLAG_IS_CHAMPION) ? LEAGUE_REMATCH_LEVEL : LEAGUE_LEVEL;
    level = (champion ? level : level - 5 + member->room) + sLeagueLevelBonus[difficulty];

    ace = member->ace;
    if (ace == SPECIES_NONE) // Silver: his own random partner, fully evolved
    {
        ace = GangStory_GetRivalStarter();
        ace = (ace != SPECIES_NONE && IsSpeciesEnabled(ace)) ? EvolveForLevel(GetFirstStage(ace), MAX_LEVEL, SALT_RIVAL) : SPECIES_CHARIZARD;
    }
    for (i = 0; i < PARTY_SIZE; i++)
        ZeroMonData(&party[i]);

    // the others, weakest first
    for (i = 0; i + 1 < size; i++)
    {
        enum Species species;
        if (difficulty == GANG_DIFFICULTY_BOSS && i + 2 == size)
            species = member->legend;
        else
            species = PickLeagueSpecies(member, i, sLeagueMinBst[difficulty], taken, slot);
        taken[slot++] = species;
        CreateLeagueMon(&party[i], species, level - (size - 1 - i) / 2, teachable);
    }
    // the partner Pokémon closes the team
    CreateLeagueMon(&party[size - 1], ace, level + (champion ? 2 : 1), teachable);
    return TRUE;
}

// Held items and the bag: a Sitrus Berry each from PETTY CROOK, fitting items from ELITE GANGSTER on
// (BOSS already gets them from the difficulty), and 1-4 Full Restores
void GangRandom_FinishLeagueParty(struct Pokemon *party, u16 trainerNum)
{
    enum GangDifficulty difficulty = GetLeagueDifficulty();
    u32 i;

    if (TESTING || GetLeagueMember(trainerNum) == NULL || difficulty == GANG_DIFFICULTY_RECRUIT)
        return;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 item;
        if (GetMonData(&party[i], MON_DATA_SPECIES) == SPECIES_NONE || GetMonData(&party[i], MON_DATA_HELD_ITEM) != ITEM_NONE)
            continue;
        item = (difficulty == GANG_DIFFICULTY_PETTY_CROOK) ? ITEM_SITRUS_BERRY : GangDifficulty_GetFittingItem(&party[i]);
        SetMonData(&party[i], MON_DATA_HELD_ITEM, &item);
    }
}

u32 GangRandom_GetLeagueFullRestores(u16 trainerNum)
{
    if (TESTING || GetLeagueMember(trainerNum) == NULL)
        return 0;
    return sLeagueFullRestores[GetLeagueDifficulty()];
}

void GangRandom_RandomizeTrainerParty(struct Pokemon *party, u16 trainerNum)
{
    u32 i, trainerClass;

    if (TESTING || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_RECORDED)))
        return;
    if (GangRandom_IsBossTrainer(trainerNum) || BuildLeagueParty(party, trainerNum))
        return;
    trainerClass = GetTrainerClassFromId(trainerNum);
    if (trainerNum != TRAINER_RIVAL_OAKS_LAB_SQUIRTLE && trainerNum != TRAINER_RIVAL_OAKS_LAB_BULBASAUR
     && trainerNum != TRAINER_RIVAL_OAKS_LAB_CHARMANDER)
        ScaleTrainerParty(party, IsRivalClass(trainerClass));
    for (i = 0; i < PARTY_SIZE; i++)
    {
        enum Species original = GetMonData(&party[i], MON_DATA_SPECIES);
        enum Species species;
        u8 level;

        if (original == SPECIES_NONE || GetMonData(&party[i], MON_DATA_IS_EGG))
            continue;
        level = GetMonData(&party[i], MON_DATA_LEVEL);
        // Silver's partner: the random Pokémon he picked in front of Giovanni, evolved to fit the level
        if (IsRivalClass(trainerClass) && IsRivalStarterLine(original))
        {
            enum Species rival = GangStory_GetRivalStarter();
            if (rival != SPECIES_NONE && IsSpeciesEnabled(rival))
            {
                ChangeSpecies(&party[i], EvolveForLevel(rival, level, SALT_RIVAL));
                if (level <= 10)
                    GangStory_GiveFullMoveset(&party[i]);
            }
            continue;
        }
        species = PickSpecies(level, Hash(SALT_TRAINER, trainerNum, i, 0), trainerNum);
        if (species != SPECIES_NONE)
            ChangeSpecies(&party[i], species);
    }
    FillUpTrainerParty(party, trainerNum);
}
