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

// Gangster Outbreak: realistic shop prices (buying and selling) for everything the vending machines, SHOP-BOTs and
// loot bags hand out: the classic Gen 3 prices for balls, medicine and X items, 2100-3000 for evolution items and
// held battle items staggered by strength (3000-25000) instead of the modern 10000-100000. 0 = use the item's own price.
u32 GangItems_GetPrice(enum Item itemId)
{
    if (TESTING)
        return 0; // the test suite checks the expansion's prices
    switch (itemId)
    {
    case ITEM_POKE_BALL: return 200;
    case ITEM_GREAT_BALL: return 600;
    case ITEM_ULTRA_BALL: return 1200;
    case ITEM_NET_BALL: return 1000;
    case ITEM_DIVE_BALL: return 1000;
    case ITEM_NEST_BALL: return 1000;
    case ITEM_REPEAT_BALL: return 1000;
    case ITEM_TIMER_BALL: return 1000;
    case ITEM_DUSK_BALL: return 1000;
    case ITEM_QUICK_BALL: return 1000;
    case ITEM_LUXURY_BALL: return 1000;
    case ITEM_HEAL_BALL: return 300;
    case ITEM_PREMIER_BALL: return 200;
    case ITEM_POTION: return 300;
    case ITEM_SUPER_POTION: return 700;
    case ITEM_HYPER_POTION: return 1200;
    case ITEM_MAX_POTION: return 2500;
    case ITEM_FULL_RESTORE: return 3000;
    case ITEM_FULL_HEAL: return 600;
    case ITEM_REVIVE: return 1500;
    case ITEM_MAX_REVIVE: return 4000;
    case ITEM_SACRED_ASH: return 8000;
    case ITEM_ETHER: return 1200;
    case ITEM_MAX_ETHER: return 2000;
    case ITEM_ELIXIR: return 3000;
    case ITEM_MAX_ELIXIR: return 4500;
    case ITEM_ANTIDOTE: return 100;
    case ITEM_PARALYZE_HEAL: return 200;
    case ITEM_AWAKENING: return 250;
    case ITEM_BURN_HEAL: return 250;
    case ITEM_ICE_HEAL: return 250;
    case ITEM_FRESH_WATER: return 200;
    case ITEM_SODA_POP: return 300;
    case ITEM_LEMONADE: return 350;
    case ITEM_MOOMOO_MILK: return 500;
    case ITEM_ENERGY_POWDER: return 500;
    case ITEM_ENERGY_ROOT: return 800;
    case ITEM_HEAL_POWDER: return 450;
    case ITEM_REVIVAL_HERB: return 2800;
    case ITEM_BERRY_JUICE: return 100;
    case ITEM_SITRUS_BERRY: return 400;
    case ITEM_LUM_BERRY: return 400;
    case ITEM_REPEL: return 350;
    case ITEM_SUPER_REPEL: return 500;
    case ITEM_MAX_REPEL: return 700;
    case ITEM_X_ATTACK: return 500;
    case ITEM_X_DEFENSE: return 550;
    case ITEM_X_SP_ATK: return 350;
    case ITEM_X_SP_DEF: return 350;
    case ITEM_X_SPEED: return 350;
    case ITEM_X_ACCURACY: return 950;
    case ITEM_DIRE_HIT: return 650;
    case ITEM_GUARD_SPEC: return 700;
    case ITEM_HP_UP: return 9800;
    case ITEM_PROTEIN: return 9800;
    case ITEM_IRON: return 9800;
    case ITEM_CALCIUM: return 9800;
    case ITEM_ZINC: return 9800;
    case ITEM_CARBOS: return 9800;
    case ITEM_PP_UP: return 9800;
    case ITEM_PP_MAX: return 14800;
    case ITEM_RARE_CANDY: return 4800;
    case ITEM_ABILITY_CAPSULE: return 10000;
    case ITEM_BOTTLE_CAP: return 10000;
    case ITEM_FIRE_STONE: return 2100;
    case ITEM_WATER_STONE: return 2100;
    case ITEM_THUNDER_STONE: return 2100;
    case ITEM_LEAF_STONE: return 2100;
    case ITEM_MOON_STONE: return 2100;
    case ITEM_SUN_STONE: return 2100;
    case ITEM_SHINY_STONE: return 2100;
    case ITEM_DUSK_STONE: return 2100;
    case ITEM_DAWN_STONE: return 2100;
    case ITEM_ICE_STONE: return 2100;
    case ITEM_OVAL_STONE: return 2100;
    case ITEM_LINKING_CORD: return 3000;
    case ITEM_KINGS_ROCK: return 3000;
    case ITEM_METAL_COAT: return 3000;
    case ITEM_DRAGON_SCALE: return 3000;
    case ITEM_UPGRADE: return 3000;
    case ITEM_DUBIOUS_DISC: return 3000;
    case ITEM_PROTECTOR: return 3000;
    case ITEM_ELECTIRIZER: return 3000;
    case ITEM_MAGMARIZER: return 3000;
    case ITEM_REAPER_CLOTH: return 3000;
    case ITEM_RAZOR_CLAW: return 3000;
    case ITEM_RAZOR_FANG: return 3000;
    case ITEM_PRISM_SCALE: return 3000;
    case ITEM_SACHET: return 3000;
    case ITEM_WHIPPED_DREAM: return 3000;
    case ITEM_DEEP_SEA_TOOTH: return 2100;
    case ITEM_DEEP_SEA_SCALE: return 2100;
    case ITEM_SWEET_APPLE: return 2200;
    case ITEM_TART_APPLE: return 2200;
    case ITEM_CRACKED_POT: return 2200;
    case ITEM_GALARICA_CUFF: return 3000;
    case ITEM_GALARICA_WREATH: return 3000;
    case ITEM_BLACK_AUGURITE: return 3000;
    case ITEM_PEAT_BLOCK: return 3000;
    case ITEM_AUSPICIOUS_ARMOR: return 3000;
    case ITEM_MALICIOUS_ARMOR: return 3000;
    case ITEM_CHARCOAL: return 3000;
    case ITEM_QUICK_CLAW: return 4000;
    case ITEM_MUSCLE_BAND: return 4000;
    case ITEM_WISE_GLASSES: return 4000;
    case ITEM_SCOPE_LENS: return 5000;
    case ITEM_FOCUS_BAND: return 5000;
    case ITEM_MENTAL_HERB: return 4000;
    case ITEM_WHITE_HERB: return 4000;
    case ITEM_AIR_BALLOON: return 4000;
    case ITEM_BLACK_SLUDGE: return 5000;
    case ITEM_SHELL_BELL: return 6000;
    case ITEM_POWER_HERB: return 6000;
    case ITEM_HEAVY_DUTY_BOOTS: return 8000;
    case ITEM_COVERT_CLOAK: return 8000;
    case ITEM_LEFTOVERS: return 10000;
    case ITEM_EXPERT_BELT: return 10000;
    case ITEM_ROCKY_HELMET: return 10000;
    case ITEM_FOCUS_SASH: return 10000;
    case ITEM_LOADED_DICE: return 10000;
    case ITEM_CLEAR_AMULET: return 10000;
    case ITEM_WEAKNESS_POLICY: return 12000;
    case ITEM_EVIOLITE: return 12000;
    case ITEM_ASSAULT_VEST: return 12000;
    case ITEM_LIFE_ORB: return 20000;
    case ITEM_CHOICE_BAND: return 25000;
    case ITEM_CHOICE_SPECS: return 25000;
    case ITEM_CHOICE_SCARF: return 25000;
    default: return 0;
    }
}
