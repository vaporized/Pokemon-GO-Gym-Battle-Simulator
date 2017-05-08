#include "pokemon.h"


Pokemon::Pokemon(string name, PokemonType type1, PokemonType type2,
	int baseAtk, int baseDef, int baseStm) : name(name),
	type1(type1), type2(type2),
	baseAtk(baseAtk), baseDef(baseDef), baseStm(baseStm) {
	ivAtk = ivDef = ivStm = 0; // initial IVs are 0,0,0
	set_level(1, false); // initial level is 1
	quick_move = NULL;
	charge_move = NULL;
}

Pokemon::Pokemon(const Pokemon & other)
{
	*this = other;
}

string Pokemon::get_name() const
{
	return name;
}

PokemonLevel Pokemon::get_level() const
{
	return level;
}

QuickMove * Pokemon::get_quick_move() const
{
	return quick_move;
}

ChargeMove * Pokemon::get_charge_move() const
{
	return charge_move;
}




void Pokemon::set_level(double level)
{
	int floored_level = (int)level;
	set_level(floored_level, level > floored_level + 0.1);
}

void Pokemon::set_level(int floored_level, bool half) {
	PokemonLevel lvl(floored_level, half);
	if (CPM_TABLE.find(lvl) == CPM_TABLE.end()) // the level is not in CPM table
		lvl = CPM_TABLE.begin()->first; // Take the first level (1.0) in CPM table

	this->level = lvl;
	this->cpm = CPM_TABLE[lvl];
}

const Pokemon & Pokemon::operator=(const Pokemon & rhs)
{
	name = rhs.name;
	type1 = rhs.type1;
	type2 = rhs.type2;
	baseAtk = rhs.baseAtk;
	baseDef = rhs.baseDef;
	baseStm = rhs.baseStm;
	ivAtk = rhs.ivAtk;
	ivDef = rhs.ivDef;
	ivStm = rhs.ivStm;
	quick_move = rhs.quick_move;
	charge_move = rhs.charge_move;
	level = rhs.level;
	cpm = rhs.cpm;

	return *this;
}


void Pokemon::set_iv(int ivAtk, int ivDef, int ivStm) {
	if (0 <= ivAtk && ivAtk <= 15)
		this->ivAtk = ivAtk;
	if (0 <= ivDef && ivDef <= 15)
		this->ivDef = ivDef;
	if (0 <= ivStm && ivStm <= 15)
		this->ivStm = ivStm;
}


void Pokemon::set_quick_move(const string & quick_move_name)
{
	if(QUICK_MOVES_LIST.find(quick_move_name) != QUICK_MOVES_LIST.end()) // There is such move;
		this->quick_move = QUICK_MOVES_LIST[quick_move_name];
}

void Pokemon::set_charge_move(const string & charge_move_name)
{	
	if(CHARGE_MOVES_LIST.find(charge_move_name) != CHARGE_MOVES_LIST.end()) // There is such move
		this->charge_move = CHARGE_MOVES_LIST[charge_move_name];
}
