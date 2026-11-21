#ifndef GUARD_PLAYER_CUSTOM_H
#define GUARD_PLAYER_CUSTOM_H

// Gangster Outbreak character builder: the player's look is assembled from a head template,
// an outfit template and colour choices (see tools/paperdoll/generate.py).

#include "constants/player_custom.h"

enum PlayerCustomState
{
    PLAYER_CUSTOM_STATE_NORMAL,
    PLAYER_CUSTOM_STATE_BIKE,
    PLAYER_CUSTOM_STATE_SURF,
    PLAYER_CUSTOM_STATE_FISH,
    PLAYER_CUSTOM_STATE_FIELDMOVE,
    PLAYER_CUSTOM_STATE_COUNT,
};

enum PlayerCustomColorRole
{
    PLAYER_COLOR_HAIR,
    PLAYER_COLOR_SKIN,
    PLAYER_COLOR_HAT,
    PLAYER_COLOR_TOP,
    PLAYER_COLOR_BOTTOM,
    PLAYER_COLOR_ROLE_COUNT,
};

struct PlayerLook
{
    u8 head;    // enum PlayerPart
    u8 outfit;  // enum PlayerPart
    u8 colors[PLAYER_COLOR_ROLE_COUNT]; // 0 = colours of the template, otherwise a preset
};

bool32 PlayerCustom_IsEnabled(void);
void PlayerCustom_GetLook(struct PlayerLook *look);
void PlayerCustom_SetLook(const struct PlayerLook *look); // also enables the builder and sets playerGender
bool32 PlayerCustom_IsCustomGraphicsId(u16 graphicsId);
u16 PlayerCustom_GetGraphicsId(u8 avatarState);
u8 PlayerCustom_GetAvatarFlag(u16 graphicsId);
enum Gender PlayerCustom_GetGender(void);
void PlayerCustom_EnsureComposed(void);
void PlayerCustom_RefreshLoadedPalette(void);
bool32 PlayerCustom_UsesCustomPic(enum Gender gender);

// Data for the builder screen
const u8 *PlayerCustom_GetPartName(u8 part);
u32 PlayerCustom_GetColorPresetCount(enum PlayerCustomColorRole role);
const u8 *PlayerCustom_GetColorPresetName(enum PlayerCustomColorRole role, u8 preset);
void PlayerCustom_BuildPreview(const struct PlayerLook *look, u8 *gfxDest, u16 *palDest); // 18 frames + 16 colours

extern struct ObjectEventGraphicsInfo gPlayerCustomGraphicsInfo[PLAYER_CUSTOM_STATE_COUNT];
extern u16 gPlayerCustomPalette[16];

#endif // GUARD_PLAYER_CUSTOM_H
