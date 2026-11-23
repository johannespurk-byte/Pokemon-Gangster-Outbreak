#include "global.h"
#include "event_object_movement.h"
#include "field_weather.h"
#include "palette.h"
#include "player_custom.h"
#include "sprite.h"
#include "data.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"

// Gangster Outbreak character builder.
// Walking/running frames: rows above the outfit's neck line come from the head template,
// the rest from the outfit template. Bike/surf/fish/field move sprites come from the head
// template (their bodies are the same small generic body for everyone). All sprites use the
// standard palette layout from tools/paperdoll/generate.py, recoloured with the player's choices.

struct PlayerPartData
{
    const u8 *name;
    const u32 *gfx;       // 18 frames, 16x32
    u16 palette[16];
    u8 neck[18];
    u8 stateHead;
    bool8 female;
    u16 frontPic;      // enum TrainerPicID
    u16 backPic;       // enum TrainerPicID (heroes' own, villains borrow one)
    bool8 ownBackPic;
    u8 frontRoles[16]; // per palette colour: role << 2 | shade (role 0 = keep)
    u8 backRoles[16];
};

extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_HilbertNormal;
extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_HilbertMachBike;
extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_HilbertSurfing;
extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_HilbertFishing;
extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_HilbertFieldMove;

#include "data/player_custom_parts.h"

#define FRAME_SIZE_16x32 (2 * 4 * TILE_SIZE_4BPP)
#define NUM_WALK_FRAMES  18
#define COMPOSED_MAGIC   0x474E4147 // "GANG"

// standard palette slots
#define SLOT_OUTLINE 1
#define SLOT_SKIN    2  // 2 shades
#define SLOT_HAIR    4  // 3 shades
#define SLOT_HAT     7  // 2 shades
#define SLOT_ACCENT  9
#define SLOT_TOP     10 // 3 shades
#define SLOT_BOTTOM  13 // 2 shades
#define SLOT_WHITE   15

EWRAM_DATA struct ObjectEventGraphicsInfo gPlayerCustomGraphicsInfo[PLAYER_CUSTOM_STATE_COUNT] = {0};
EWRAM_DATA u16 gPlayerCustomPalette[16] = {0};
static EWRAM_DATA u8 sPlayerCustomGfx[NUM_WALK_FRAMES * FRAME_SIZE_16x32] ALIGNED(4) = {0};
static EWRAM_DATA u32 sComposedMagic = 0;
EWRAM_DATA struct TrainerFrontPicInfo gPlayerCustomFrontPicInfo = {0};
EWRAM_DATA struct TrainerBackPicInfo gPlayerCustomBackPicInfo = {0};
static EWRAM_DATA u16 sPlayerCustomFrontPal[16] = {0};
static EWRAM_DATA u16 sPlayerCustomBackPal[16] = {0};
static EWRAM_DATA struct PlayerLook sComposedLook = {0};

static const struct SpriteFrameImage sPicTable_PlayerCustomNormal[] = {
    overworld_ascending_frames(sPlayerCustomGfx, 2, 4),
};

// ---------------------------------------------------------------------------- colour presets
struct ColorPreset
{
    const u8 *name;
    u8 r, g, b;
};

static const struct ColorPreset sHairPresets[] = {
    {COMPOUND_STRING("ORIGINAL"),   0,   0,   0},
    {COMPOUND_STRING("BLACK"),     40,  40,  48},
    {COMPOUND_STRING("BROWN"),    110,  70,  40},
    {COMPOUND_STRING("CHESTNUT"), 150,  90,  50},
    {COMPOUND_STRING("BLONDE"),   232, 200, 110},
    {COMPOUND_STRING("RED"),      200,  50,  40},
    {COMPOUND_STRING("GINGER"),   230, 120,  50},
    {COMPOUND_STRING("SILVER"),   200, 200, 212},
    {COMPOUND_STRING("BLUE"),      60, 110, 210},
    {COMPOUND_STRING("GREEN"),     60, 150,  80},
    {COMPOUND_STRING("PINK"),     240, 130, 180},
    {COMPOUND_STRING("PURPLE"),   140,  80, 190},
};

