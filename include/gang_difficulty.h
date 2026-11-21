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

#endif // GUARD_GANG_DIFFICULTY_H
