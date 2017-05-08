#ifndef MOVES_H
#define MOVES_H

#include <string>

#include "types.h"

using namespace std;



// An abstract base class
class Move {
public:
	Move(string name, PokemonType type, int power, int energy, int duration, int dmg_wd_start, int dmg_wd_end);

	const string name;
	const PokemonType type;
	const int power;
	const int energy;
	const int duration;
	const int dmg_wd_start;
	const int dmg_wd_end;
};



class QuickMove : public Move {
public:
	QuickMove(string name, PokemonType type = PokemonType("None"), int power = 0, int energy = 0, int duration = 0,
		int dmg_wd_start = 0, int dmg_wd_end = 0);

};


class ChargeMove : public Move {
public:
	ChargeMove(string name, PokemonType type = PokemonType("None"), int power = 0, int energy = 0, int duration = 0,
		int dmg_wd_start = 0, int dmg_wd_end = 0);

};




#endif // !moves_H
