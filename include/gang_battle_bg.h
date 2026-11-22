#ifndef GUARD_GANG_BATTLE_BG_H
#define GUARD_GANG_BATTLE_BG_H

// Gangster Outbreak: battle backgrounds of the ruined Kanto (src/gang_battle_bg.c)
struct GangBattleScene
{
    const u32 *tiles;
    const u32 *tilemap;
    const u16 *palette; // 3 palettes, loaded to BG palettes 2-4
};

// NULL: use the normal background of that environment
const struct GangBattleScene *GangBattleBg_GetScene(u32 environment);
const struct GangBattleScene *GangBattleBg_GetEntry(u32 environment);

#endif // GUARD_GANG_BATTLE_BG_H
