#include "global.h"
#include "battle.h"
#include "gang_battle_bg.h"
#include "constants/battle.h"
#include "constants/map_groups.h"

// Gangster Outbreak: battle backgrounds of the ruined Kanto (tools/apocalypse/battle_bg.py).
// They replace the backgrounds of the usual battle environments; inside a gym the gang's own arena is used.

static const u32 sGangBg_Street_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/street/tiles.png", ".4bpp.smol");
static const u32 sGangBg_Street_Map[] = INCGFX_U32("graphics/battle_environment/gang/street/map.bin", ".smolTM");
static const u16 sGangBg_Street_Pal[] = INCGFX_U16("graphics/battle_environment/gang/street/palette.pal", ".gbapal");

static const u32 sGangBg_Grass_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/grass/tiles.png", ".4bpp.smol");
static const u32 sGangBg_Grass_Map[] = INCGFX_U32("graphics/battle_environment/gang/grass/map.bin", ".smolTM");
static const u16 sGangBg_Grass_Pal[] = INCGFX_U16("graphics/battle_environment/gang/grass/palette.pal", ".gbapal");

static const u32 sGangBg_Water_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/water/tiles.png", ".4bpp.smol");
static const u32 sGangBg_Water_Map[] = INCGFX_U32("graphics/battle_environment/gang/water/map.bin", ".smolTM");
static const u16 sGangBg_Water_Pal[] = INCGFX_U16("graphics/battle_environment/gang/water/palette.pal", ".gbapal");

static const u32 sGangBg_Cave_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/cave/tiles.png", ".4bpp.smol");
static const u32 sGangBg_Cave_Map[] = INCGFX_U32("graphics/battle_environment/gang/cave/map.bin", ".smolTM");
static const u16 sGangBg_Cave_Pal[] = INCGFX_U16("graphics/battle_environment/gang/cave/palette.pal", ".gbapal");

static const u32 sGangBg_Building_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/building/tiles.png", ".4bpp.smol");
static const u32 sGangBg_Building_Map[] = INCGFX_U32("graphics/battle_environment/gang/building/map.bin", ".smolTM");
static const u16 sGangBg_Building_Pal[] = INCGFX_U16("graphics/battle_environment/gang/building/palette.pal", ".gbapal");

static const u32 sGangBg_GymPewter_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_pewter_city/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymPewter_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_pewter_city/map.bin", ".smolTM");
static const u16 sGangBg_GymPewter_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_pewter_city/palette.pal", ".gbapal");

static const u32 sGangBg_GymCerulean_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_cerulean_city/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymCerulean_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_cerulean_city/map.bin", ".smolTM");
static const u16 sGangBg_GymCerulean_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_cerulean_city/palette.pal", ".gbapal");

static const u32 sGangBg_GymVermilion_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_vermilion_city/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymVermilion_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_vermilion_city/map.bin", ".smolTM");
static const u16 sGangBg_GymVermilion_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_vermilion_city/palette.pal", ".gbapal");

static const u32 sGangBg_GymCeladon_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_celadon_city/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymCeladon_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_celadon_city/map.bin", ".smolTM");
static const u16 sGangBg_GymCeladon_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_celadon_city/palette.pal", ".gbapal");

static const u32 sGangBg_GymFuchsia_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_fuchsia_city/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymFuchsia_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_fuchsia_city/map.bin", ".smolTM");
static const u16 sGangBg_GymFuchsia_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_fuchsia_city/palette.pal", ".gbapal");

static const u32 sGangBg_GymSaffron_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_saffron_city/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymSaffron_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_saffron_city/map.bin", ".smolTM");
static const u16 sGangBg_GymSaffron_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_saffron_city/palette.pal", ".gbapal");

static const u32 sGangBg_GymCinnabar_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_cinnabar_island/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymCinnabar_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_cinnabar_island/map.bin", ".smolTM");
static const u16 sGangBg_GymCinnabar_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_cinnabar_island/palette.pal", ".gbapal");

static const u32 sGangBg_GymViridian_Tiles[] = INCGFX_U32("graphics/battle_environment/gang/gym_viridian_city/tiles.png", ".4bpp.smol");
static const u32 sGangBg_GymViridian_Map[] = INCGFX_U32("graphics/battle_environment/gang/gym_viridian_city/map.bin", ".smolTM");
static const u16 sGangBg_GymViridian_Pal[] = INCGFX_U16("graphics/battle_environment/gang/gym_viridian_city/palette.pal", ".gbapal");