static const struct ColorPreset sSkinPresets[] = {
    {COMPOUND_STRING("ORIGINAL"),   0,   0,   0},
    {COMPOUND_STRING("LIGHT"),    255, 224, 196},
    {COMPOUND_STRING("MEDIUM"),   240, 192, 150},
    {COMPOUND_STRING("TAN"),      214, 150, 110},
    {COMPOUND_STRING("BROWN"),    160, 100,  70},
    {COMPOUND_STRING("DARK"),     110,  70,  50},
};

static const struct ColorPreset sClothesPresets[] = {
    {COMPOUND_STRING("ORIGINAL"),   0,   0,   0},
    {COMPOUND_STRING("BLACK"),     48,  48,  56},
    {COMPOUND_STRING("WHITE"),    232, 232, 240},
    {COMPOUND_STRING("GRAY"),     140, 140, 150},
    {COMPOUND_STRING("RED"),      200,  40,  40},
    {COMPOUND_STRING("ORANGE"),   240, 130,  40},
    {COMPOUND_STRING("YELLOW"),   240, 210,  60},
    {COMPOUND_STRING("GREEN"),     60, 150,  70},
    {COMPOUND_STRING("TEAL"),      50, 160, 160},
    {COMPOUND_STRING("BLUE"),      50,  90, 200},
    {COMPOUND_STRING("NAVY"),      40,  50, 110},
    {COMPOUND_STRING("PURPLE"),   130,  60, 170},
    {COMPOUND_STRING("PINK"),     235, 120, 170},
    {COMPOUND_STRING("BROWN"),    120,  80,  50},
};

static const struct
{
    const struct ColorPreset *presets;
    u8 count;
    u8 firstSlot;
    u8 numSlots;
} sColorRoles[PLAYER_COLOR_ROLE_COUNT] = {
    [PLAYER_COLOR_HAIR]   = {sHairPresets,    ARRAY_COUNT(sHairPresets),    SLOT_HAIR,   3},
    [PLAYER_COLOR_SKIN]   = {sSkinPresets,    ARRAY_COUNT(sSkinPresets),    SLOT_SKIN,   2},
    [PLAYER_COLOR_HAT]    = {sClothesPresets, ARRAY_COUNT(sClothesPresets), SLOT_HAT,    2},
    [PLAYER_COLOR_TOP]    = {sClothesPresets, ARRAY_COUNT(sClothesPresets), SLOT_TOP,    3},
    [PLAYER_COLOR_BOTTOM] = {sClothesPresets, ARRAY_COUNT(sClothesPresets), SLOT_BOTTOM, 2},
};

static u16 ShadeToGba(u32 r, u32 g, u32 b, s32 shade)
{
    // shade > 0 lightens towards white, shade < 0 darkens (in percent)
    if (shade > 0)
    {
        r += (255 - r) * shade / 100;
        g += (255 - g) * shade / 100;
        b += (255 - b) * shade / 100;
    }
    else if (shade < 0)
    {
        r = r * (100 + shade) / 100;
        g = g * (100 + shade) / 100;
        b = b * (100 + shade) / 100;
    }
    return RGB(r >> 3, g >> 3, b >> 3);
}

static void ApplyColorPreset(u16 *pal, enum PlayerCustomColorRole role, u8 preset)
{
    static const s8 sShades3[] = {35, 0, -40};
    static const s8 sShades2[] = {0, -35};
    const struct ColorPreset *p;
    u32 i;

    if (preset == 0 || preset >= sColorRoles[role].count)
        return;
    p = &sColorRoles[role].presets[preset];
    for (i = 0; i < sColorRoles[role].numSlots; i++)
    {
        s32 shade = sColorRoles[role].numSlots == 3 ? sShades3[i] : sShades2[i];
        pal[sColorRoles[role].firstSlot + i] = ShadeToGba(p->r, p->g, p->b, shade);
    }
}

// ---------------------------------------------------------------------------- composing
static u8 ClampPart(u8 part)
{
    return part < PLAYER_PART_COUNT ? part : PLAYER_PART_RED;
}

static void ComposeWalkFrames(u8 head, u8 outfit, u8 *dest)
{
    const u32 *headGfx = sPlayerParts[head].gfx;
    const u32 *bodyGfx = sPlayerParts[outfit].gfx;
    u32 *dst = (u32 *)dest;
    u32 frame, ty, tx, row;

    // 16x32 frame = 2x4 tiles in row-major order; one tile row of a 4bpp tile is one u32
    for (frame = 0; frame < NUM_WALK_FRAMES; frame++)
    {
        u32 neck = sPlayerParts[outfit].neck[frame];
        for (ty = 0; ty < 4; ty++)
        {
            for (tx = 0; tx < 2; tx++)
            {
                for (row = 0; row < 8; row++)
                {
                    u32 i = (frame * FRAME_SIZE_16x32 + (ty * 2 + tx) * TILE_SIZE_4BPP) / 4 + row;
                    dst[i] = (ty * 8 + row < neck) ? headGfx[i] : bodyGfx[i];
                }
            }
        }
    }
}

