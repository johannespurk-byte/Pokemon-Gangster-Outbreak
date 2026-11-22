#include "global.h"
#include "battle_util.h"
#include "event_data.h"
#include "gang_random.h"
#include "list_menu.h"
#include "malloc.h"
#include "script_menu.h"
#include "string_util.h"
#include "battle_main.h"
#include "move.h"
#include "data.h"
#include "pokemon.h"
#include "random.h"
#include "script_pokemon_util.h"
#include "constants/items.h"
#include "constants/pokedex.h"
#include "constants/species.h"
#include "constants/vars.h"
#include "constants/characters.h"

// Gangster Outbreak story helpers (called from scripts via specials).

#define RANDOM_STARTER_LEVEL   5
#define RANDOM_STARTER_TRIES   2000
#define MAX_STARTER_DEX_NUM    NATIONAL_DEX_COUNT // Gen 1-9

static bool32 CanSpeciesEvolve(enum Species species)
{
    const struct Evolution *evolutions = GetSpeciesEvolutions(species);
    return evolutions != NULL && evolutions[0].method != EVOLUTIONS_END;
}

// A first-stage Pokémon from Gen 1-9 that can evolve, no legendaries or special forms
static bool32 IsValidRandomStarter(enum Species species)
{
    const struct SpeciesInfo *info;
    u32 dexNum;

    if (!IsSpeciesEnabled(species))
        return FALSE;
    dexNum = SpeciesToNationalPokedexNum(species);
    if (dexNum == 0 || dexNum > MAX_STARTER_DEX_NUM)
        return FALSE;
    info = &gSpeciesInfo[species];
    if (info->isRestrictedLegendary || info->isSubLegendary || info->isMythical || info->isUltraBeast || info->isParadox
     || info->isTotem || info->isMegaEvolution || info->isPrimalReversion || info->isUltraBurst || info->isGigantamax
     || info->isTeraForm)
        return FALSE;
    return GetSpeciesPreEvolution(species) == SPECIES_NONE && CanSpeciesEvolve(species);
}

static enum Species PickRandomStarter(void)
{
    u32 i;

    for (i = 0; i < RANDOM_STARTER_TRIES; i++)
    {
        enum Species species = 1 + Random() % (NUM_SPECIES - 1);
        if (IsValidRandomStarter(species))
            return species;
    }
    return SPECIES_RATTATA;
}

// How hard an attacking type hits the species (UQ 4.12, 0x1000 = neutral)
static u32 GetMatchup(enum Type attackType, enum Species species)
{
    enum Type type1 = GetSpeciesType(species, 0);
    enum Type type2 = GetSpeciesType(species, 1);
    u32 modifier = GetTypeModifier(attackType, type1);

    if (type2 != type1)
        modifier = modifier * GetTypeModifier(attackType, type2) / 0x1000;
    return modifier;
}

// --- starter choice ------------------------------------------------------------------------------------------

#define STARTER_CHOICES      3
#define RIVAL_CANDIDATES     8

static EWRAM_DATA enum Species sStarterChoices[STARTER_CHOICES] = {0};

static bool32 KnowsMove(struct Pokemon *mon, enum Move move)
{
    u32 i;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (GetMonData(mon, MON_DATA_MOVE1 + i) == move)
            return TRUE;
    }
    return FALSE;
}

static u32 CountMoves(struct Pokemon *mon)
{
    u32 i, n = 0;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (GetMonData(mon, MON_DATA_MOVE1 + i) != MOVE_NONE)
            n++;
    }
    return n;
}

