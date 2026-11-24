#ifndef GUARD_GANG_RANDOM_H
#define GUARD_GANG_RANDOM_H

// Gangster Outbreak: per-save randomizer for wild Pokémon and trainer teams.
// Every save file gets its own seed; the same route / trainer always gives the same species in that save.
// Species fit the level (evolution stage), legendaries only from level 50 and rarely.

void GangRandom_NewSeed(void);
enum Species GangRandom_WildSpecies(enum Species tableSpecies, u8 level);
void GangRandom_RandomizeTrainerParty(struct Pokemon *party, u16 trainerNum);
bool32 GangRandom_IsBossTrainer(u16 trainerNum);
void GangRandom_FinishLeagueParty(struct Pokemon *party, u16 trainerNum); // after the difficulty: held items
u32 GangRandom_GetLeagueFullRestores(u16 trainerNum);                   // Full Restores in the league trainers' bag
u8 GangRandom_ScaleWildLevel(u8 level);
u32 GangRandom_Hash(u32 a, u32 b, u32 c, u32 d); // deterministic per save file // level scaling: wild Pokémon keep up with the player (up to the level cap)

// Gangster Outbreak story (gang_story.c)
void GangStory_GiveFullMoveset(struct Pokemon *mon); // fills all four move slots
enum Species GangStory_GetRivalStarter(void);         // Silver's random Pokémon (SPECIES_NONE in old saves)

// Gangster Outbreak loot (gang_loot.c): take a Pokémon from a beaten normal trainer
void GangLoot_RememberParty(struct Pokemon *party); // called when a trainer party is created
void GangLoot_RepairBadEggs(void);                   // repairs Pokémon older versions broke when taking them

// Gangster Outbreak items (gang_items.c): Poké Vial and Auto-Repel with 1 + one charge per Gym Badge
#define GANG_AUTO_REPEL_STEPS 250
u32 GangItems_GetVialCharges(void);
u32 GangItems_GetRepelCharges(void);
void GangItems_UseVialCharge(void);
void GangItems_UseRepelCharge(void);
void GangItems_Recharge(void);

#endif // GUARD_GANG_RANDOM_H
