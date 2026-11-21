#ifndef GUARD_GANG_DIFFICULTY_H
#define GUARD_GANG_DIFFICULTY_H

// Gangster Outbreak: difficulty chosen after the character builder in Giovanni's speech.
enum GangDifficulty
{
    GANG_DIFFICULTY_RECRUIT,        // no IVs, no EVs, no items
    GANG_DIFFICULTY_PETTY_CROOK,    // best ability, still no IVs/EVs/items
    GANG_DIFFICULTY_ELITE_GANGSTER, // best ability, perfect IVs, full EVs; unlocks the stat editor
    GANG_DIFFICULTY_BOSS,           // like Elite Gangster plus a fitting held item
    GANG_DIFFICULTY_COUNT,
};

#define GANG_DIFFICULTY_RECOMMENDED GANG_DIFFICULTY_RECRUIT

enum GangDifficulty GangDifficulty_Get(void);
void GangDifficulty_Set(enum GangDifficulty difficulty);
bool32 GangDifficulty_IsStatEditorUnlocked(void);
const u8 *GangDifficulty_GetName(enum GangDifficulty difficulty);
const u8 *GangDifficulty_GetDescription(enum GangDifficulty difficulty);
void GangDifficulty_ApplyToTrainerMon(struct Pokemon *mon);

// Difficulty menu limits: in a NEW GAME+ the player may keep the cleared difficulty or pick a harder one
enum GangDifficulty GangDifficulty_GetMinSelectable(void);
enum GangDifficulty GangDifficulty_GetDefaultSelection(void);

// NEW GAME+: unlocked by entering the Hall of Fame; keeps party, boxes, Pokédex, money and items
// (key items are dropped so story events still trigger).
extern bool8 gNewGamePlusPending; // set by the main menu, consumed by NewGameInitData()
bool32 NewGamePlus_IsUnlocked(void);
void NewGamePlus_OnHallOfFame(void);
void *NewGamePlus_BackupBeforeReset(void);
void NewGamePlus_RestoreAfterReset(void *backup);
void NewGamePlus_UpdateOriginalTrainerNames(void);

#endif // GUARD_GANG_DIFFICULTY_H
