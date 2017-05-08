
#ifndef POKEMON_H_
#define POKEMON_H

#include <string>
#include <map>

#include "cpm.h"
#include "moves.h"

using namespace std;

extern map<PokemonLevel, double> CPM_TABLE;
extern map<PokemonType, map<PokemonType, double>> TYPE_TABLE;
extern map<string, QuickMove*> QUICK_MOVES_LIST;
extern map<string, ChargeMove*> CHARGE_MOVES_LIST;

// This class only stores basic info of a species and/or an individual,
// For advance use (such as battling), use its derived classes in combatant.h
class Pokemon {
public:
	Pokemon(string name = "", PokemonType type1 = PokemonType("None"), 
		PokemonType type2 = PokemonType("None"), int baseAtk=0, int baseDef=0, int baseStm=0);
	Pokemon(const Pokemon&);

	/* Accessors */
	string get_name() const;
	PokemonLevel get_level() const;
	QuickMove* get_quick_move() const;
	ChargeMove* get_charge_move() const;


	/* Modifiers */
	const Pokemon& operator=(const Pokemon&);

	void set_level(double level);
	void set_level(int floored_level, bool half);
	void set_iv(int ivAtk, int ivDef, int ivStm);

	void set_quick_move(const string& quick_move_name);
	void set_charge_move(const string& charge_move_name);

	
	/* Debug Usage*/


protected:
	string name;
	PokemonType type1;
	PokemonType type2;

	int baseAtk;
	int baseDef;
	int baseStm;
	int ivAtk;
	int ivDef;
	int ivStm;

	PokemonLevel level;
	double cpm;

	QuickMove* quick_move;
	ChargeMove* charge_move;

};





#endif
