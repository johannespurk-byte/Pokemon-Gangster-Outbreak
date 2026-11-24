#include "global.h"
#include "event_data.h"
#include "gang_random.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/event_objects.h"
#include "item.h"

// Gangster Outbreak: the Poké Vial and the Auto-Repel hold 1 charge plus one per Gym Badge.
// HEAL-BOTs (the Pokémon Center nurses) refill them.

static u32 CountBadges(void)
{
    u32 flag, count = 0;

    for (flag = FLAG_BADGE01_GET; flag <= FLAG_BADGE08_GET; flag++)
    {
        if (FlagGet(flag))
            count++;
    }
    return count;
}

static u32 GetMaxCharges(void)
{
    return 1 + CountBadges();
}

// Special for the shared mart stock (data/scripts/gang_mart.inc): VAR_RESULT = number of Gym Badges
void GangItems_CountBadges(void)
{
    gSpecialVar_Result = CountBadges();
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

// ---------------------------------------------------------------------------- loot bags
// Every item ball on the maps is a loot bag with random contents: fixed per save file and bag, but the quality
// is rolled when the bag is opened, from the Gym Badges the player has by then. Key items and HMs stay as they
// are, TMs become a random TM.

struct LootEntry
{
    u16 item;
    u8 minBadges;
    u8 maxBadges;
    u8 weight;
    u8 maxAmount;
};

static const struct LootEntry sLootTable[] =
{
    // healing
    {ITEM_POTION,          0, 2, 12, 3},
    {ITEM_SUPER_POTION,    0, 5, 10, 3},
    {ITEM_HYPER_POTION,    3, 8, 10, 2},
    {ITEM_MAX_POTION,      6, 8,  6, 2},
    {ITEM_FULL_RESTORE,    6, 8,  5, 1},
    {ITEM_FRESH_WATER,     0, 4,  6, 3},
    {ITEM_SODA_POP,        1, 5,  5, 3},
    {ITEM_MOOMOO_MILK,     3, 8,  5, 3},
    {ITEM_ORAN_BERRY,      0, 3,  6, 3},
    {ITEM_SITRUS_BERRY,    2, 8,  6, 2},
    {ITEM_LUM_BERRY,       3, 8,  4, 2},
    {ITEM_ANTIDOTE,        0, 3,  6, 3},
    {ITEM_PARALYZE_HEAL,   0, 3,  6, 3},
    {ITEM_AWAKENING,       0, 3,  4, 2},
    {ITEM_BURN_HEAL,       0, 3,  4, 2},
    {ITEM_FULL_HEAL,       2, 8,  8, 2},
    {ITEM_REVIVE,          1, 8,  8, 2},
    {ITEM_MAX_REVIVE,      5, 8,  4, 1},
    {ITEM_ETHER,           0, 5,  5, 2},
    {ITEM_MAX_ETHER,       4, 8,  4, 1},
    {ITEM_ELIXIR,          4, 8,  3, 1},
    {ITEM_MAX_ELIXIR,      6, 8,  2, 1},
    // balls
    {ITEM_POKE_BALL,       0, 2, 12, 5},
    {ITEM_GREAT_BALL,      1, 6, 10, 4},
    {ITEM_ULTRA_BALL,      4, 8, 10, 4},
    {ITEM_QUICK_BALL,      2, 8,  4, 2},
    {ITEM_DUSK_BALL,       2, 8,  4, 2},
    {ITEM_TIMER_BALL,      4, 8,  3, 2},
    // field and battle items
    {ITEM_REPEL,           0, 3,  4, 2},
    {ITEM_SUPER_REPEL,     2, 6,  3, 2},
    {ITEM_MAX_REPEL,       5, 8,  3, 2},
    {ITEM_ESCAPE_ROPE,     0, 8,  3, 1},
    {ITEM_X_ATTACK,        0, 8,  3, 2},
    {ITEM_X_DEFENSE,       0, 8,  3, 2},
    {ITEM_X_SPEED,         0, 8,  3, 2},
    {ITEM_X_SP_ATK,        0, 8,  3, 2},
    // growth
    {ITEM_RARE_CANDY,      1, 8,  6, 2},
    {ITEM_PP_UP,           2, 8,  3, 1},
    {ITEM_PP_MAX,          6, 8,  1, 1},
    {ITEM_HP_UP,           2, 8,  2, 1},
    {ITEM_PROTEIN,         2, 8,  2, 1},
    {ITEM_IRON,            2, 8,  2, 1},
    {ITEM_CALCIUM,         2, 8,  2, 1},
    {ITEM_ZINC,            2, 8,  2, 1},
    {ITEM_CARBOS,          2, 8,  2, 1},
    // evolution
    {ITEM_MOON_STONE,      0, 8,  2, 1},
    {ITEM_FIRE_STONE,      1, 8,  2, 1},
    {ITEM_WATER_STONE,     1, 8,  2, 1},
    {ITEM_THUNDER_STONE,   1, 8,  2, 1},
    {ITEM_LEAF_STONE,      1, 8,  2, 1},
    {ITEM_SUN_STONE,       3, 8,  1, 1},
    {ITEM_SHINY_STONE,     4, 8,  1, 1},
    {ITEM_DUSK_STONE,      4, 8,  1, 1},
    {ITEM_DAWN_STONE,      4, 8,  1, 1},
    {ITEM_ICE_STONE,       4, 8,  1, 1},
    {ITEM_LINKING_CORD,    3, 8,  1, 1},
    // loot to sell
    {ITEM_NUGGET,          1, 8,  4, 1},
    {ITEM_BIG_NUGGET,      5, 8,  2, 1},
    // held items
    {ITEM_QUICK_CLAW,      1, 8,  1, 1},
    {ITEM_SCOPE_LENS,      2, 8,  1, 1},
    {ITEM_KINGS_ROCK,      2, 8,  1, 1},
    {ITEM_BLACK_BELT,      1, 8,  1, 1},
    {ITEM_CHARCOAL,        1, 8,  1, 1},
    {ITEM_MYSTIC_WATER,    1, 8,  1, 1},
    {ITEM_MAGNET,          1, 8,  1, 1},
    {ITEM_MIRACLE_SEED,    1, 8,  1, 1},
    {ITEM_BLACK_GLASSES,   1, 8,  1, 1},
    {ITEM_POISON_BARB,     1, 8,  1, 1},
    {ITEM_SOFT_SAND,       1, 8,  1, 1},
    {ITEM_MUSCLE_BAND,     2, 8,  1, 1},
    {ITEM_WISE_GLASSES,    2, 8,  1, 1},
    {ITEM_LEFTOVERS,       3, 8,  1, 1},
    {ITEM_EXPERT_BELT,     4, 8,  1, 1},
    {ITEM_EVIOLITE,        3, 8,  1, 1},
    {ITEM_ROCKY_HELMET,    4, 8,  1, 1},
    {ITEM_FOCUS_SASH,      4, 8,  1, 1},
    {ITEM_LIFE_ORB,        5, 8,  1, 1},
    {ITEM_CHOICE_BAND,     6, 8,  1, 1},
    {ITEM_CHOICE_SPECS,    6, 8,  1, 1},
    {ITEM_CHOICE_SCARF,    6, 8,  1, 1},
    {ITEM_ASSAULT_VEST,    6, 8,  1, 1},
};

#define LOOT_SALT 0x4C4F4F54 // "LOOT"
#define LOOT_TM_CHANCE 12    // percent of ordinary bags that hold a TM

static bool32 IsLootBag(u16 localId)
{
    u32 i;

    for (i = 0; i < gMapHeader.events->objectEventCount; i++)
    {
        if (gMapHeader.events->objectEvents[i].localId == localId)
            return gMapHeader.events->objectEvents[i].graphicsId == OBJ_EVENT_GFX_ITEM_BALL;
    }
    return FALSE;
}

// callnative at the start of Std_FindItem: VAR_0x8000 = item, VAR_0x8001 = amount
void GangItems_RandomizeFoundItem(void)
{
    u32 item = gSpecialVar_0x8000;
    u32 hash, badges, total, roll, i;
    u32 tmIndex;

    if (item == ITEM_NONE || !IsLootBag(gSpecialVar_LastTalked) || GetItemPocket(item) == POCKET_KEY_ITEMS)
        return;
    tmIndex = GetItemTMHMIndex(item);
    if (tmIndex > NUM_TECHNICAL_MACHINES)
        return; // HMs stay where the story needs them

    hash = GangRandom_Hash(LOOT_SALT, (gSaveBlock1Ptr->location.mapGroup << 8) | gSaveBlock1Ptr->location.mapNum,
                           gSpecialVar_LastTalked, 0);
    if (tmIndex != 0 || hash % 100 < LOOT_TM_CHANCE)
    {
        gSpecialVar_0x8000 = GetTMHMItemId(1 + (hash >> 8) % NUM_TECHNICAL_MACHINES);
        gSpecialVar_0x8001 = 1;
        return;
    }

    badges = CountBadges();
    total = 0;
    for (i = 0; i < ARRAY_COUNT(sLootTable); i++)
    {
        if (badges >= sLootTable[i].minBadges && badges <= sLootTable[i].maxBadges)
            total += sLootTable[i].weight;
    }
    roll = (hash >> 8) % total;
    for (i = 0; i < ARRAY_COUNT(sLootTable); i++)
    {
        if (badges < sLootTable[i].minBadges || badges > sLootTable[i].maxBadges)
            continue;
        if (roll < sLootTable[i].weight)
            break;
        roll -= sLootTable[i].weight;
    }
    gSpecialVar_0x8000 = sLootTable[i].item;
    gSpecialVar_0x8001 = 1 + (hash >> 24) % sLootTable[i].maxAmount;
}