static const u32 sGangBgEntryTiles[] = INCGFX_U32("graphics/battle_environment/gang/entry_tiles.png", ".4bpp.smol");
static const u32 sGangBgEntryMap[] = INCGFX_U32("graphics/battle_environment/gang/entry_map.bin", ".smolTM");

#define SCENE(name) {sGangBg_##name##_Tiles, sGangBg_##name##_Map, sGangBg_##name##_Pal}

static const struct GangBattleScene sGangScenes[] =
{
    SCENE(Street), SCENE(Grass), SCENE(Water), SCENE(Cave), SCENE(Building),
    SCENE(GymPewter), SCENE(GymCerulean), SCENE(GymVermilion), SCENE(GymCeladon),
    SCENE(GymFuchsia), SCENE(GymSaffron), SCENE(GymCinnabar), SCENE(GymViridian),
};

static const struct GangBattleScene sGangEntry = {sGangBgEntryTiles, sGangBgEntryMap, NULL};

enum
{
    SCENE_STREET,
    SCENE_GRASS,
    SCENE_WATER,
    SCENE_CAVE,
    SCENE_BUILDING,
    SCENE_GYM_PEWTER,
};

static s32 GetGymScene(void)
{
#if IS_FRLG
    static const u16 sGyms[] =
    {
        MAP_PEWTER_CITY_GYM, MAP_CERULEAN_CITY_GYM, MAP_VERMILION_CITY_GYM, MAP_CELADON_CITY_GYM,
        MAP_FUCHSIA_CITY_GYM, MAP_SAFFRON_CITY_GYM, MAP_CINNABAR_ISLAND_GYM, MAP_VIRIDIAN_CITY_GYM,
    };
    u16 map = gSaveBlock1Ptr->location.mapNum | (gSaveBlock1Ptr->location.mapGroup << 8);
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sGyms); i++)
    {
        if (sGyms[i] == map)
            return SCENE_GYM_PEWTER + i;
    }
#endif
    return -1;
}

const struct GangBattleScene *GangBattleBg_GetScene(u32 environment)
{
    s32 gym;

    if (TESTING || (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_RECORDED_LINK | BATTLE_TYPE_FRONTIER)))
        return NULL;
    gym = GetGymScene();
    if (gym >= 0)
        return &sGangScenes[gym];

    switch (environment)
    {
    case BATTLE_ENVIRONMENT_GRASS:
    case BATTLE_ENVIRONMENT_LONG_GRASS:
    case BATTLE_ENVIRONMENT_MARSH:
    case BATTLE_ENVIRONMENT_SWAMP:
        return &sGangScenes[SCENE_GRASS];
    case BATTLE_ENVIRONMENT_WATER:
    case BATTLE_ENVIRONMENT_POND:
    case BATTLE_ENVIRONMENT_UNDERWATER:
    case BATTLE_ENVIRONMENT_PUDDLE:
    case BATTLE_ENVIRONMENT_KYOGRE:
        return &sGangScenes[SCENE_WATER];
    case BATTLE_ENVIRONMENT_CAVE:
    case BATTLE_ENVIRONMENT_GROUDON:
    case BATTLE_ENVIRONMENT_BURIAL_GROUND:
        return &sGangScenes[SCENE_CAVE];
    case BATTLE_ENVIRONMENT_BUILDING:
    case BATTLE_ENVIRONMENT_GYM:
    case BATTLE_ENVIRONMENT_LEADER:
    case BATTLE_ENVIRONMENT_CHAMPION:
    case BATTLE_ENVIRONMENT_MAGMA:
    case BATTLE_ENVIRONMENT_AQUA:
    case BATTLE_ENVIRONMENT_SIDNEY:
    case BATTLE_ENVIRONMENT_PHOEBE:
    case BATTLE_ENVIRONMENT_GLACIA:
    case BATTLE_ENVIRONMENT_DRAKE:
        return &sGangScenes[SCENE_BUILDING];
    default:
        return &sGangScenes[SCENE_STREET];
    }
}

const struct GangBattleScene *GangBattleBg_GetEntry(u32 environment)
{
    return GangBattleBg_GetScene(environment) != NULL ? &sGangEntry : NULL;
}
