#include "moves.h"

Move::Move(string name, PokemonType type, int power, int energy, int duration, int dmg_wd_start, int dmg_wd_end) :
	name(name), type(type), power(power), energy(energy), duration(duration),
	dmg_wd_start(dmg_wd_start), dmg_wd_end(dmg_wd_end) {

}



QuickMove::QuickMove(string name, PokemonType type, int power, int energy, int duration,
	int dmg_wd_start, int dmg_wd_end) :
	Move(name, type, power, energy, duration, dmg_wd_start, dmg_wd_end) {

}


ChargeMove::ChargeMove(string name, PokemonType type, int power, int energy, int duration,
	int dmg_wd_start, int dmg_wd_end) :
	Move(name, type, power, energy, duration, dmg_wd_start, dmg_wd_end) {

}