// Fills the free move slots: first with the next moves it would learn by level, then with moves it can be
// taught (attacks of its own types first, then any attack, then anything)
void GangStory_GiveFullMoveset(struct Pokemon *mon)
{
    enum Species species = GetMonData(mon, MON_DATA_SPECIES);
    const struct LevelUpMove *levelUp = GetSpeciesLevelUpLearnset(species);
    const u16 *teachable = GetSpeciesTeachableLearnset(species);
    u32 level = GetMonData(mon, MON_DATA_LEVEL);
    u32 i, pass;

    for (i = 0; levelUp != NULL && levelUp[i].move != LEVEL_UP_MOVE_END && CountMoves(mon) < MAX_MON_MOVES; i++)
    {
        if (levelUp[i].level > level && !KnowsMove(mon, levelUp[i].move))
            GiveMoveToMon(mon, levelUp[i].move);
    }
    for (pass = 0; pass < 3 && teachable != NULL && CountMoves(mon) < MAX_MON_MOVES; pass++)
    {
        for (i = 0; teachable[i] != MOVE_UNAVAILABLE && CountMoves(mon) < MAX_MON_MOVES; i++)
        {
            enum Move move = teachable[i];
            enum Type type = GetMoveType(move);
            bool32 stab = type == GetSpeciesType(species, 0) || type == GetSpeciesType(species, 1);

            if (KnowsMove(mon, move))
                continue;
            if ((pass == 0 && GetMovePower(move) > 0 && stab) || (pass == 1 && GetMovePower(move) > 0) || pass == 2)
                GiveMoveToMon(mon, move);
        }
    }
}

static void SetRivalStarter(enum Species species)
{
    gSaveBlock2Ptr->pokedex.gangsterRivalStarter[0] = species;
    gSaveBlock2Ptr->pokedex.gangsterRivalStarter[1] = species >> 8;
}

enum Species GangStory_GetRivalStarter(void)
{
    const u8 *s = gSaveBlock2Ptr->pokedex.gangsterRivalStarter;
    return s[0] | (s[1] << 8);
}

// Special: Giovanni offers three random Pokémon. Fills the dynamic multichoice with "NAME  TYPE/TYPE".
void GangIntro_PrepareStarters(void)
{
    u32 i, j;

    for (i = 0; i < STARTER_CHOICES; i++)
    {
        enum Species species;
        do
        {
            species = PickRandomStarter();
            for (j = 0; j < i && sStarterChoices[j] != species; j++)
                ;
        } while (j < i);
        sStarterChoices[i] = species;
    }
    MultichoiceDynamic_InitStack(STARTER_CHOICES);
    for (i = 0; i < STARTER_CHOICES; i++)
    {
        struct ListMenuItem item;
        enum Species species = sStarterChoices[i];
        enum Type type1 = GetSpeciesType(species, 0), type2 = GetSpeciesType(species, 1);
        u8 *name = Alloc(POKEMON_NAME_LENGTH + 2 * TYPE_NAME_LENGTH + 4);
        u8 *end = StringCopy(name, GetSpeciesName(species));

        *end++ = CHAR_SPACE;
        end = StringCopy(end, gTypesInfo[type1].name);
        if (type2 != type1)
        {
            *end++ = CHAR_SLASH;
            end = StringCopy(end, gTypesInfo[type2].name);
        }
        item.name = name;
        item.id = i;
        MultichoiceDynamic_PushElement(item);
    }
}

// Special: VAR_0x8004 = chosen entry. Gives that Pokémon (with four moves); Silver grabs another random
// Pokémon, the one of a few candidates that hits the player's choice hardest.
// Out: VAR_0x8004 = player's species, VAR_0x8005 = Silver's species, VAR_STARTER_MON = 0 (his trainer party)
void GangIntro_GiveChosenStarter(void)
{
    enum Species species = sStarterChoices[gSpecialVar_0x8004 < STARTER_CHOICES ? gSpecialVar_0x8004 : 0];
    enum Species rival = SPECIES_NONE;
    u32 i, bestMatchup = 0;

    if (species == SPECIES_NONE)
        species = PickRandomStarter();
    ScriptGiveMon(species, RANDOM_STARTER_LEVEL, ITEM_NONE);
    GangStory_GiveFullMoveset(&gParties[B_TRAINER_PLAYER][CalculatePlayerPartyCount() - 1]);
    for (i = 0; i < RIVAL_CANDIDATES; i++)
    {
        enum Species candidate = PickRandomStarter();
        u32 matchup = max(GetMatchup(GetSpeciesType(candidate, 0), species), GetMatchup(GetSpeciesType(candidate, 1), species));

        if (candidate == species)
            continue;
        if (rival == SPECIES_NONE || matchup > bestMatchup)
        {
            rival = candidate;
            bestMatchup = matchup;
        }
    }
    if (rival == SPECIES_NONE)
        rival = SPECIES_RATTATA;
    SetRivalStarter(rival);
    gSpecialVar_0x8004 = species;
    gSpecialVar_0x8005 = rival;
    VarSet(VAR_STARTER_MON, 0);
}
