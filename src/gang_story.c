#include "global.h"
#include "battle_util.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "script_pokemon_util.h"
#include "constants/items.h"
#include "constants/pokedex.h"
#include "constants/species.h"
#include "constants/vars.h"

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

// Giovanni hands the new recruit a random Pokémon; Silver grabs the Kanto starter with the best matchup.
// Out: VAR_0x8004 = player's species, VAR_STARTER_MON = rival starter (0 Charmander, 1 Bulbasaur, 2 Squirtle),
//      VAR_0x8005 = rival's species.
void GangIntro_GiveRandomStarter(void)
{
    static const struct { enum Species species; enum Type type; u8 starterVar; } sRivalChoices[] =
    {
        {SPECIES_CHARMANDER, TYPE_FIRE,  0},
        {SPECIES_BULBASAUR,  TYPE_GRASS, 1},
        {SPECIES_SQUIRTLE,   TYPE_WATER, 2},
    };
    enum Species species = PickRandomStarter();
    u32 i, best = Random() % ARRAY_COUNT(sRivalChoices), bestMatchup = 0;

    ScriptGiveMon(species, RANDOM_STARTER_LEVEL, ITEM_NONE);
    for (i = 0; i < ARRAY_COUNT(sRivalChoices); i++)
    {
        u32 matchup = GetMatchup(sRivalChoices[i].type, species);
        if (matchup > bestMatchup)
        {
            bestMatchup = matchup;
            best = i;
        }
    }
    gSpecialVar_0x8004 = species;
    gSpecialVar_0x8005 = sRivalChoices[best].species;
    VarSet(VAR_STARTER_MON, sRivalChoices[best].starterVar);
}
