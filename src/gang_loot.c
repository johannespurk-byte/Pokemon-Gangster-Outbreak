#include "global.h"
#include "gang_random.h"
#include "battle.h"
#include "battle_setup.h"
#include "event_data.h"
#include "list_menu.h"
#include "malloc.h"
#include "overworld.h"
#include "pokedex.h"
#include "pokemon.h"
#include "script_menu.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/pokeball.h"
#include "constants/trainers.h"
#include "data.h"

// Gangster Outbreak: after beating a normal trainer the player may take one Pokémon from the trainer's team.
// Gym leaders, the Elite Four, champions, rivals and Giovanni keep their Pokémon.

#define LOOT_NONE_ID 0xFF

// The enemy party is cleared when a battle ends, so a copy of the trainer teams is kept for the loot choice
// (opponent A first, opponent B behind it in double battles against two trainers)
#define LOOT_SLOTS (PARTY_SIZE * 2)
static EWRAM_DATA struct Pokemon sLootParty[LOOT_SLOTS] = {0};

static const u8 sText_Level[] = _(" Lv");
static const u8 sText_None[] = _("NONE");

static bool32 IsLootTrainer(u16 trainerNum)
{
    if (GangRandom_IsBossTrainer(trainerNum))
        return FALSE;
    switch (GetTrainerClassFromId(trainerNum))
    {
    case TRAINER_CLASS_LEADER:
    case TRAINER_CLASS_LEADER_FRLG:
    case TRAINER_CLASS_ELITE_FOUR:
    case TRAINER_CLASS_ELITE_FOUR_FRLG:
    case TRAINER_CLASS_CHAMPION:
    case TRAINER_CLASS_CHAMPION_FRLG:
    case TRAINER_CLASS_RIVAL:
    case TRAINER_CLASS_RIVAL_EARLY_FRLG:
    case TRAINER_CLASS_RIVAL_LATE_FRLG:
    case TRAINER_CLASS_BOSS_FRLG:
        return FALSE;
    default:
        return TRUE;
    }
}

// Called when a trainer party is created: opponent A starts a new loot list, opponent B is added behind it
void GangLoot_RememberParty(struct Pokemon *party)
{
    if (party == gParties[B_TRAINER_OPPONENT_A])
    {
        memset(sLootParty, 0, sizeof(sLootParty));
        memcpy(sLootParty, party, sizeof(struct Pokemon) * PARTY_SIZE);
    }
    else if (party == gParties[B_TRAINER_OPPONENT_B])
    {
        memcpy(&sLootParty[PARTY_SIZE], party, sizeof(struct Pokemon) * PARTY_SIZE);
    }
}

// Special: fills the dynamic multichoice with the beaten trainer's Pokémon.
// VAR_RESULT = number of Pokémon offered (0 = nothing to take).
void GangLoot_Prepare(void)
{
    u32 i, count = 0;
    u16 trainerNum = TRAINER_BATTLE_PARAM.opponentA;

    gSpecialVar_Result = 0;
    if (gBattleOutcome != B_OUTCOME_WON || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER))
     || trainerNum == 0 || !IsLootTrainer(trainerNum))
        return;

    MultichoiceDynamic_InitStack(LOOT_SLOTS + 1);
    for (i = 0; i < LOOT_SLOTS; i++)
    {
        enum Species species = GetMonData(&sLootParty[i], MON_DATA_SPECIES);
        struct ListMenuItem item;
        u8 *name, *end;

        if (species == SPECIES_NONE || GetMonData(&sLootParty[i], MON_DATA_IS_EGG))
            continue;
        name = Alloc(POKEMON_NAME_LENGTH + 12);
        end = StringCopy(name, GetSpeciesName(species));
        end = StringAppend(end, sText_Level);
        ConvertIntToDecimalStringN(end, GetMonData(&sLootParty[i], MON_DATA_LEVEL), STR_CONV_MODE_LEFT_ALIGN, 3);
        item.name = name;
        item.id = i;
        MultichoiceDynamic_PushElement(item);
        count++;
    }
    if (count == 0)
    {
        MultichoiceDynamic_DestroyStack();
        return;
    }
    {
        struct ListMenuItem none;
        u8 *name = Alloc(16);

        StringCopy(name, sText_None);
        none.name = name;
        none.id = LOOT_NONE_ID;
        MultichoiceDynamic_PushElement(none);
    }
    gSpecialVar_Result = count;
}

// Special: VAR_0x8004 = slot in the enemy party. Gives the Pokémon to the player.
// VAR_RESULT = MON_GIVEN_TO_PARTY / MON_GIVEN_TO_PC / MON_CANT_GIVE, STR_VAR_1 = species name.
void GangLoot_Take(void)
{
    struct Pokemon *mon;
    u32 slot = gSpecialVar_0x8004;
    u8 metLocation = GetCurrentRegionMapSectionId();
    u8 level, ball = BALL_POKE;
    enum Species species;

    gSpecialVar_Result = MON_CANT_GIVE;
    if (slot >= LOOT_SLOTS || GetMonData(&sLootParty[slot], MON_DATA_SPECIES) == SPECIES_NONE)
        return;
    mon = AllocZeroed(sizeof(struct Pokemon));
    if (mon == NULL)
        return;
    CopyMon(mon, &sLootParty[slot], sizeof(*mon));
    species = GetMonData(mon, MON_DATA_SPECIES);
    level = GetMonData(mon, MON_DATA_LEVEL);
    SetMonData(mon, MON_DATA_MET_LOCATION, &metLocation);
    SetMonData(mon, MON_DATA_MET_LEVEL, &level);
    SetMonData(mon, MON_DATA_POKEBALL, &ball);
    HealPokemon(mon);
    gSpecialVar_Result = GiveCapturedMonToPlayer(mon); // also makes the player its trainer, so it obeys
    if (gSpecialVar_Result != MON_CANT_GIVE)
    {
        u32 dexNum = SpeciesToNationalPokedexNum(species);
        GetSetPokedexFlag(dexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(dexNum, FLAG_SET_CAUGHT);
    }
    StringCopy(gStringVar1, GetSpeciesName(species));
    Free(mon);
}