static void ComposePalette(const struct PlayerLook *look, u16 *pal)
{
    const u16 *headPal = sPlayerParts[ClampPart(look->head)].palette;
    const u16 *bodyPal = sPlayerParts[ClampPart(look->outfit)].palette;
    u32 i;

    for (i = 0; i < 16; i++)
    {
        bool32 fromHead = (i == SLOT_OUTLINE || i == SLOT_WHITE
                        || (i >= SLOT_SKIN && i < SLOT_ACCENT)); // skin, hair, hat
        pal[i] = fromHead ? headPal[i] : bodyPal[i];
    }
    for (i = 0; i < PLAYER_COLOR_ROLE_COUNT; i++)
        ApplyColorPreset(pal, i, look->colors[i]);
}

void PlayerCustom_BuildPreview(const struct PlayerLook *look, u8 *gfxDest, u16 *palDest)
{
    ComposeWalkFrames(ClampPart(look->head), ClampPart(look->outfit), gfxDest);
    ComposePalette(look, palDest);
}

static void SetupGraphicsInfo(u32 state, const struct ObjectEventGraphicsInfo *base, const struct SpriteFrameImage *images)
{
    gPlayerCustomGraphicsInfo[state] = *base;
    gPlayerCustomGraphicsInfo[state].paletteTag = OBJ_EVENT_PAL_TAG_PLAYER_CUSTOM;
    gPlayerCustomGraphicsInfo[state].images = images;
}

enum { PIC_ROLE_KEEP, PIC_ROLE_SKIN, PIC_ROLE_HAIR, PIC_ROLE_HAT, PIC_ROLE_TOP, PIC_ROLE_BOTTOM, PIC_ROLE_COUNT };
static const u8 sPicRoleFirstSlot[PIC_ROLE_COUNT] = {0, SLOT_SKIN, SLOT_HAIR, SLOT_HAT, SLOT_TOP, SLOT_BOTTOM};

// Recolour a trainer pic palette with the colours of the composed overworld palette
static void RecolorPicPalette(u16 *dest, const u16 *src, const u8 *roles, const bool8 *roleOn)
{
    u32 i;

    for (i = 0; i < 16; i++)
    {
        u32 role = roles[i] >> 2;
        dest[i] = src[i];
        if (role != PIC_ROLE_KEEP && role < PIC_ROLE_COUNT && roleOn[role])
            dest[i] = gPlayerCustomPalette[sPicRoleFirstSlot[role] + (roles[i] & 3)];
    }
}

static void ComposeTrainerPics(const struct PlayerLook *look)
{
    const struct PlayerPartData *outfit = &sPlayerParts[look->outfit];
    const struct PlayerPartData *back;
    bool8 headChanged = (look->head != look->outfit);
    bool8 roleOn[PIC_ROLE_COUNT];

    // front pic: the outfit's trainer, head colours from the chosen head, clothes only if recoloured
    roleOn[PIC_ROLE_KEEP] = FALSE;
    roleOn[PIC_ROLE_SKIN] = headChanged || look->colors[PLAYER_COLOR_SKIN] != 0;
    roleOn[PIC_ROLE_HAIR] = headChanged || look->colors[PLAYER_COLOR_HAIR] != 0;
    roleOn[PIC_ROLE_HAT] = headChanged || look->colors[PLAYER_COLOR_HAT] != 0;
    roleOn[PIC_ROLE_TOP] = look->colors[PLAYER_COLOR_TOP] != 0;
    roleOn[PIC_ROLE_BOTTOM] = look->colors[PLAYER_COLOR_BOTTOM] != 0;
    memcpy(&gPlayerCustomFrontPicInfo, gTrainerPicInfo[outfit->frontPic].frontPic, sizeof(gPlayerCustomFrontPicInfo));
    RecolorPicPalette(sPlayerCustomFrontPal, gTrainerPicInfo[outfit->frontPic].frontPic->paletteData, outfit->frontRoles, roleOn);
    gPlayerCustomFrontPicInfo.paletteData = sPlayerCustomFrontPal;

    // back pic: it mostly shows head and hair, so the HEAD decides which one (cap, hood, bare hair ...);
    // unless it is the template's own back pic, it takes all colours of the composed character
    back = &sPlayerParts[look->head];
    if (headChanged || !back->ownBackPic)
        roleOn[PIC_ROLE_TOP] = roleOn[PIC_ROLE_BOTTOM] = roleOn[PIC_ROLE_SKIN] = roleOn[PIC_ROLE_HAIR] = roleOn[PIC_ROLE_HAT] = TRUE;
    memcpy(&gPlayerCustomBackPicInfo, gTrainerPicInfo[back->backPic].backPic, sizeof(gPlayerCustomBackPicInfo));
    RecolorPicPalette(sPlayerCustomBackPal, gTrainerPicInfo[back->backPic].backPic->paletteData, back->backRoles, roleOn);
    gPlayerCustomBackPicInfo.paletteData = sPlayerCustomBackPal;
}

