#ifndef GUARD_GANG_RANDOM_H
#define GUARD_GANG_RANDOM_H

// Gangster Outbreak: per-save randomizer for wild Pokémon and trainer teams.
// Every save file gets its own seed; the same route / trainer always gives the same species in that save.
// Species fit the level (evolution stage), legendaries only from level 50 and rarely.

void GangRandom_NewSeed(void);
enum Species GangRandom_WildSpecies(enum Species tableSpecies, u8 level);
void GangRandom_RandomizeTrainerParty(struct Pokemon *party, u16 trainerNum);

// Gangster Outbreak loot (gang_loot.c): take a Pokémon from a beaten normal trainer
void GangLoot_RememberParty(struct Pokemon *party); // called when a trainer party is created

#endif // GUARD_GANG_RANDOM_H
