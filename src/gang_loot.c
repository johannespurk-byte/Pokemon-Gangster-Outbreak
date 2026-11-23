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
#include "pokemon_storage_system.h"
#include "script_menu.h"
#include "string_util.h"
#include "constants/battle.h"
#include "constants/moves.h"
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
    SetMonOriginalTrainerId(mon, gSaveBlock2Ptr->playerTrainerId); // re-encrypts it, otherwise it becomes a Bad Egg
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

// ---------------------------------------------------------------------------------------------------------
// Repair of Pokémon that older versions turned into Bad Eggs when they were taken from a trainer: the player's
// trainer id was written over the trainer's one without re-encrypting the secure data. The data itself is still
// intact, only encrypted with an unknown key (personality ^ old trainer id). The key is found again by its
// checksum plus what is known about such a Pokémon: species (its name is still the unencrypted nickname),
// Poké Ball, met level and experience fitting that level, valid moves.

static const u8 sLootSubstructOffsets[4][24] =
{
    {0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3},
    {1, 1, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2},
    {2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 2, 1, 1},
    {3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0},
};

#define SECURE_WORDS 12
#define MAX_LOW_KEYS 96

static enum Species SpeciesFromNickname(const u8 *nickname)
{
    u32 species, i;

    for (species = 1; species < NUM_SPECIES; species++)
    {
        const u8 *name;
        if (!IsSpeciesEnabled(species))
            continue;
        name = GetSpeciesName(species);
        for (i = 0; i < 10; i++)
        {
            if (nickname[i] != name[i])
                break;
            if (name[i] == EOS)
            {
                i = 10;
                break;
            }
        }
        if (i == 10)
            return species;
    }
    return SPECIES_NONE;
}

// -1 if the decrypted data cannot be the stolen Pokémon, otherwise a score (higher = more plausible)
static s32 ScoreDecryptedLoot(const u32 *dec, const u8 *block, enum Species species)
{
    const u32 *s0 = &dec[3 * block[0]], *s1 = &dec[3 * block[1]], *s3 = &dec[3 * block[3]];
    u32 exp = s0[1] & 0x1FFFFF, ball = (s0[2] >> 16) & 0x3F, metLevel = (s3[0] >> 16) & 0x7F;
    u32 moves[MAX_MON_MOVES] = {s1[0] & 0x7FF, (s1[0] >> 16) & 0x7FF, s1[1] & 0x7FF, (s1[1] >> 16) & 0x7FF};
    enum GrowthRate growth = gSpeciesInfo[species].growthRate;
    s32 score = 0;
    u32 i;

    if ((s0[0] & 0x7FF) != species || ball != BALL_POKE || metLevel == 0 || metLevel >= MAX_LEVEL)
        return -1;
    if (exp < gExperienceTables[growth][metLevel] || exp >= gExperienceTables[growth][metLevel + 1])
        return -1;
    if (moves[0] == MOVE_NONE || ((s3[1] >> 30) & 1)) // no first move / still marked as an egg
        return -1;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (moves[i] >= MOVES_COUNT)
            return -1;
    }
    for (i = 0; i < SECURE_WORDS; i++) // unused high bits are 0 in real data
    {
        if (!(dec[i] >> 31))
            score++;
    }
    return score;
}

static bool32 RepairLootBadEgg(struct BoxPokemon *box)
{
    u32 playerId = gSaveBlock2Ptr->playerTrainerId[0] | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
                 | (gSaveBlock2Ptr->playerTrainerId[2] << 16) | ((u32)gSaveBlock2Ptr->playerTrainerId[3] << 24);
    u16 lowKeys[MAX_LOW_KEYS], need[MAX_LOW_KEYS];
    u32 *raw = box->secure.raw;
    u8 block[4];
    u32 i, k, n = 0, bestKey = 0;
    s32 bestScore = -1;
    enum Species species;

    if (!box->isBadEgg || box->otId != playerId)
        return FALSE;
    species = SpeciesFromNickname(box->nickname);
    if (species == SPECIES_NONE)
        return FALSE;
    for (i = 0; i < 4; i++)
        block[i] = sLootSubstructOffsets[i][box->personality % 24];

    // low half of the key: the species fixes 11 bits, the checksum fixes what the high half has to add up to
    for (k = 0; k < 0x10000 && n < MAX_LOW_KEYS; k++)
    {
        u32 sum = 0;
        if (((raw[3 * block[0]] ^ k) & 0x7FF) != species)
            continue;
        for (i = 0; i < SECURE_WORDS; i++)
            sum += (raw[i] & 0xFFFF) ^ k;
        lowKeys[n] = k;
        need[n] = box->checksum - sum;
        n++;
    }
    for (k = 0; k < 0x10000; k++)
    {
        u32 sum = 0, j;
        for (i = 0; i < SECURE_WORDS; i++)
            sum += (raw[i] >> 16) ^ k;
        for (j = 0; j < n; j++)
        {
            if ((u16)sum == need[j])
            {
                u32 key = (k << 16) | lowKeys[j], dec[SECURE_WORDS];
                s32 score;
                for (i = 0; i < SECURE_WORDS; i++)
                    dec[i] = raw[i] ^ key;
                score = ScoreDecryptedLoot(dec, block, species);
                if (score > bestScore)
                {
                    bestScore = score;
                    bestKey = key;
                }
            }
        }
    }
    if (bestScore < 0)
        return FALSE;
    for (i = 0; i < SECURE_WORDS; i++)
        raw[i] = raw[i] ^ bestKey ^ box->personality ^ box->otId;
    box->isBadEgg = FALSE;
    box->isEgg = FALSE;
    box->hasSpecies = TRUE;
    return TRUE;
}

// Called when a save is continued and before healing: repairs such Bad Eggs in the party and the PC boxes
void GangLoot_RepairBadEggs(void)
{
    u32 i, box, pos;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gParties[B_TRAINER_PLAYER][i];
        if (RepairLootBadEgg(&mon->box))
        {
            CalculateMonStats(mon);
            HealPokemon(mon);
        }
    }
    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (pos = 0; pos < IN_BOX_COUNT; pos++)
            RepairLootBadEgg(GetBoxedMonPtr(box, pos));
    }
}