static void ComposeAll(void)
{
    struct PlayerLook look;
    u8 stateHead;

    PlayerCustom_GetLook(&look);
    look.head = ClampPart(look.head);
    look.outfit = ClampPart(look.outfit);
    stateHead = sPlayerParts[look.head].stateHead;

    ComposeWalkFrames(look.head, look.outfit, sPlayerCustomGfx);
    ComposePalette(&look, gPlayerCustomPalette);
    ComposeTrainerPics(&look);

    SetupGraphicsInfo(PLAYER_CUSTOM_STATE_NORMAL,    &gObjectEventGraphicsInfo_HilbertNormal,    sPicTable_PlayerCustomNormal);
    SetupGraphicsInfo(PLAYER_CUSTOM_STATE_BIKE,      &gObjectEventGraphicsInfo_HilbertMachBike,  sPlayerPartStatePics[stateHead][PLAYER_CUSTOM_STATE_BIKE]);
    SetupGraphicsInfo(PLAYER_CUSTOM_STATE_SURF,      &gObjectEventGraphicsInfo_HilbertSurfing,   sPlayerPartStatePics[stateHead][PLAYER_CUSTOM_STATE_SURF]);
    SetupGraphicsInfo(PLAYER_CUSTOM_STATE_FISH,      &gObjectEventGraphicsInfo_HilbertFishing,   sPlayerPartStatePics[stateHead][PLAYER_CUSTOM_STATE_FISH]);
    SetupGraphicsInfo(PLAYER_CUSTOM_STATE_FIELDMOVE, &gObjectEventGraphicsInfo_HilbertFieldMove, sPlayerPartStatePics[stateHead][PLAYER_CUSTOM_STATE_FIELDMOVE]);

    sComposedLook = look;
    sComposedMagic = COMPOSED_MAGIC;
}

void PlayerCustom_EnsureComposed(void)
{
    struct PlayerLook look;

    PlayerCustom_GetLook(&look);
    if (sComposedMagic != COMPOSED_MAGIC || memcmp(&look, &sComposedLook, sizeof(look)) != 0)
        ComposeAll();
}

void PlayerCustom_RefreshLoadedPalette(void)
{
    u8 index = IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_PLAYER_CUSTOM);

    PlayerCustom_EnsureComposed();
    if (index != 0xFF)
    {
        LoadPalette(gPlayerCustomPalette, OBJ_PLTT_ID(index), PLTT_SIZE_4BPP);
        UpdateSpritePaletteWithWeather(index, FALSE);
    }
}

// TRUE when the player's own trainer pic should be the builder's TRAINER_PIC_PLAYER_CUSTOM
bool32 PlayerCustom_UsesCustomPic(enum Gender gender)
{
    if (!PlayerCustom_IsEnabled() || gender != gSaveBlock2Ptr->playerGender)
        return FALSE;
    PlayerCustom_EnsureComposed();
    return TRUE;
}

// ---------------------------------------------------------------------------- save data
bool32 PlayerCustom_IsEnabled(void)
{
    return gSaveBlock2Ptr->playerLookEnabled;
}

