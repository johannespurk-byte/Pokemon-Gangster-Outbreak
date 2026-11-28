#ifndef GUARD_GANG_NUZLOCKE_H
#define GUARD_GANG_NUZLOCKE_H

// Gangster Outbreak: optional NUZLOCKE rules, only selectable on the BOSS difficulty.
//  - only the first wild POKéMON of every area can be caught (shiny POKéMON are always allowed)
//  - a POKéMON that faints is dead: it can't be revived or healed any more (CENTER, PC, items)

bool32 GangNuzlocke_IsEnabled(void);
void GangNuzlocke_Set(bool32 enabled);           // also clears the used areas
void GangNuzlocke_OnBattleInit(void);            // first wild encounter of an area -> catching allowed
bool32 GangNuzlocke_IsCatchBlocked(void);
void GangNuzlocke_MarkFaintedPartyMons(void);    // after a battle: fainted POKéMON die
bool32 GangNuzlocke_IsBoxMonDead(const struct BoxPokemon *boxMon);

#endif // GUARD_GANG_NUZLOCKE_H