void PlayerCustom_GetLook(struct PlayerLook *look)
{
    look->head = gSaveBlock2Ptr->playerLookHead;
    look->outfit = gSaveBlock2Ptr->playerLookOutfit;
    look->colors[PLAYER_COLOR_HAIR] = gSaveBlock2Ptr->playerLookHair;
    look->colors[PLAYER_COLOR_SKIN] = gSaveBlock2Ptr->playerLookSkin;
    look->colors[PLAYER_COLOR_HAT] = gSaveBlock2Ptr->playerLookHat;
    look->colors[PLAYER_COLOR_TOP] = gSaveBlock2Ptr->playerLookTop;
    look->colors[PLAYER_COLOR_BOTTOM] = gSaveBlock2Ptr->playerLookBottom;
}

void PlayerCustom_SetLook(const struct PlayerLook *look)
{
    gSaveBlock2Ptr->playerLookHead = ClampPart(look->head);
    gSaveBlock2Ptr->playerLookOutfit = ClampPart(look->outfit);
    gSaveBlock2Ptr->playerLookHair = look->colors[PLAYER_COLOR_HAIR];
    gSaveBlock2Ptr->playerLookSkin = look->colors[PLAYER_COLOR_SKIN];
    gSaveBlock2Ptr->playerLookHat = look->colors[PLAYER_COLOR_HAT];
    gSaveBlock2Ptr->playerLookTop = look->colors[PLAYER_COLOR_TOP];
    gSaveBlock2Ptr->playerLookBottom = look->colors[PLAYER_COLOR_BOTTOM];
    gSaveBlock2Ptr->playerLookEnabled = TRUE;
    gSaveBlock2Ptr->playerGender = PlayerCustom_GetGender();
    ComposeAll();
}

enum Gender PlayerCustom_GetGender(void)
{
    return sPlayerParts[ClampPart(gSaveBlock2Ptr->playerLookHead)].female ? FEMALE : MALE;
}

// ---------------------------------------------------------------------------- overworld
bool32 PlayerCustom_IsCustomGraphicsId(u16 graphicsId)
{
    return graphicsId >= OBJ_EVENT_GFX_PLAYER_CUSTOM_NORMAL && graphicsId <= OBJ_EVENT_GFX_PLAYER_CUSTOM_FIELD_MOVE;
}

u16 PlayerCustom_GetGraphicsId(u8 avatarState)
{
    PlayerCustom_EnsureComposed();
    switch (avatarState)
    {
    case PLAYER_AVATAR_STATE_MACH_BIKE:
    case PLAYER_AVATAR_STATE_ACRO_BIKE:
        return OBJ_EVENT_GFX_PLAYER_CUSTOM_BIKE;
    case PLAYER_AVATAR_STATE_SURFING:
    case PLAYER_AVATAR_STATE_UNDERWATER:
        return OBJ_EVENT_GFX_PLAYER_CUSTOM_SURF;
    case PLAYER_AVATAR_STATE_FISHING:
        return OBJ_EVENT_GFX_PLAYER_CUSTOM_FISH;
    case PLAYER_AVATAR_STATE_FIELD_MOVE:
    case PLAYER_AVATAR_STATE_WATERING:
    case PLAYER_AVATAR_STATE_VSSEEKER:
        return OBJ_EVENT_GFX_PLAYER_CUSTOM_FIELD_MOVE;
    case PLAYER_AVATAR_STATE_NORMAL:
    default:
        return OBJ_EVENT_GFX_PLAYER_CUSTOM_NORMAL;
    }
}

u8 PlayerCustom_GetAvatarFlag(u16 graphicsId)
{
    switch (graphicsId)
    {
    case OBJ_EVENT_GFX_PLAYER_CUSTOM_BIKE:
        return PLAYER_AVATAR_FLAG_MACH_BIKE;
    case OBJ_EVENT_GFX_PLAYER_CUSTOM_SURF:
        return PLAYER_AVATAR_FLAG_SURFING;
    default:
        return PLAYER_AVATAR_FLAG_ON_FOOT;
    }
}

// ---------------------------------------------------------------------------- builder screen data
const u8 *PlayerCustom_GetPartName(u8 part)
{
    return sPlayerParts[ClampPart(part)].name;
}

u32 PlayerCustom_GetColorPresetCount(enum PlayerCustomColorRole role)
{
    return sColorRoles[role].count;
}

const u8 *PlayerCustom_GetColorPresetName(enum PlayerCustomColorRole role, u8 preset)
{
    if (preset >= sColorRoles[role].count)
        preset = 0;
    return sColorRoles[role].presets[preset].name;
}
