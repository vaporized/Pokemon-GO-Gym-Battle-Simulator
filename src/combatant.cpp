#include "combatant.h"


Combatant::Combatant(const Pokemon & pokemon) : Pokemon(pokemon)
{
	full_heal();
	strategy_obj = NULL;
}

Combatant::~Combatant()
{
	delete strategy_obj;
}


bool Combatant::is_alive() const
{
	return HP > 0;
}

bool Combatant::has_dodged() const
{
	return dodged_next_attack;
}


void Combatant::full_heal()
{
	HP = get_full_HP();
	energy = 0;
	actions.clear();
	dodged_next_attack = false;
	damage_window_opened = false;
}

void Combatant::use_quick_move()
{
	energy += quick_move->energy;
	if (energy > MAX_ENERGY)
		energy = MAX_ENERGY;
	is_current_attack_quick = true;
}

void Combatant::use_charge_move()
{
	energy += charge_move->energy; // the energyDelta of charge move is already negative
	if (energy > MAX_ENERGY)
		energy = MAX_ENERGY;
	is_current_attack_quick = false;
}

void Combatant::dodge(const Combatant* enemy)
{
	dodged_next_attack = enemy->damage_window_opened;
}

void Combatant::open_damage_window()
{
	damage_window_opened = true;
}

int Combatant::close_damage_window(Combatant * enemy)
{
	int damage;
	if (is_current_attack_quick)
		damage = get_quick_move_damage(enemy, enemy->dodged_next_attack);
	else
		damage = get_charge_move_damage(enemy, enemy->dodged_next_attack);

	enemy->take_damage(damage);
	return damage;
}

void Combatant::take_damage(int damage)
{
	HP -= damage;
	if (HP < 0)
		HP = 0;

	energy += formula_energy_gain(damage);
	if (energy > MAX_ENERGY)
		energy = MAX_ENERGY;

	dodged_next_attack = false;
}



string Combatant::get_name(bool specified) const
{
	return name;
}

int Combatant::get_energy() const
{
	return energy;
}

int Combatant::get_HP() const
{
	return HP;
}

int Combatant::get_CP() const
{
	return formula_CP(cpm, baseAtk, baseDef, baseStm, ivAtk, ivDef, ivStm);
}

double Combatant::get_cAtk() const
{
	return formula_cStat(cpm, baseAtk, ivAtk);
}

double Combatant::get_cDef() const
{
	return formula_cStat(cpm, baseDef, ivDef);
}

double Combatant::get_cStm() const
{
	return formula_cStat(cpm, baseStm, ivStm);
}

double Combatant::get_effe(const PokemonType & atk_type, const PokemonType & def_type)
{
	map<PokemonType, map<PokemonType,double>>::const_iterator it = TYPE_TABLE.find(atk_type);
	if (it != TYPE_TABLE.begin()) {
		map<PokemonType, double>::const_iterator it2 = it->second.find(def_type);
		if (it2 != it->second.begin())
			return it2->second;
	}
	return 1;
}

int Combatant::get_quick_move_damage(const Combatant * opponent, bool dodged) const
{
	return get_move_damage(quick_move, opponent, dodged);
}

int Combatant::get_charge_move_damage(const Combatant * opponent, bool dodged) const
{
	return get_move_damage(charge_move, opponent, dodged);
}

int Combatant::get_full_HP() const
{
	// to be overrided in Defender
	return formula_attackerHP(cpm, baseStm, ivStm);
}

int Combatant::get_move_damage(const Move * move, const Combatant * opponent, bool dodged) const
{
	double stab = (move->type == type1 || move->type == type2) ? STAB_MODIFIER : 1;
	double effe1 = get_effe(move->type,opponent->type1);
	double effe2 = get_effe(move->type, opponent->type2);
	double dodge_mod = dodged ? DODGE_MODIFIER : 1;
	return formula_damage(move->power, get_cAtk(), opponent->get_cDef(), stab, effe1*effe2, dodge_mod);
}


Attacker::Attacker(const Pokemon & pokemon) :
	Combatant(pokemon)
{

}


string Attacker::get_name(bool specified) const
{
	return specified ? "[Attacker]" + name : name;
}


Defender::Defender(const Pokemon & pokemon) :
	Combatant(pokemon)
{

}

string Defender::get_name(bool specified) const
{
	return specified ? "[Defender]" + name : name;
}

int Defender::get_full_HP() const
{
	return formula_defenderHP(cpm, baseStm, ivStm);
}



Action::Action(ActionType action_type, int delay) :
	action_type(action_type), delay(delay)
{
}





Event::Event(EventType event_type, int timeMs, Combatant * subject, Combatant * object) :
	event_type(event_type), timeMs(timeMs), subject(subject), object(object)
{

}

bool Event::is_dw_open(const Event &e)
{
	return e.event_type == EVENTTYPE_QUICK_ATTACK_DW_OPEN || e.event_type == EVENTTYPE_SPECIAL_ATTACK_DW_OPEN;
}

bool Event::is_dw_close(const Event &e)
{
	return e.event_type == EVENTTYPE_QUICK_ATTACK_DW_CLOSE || e.event_type == EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE;
}




ostream & operator<<(ostream & os, const Event & e)
{
	os << left;
	os << setw(8) << e.timeMs;

	os << left << setw(24);
	switch (e.event_type) {
	case EVENTTYPE_COMBATANT_ENTER:
		os << "COMBATANT_ENTER"; break;
	case EVENTTYPE_COMBATANT_LEAVE:
		os << "COMBATANT_LEAVE"; break;
	case EVENTTYPE_COOLED_DOWN:
		os << "COOL_DOWN_COMPLETED"; break;

	case EVENTTYPE_QUICK_ATTACK_START:
		os << "QUICK_ATTACK_START"; break;
	case EVENTTYPE_QUICK_ATTACK_DW_OPEN:
		os << "QUICK_ATTACK_DW_OPEN"; break;
	case EVENTTYPE_QUICK_ATTACK_DW_CLOSE:
		os << "QUICK_ATTACK_DW_CLOSE"; break;

	case EVENTTYPE_SPECIAL_ATTACK_START:
		os << "CHARGE_ATTACK_START"; break;
	case EVENTTYPE_SPECIAL_ATTACK_DW_OPEN:
		os << "CHARGE_ATTACK_DW_OPEN"; break;
	case EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE:
		os << "CHARGE_ATTACK_DW_CLOSE"; break;

	case EVENTTYPE_DODGE:
		os << "DODGE"; break;
	case EVENTTYPE_TIME_OUT:
		os << "TIME_OUT"; break;

	default:
		os << "[UNIDENTIFIED EVENT]";
	}
	

	os << setw(24) << (e.subject ? e.subject->get_name(true) : "(NULL)");

	os << setw(24) << (e.object ? e.object->get_name(true) : "(NULL)");

	return os;
}

ostream & operator<<(ostream & os, const resEvent & e)
{
	bool is_attacker = e.subject == resEvent::attacker;

	os << left;

	os << setw(COLUMN_WIDTH) << e.timeMs; // will be modified in the future

	if(is_attacker){
		os << setw(2* COLUMN_WIDTH) << " ";
		os << setw(COLUMN_WIDTH) << e.a_HP << setw(COLUMN_WIDTH) << e.a_Energy;
	}
	else {
		os << setw(COLUMN_WIDTH) << e.d_HP << setw(COLUMN_WIDTH) << e.d_Energy;
		os << setw(2* COLUMN_WIDTH) << " ";
	}

	os << e.subject->get_name(true) << " ";

	switch (e.event_type)
	{
	case RESEVENTTYPE_USE_ATTACK:
		os << "used " << e.move->name; break;
	case RESEVENTTYPE_DODGE:
		os << "dodged"; break;
	case RESEVENTTYPE_TAKE_DAMAGE:
		os << "took " << e.damage << " damage"; break;
	case RESEVENTTYPE_ENTER:
		os << "entered the battle"; break;
	case RESEVENTTYPE_FAINT:
		os << "fainted"; break;
	}

	return os;
}

const Combatant* resEvent::attacker = NULL;
const Combatant* resEvent::defender = NULL;


resEvent::resEvent(resEventType event_type, const int timeMs, const Combatant * subject,
	const Move * move, const int damage) :
	event_type(event_type), subject(subject), timeMs(timeMs), move(move), damage(damage)
{
	if (subject == attacker) {
		a_HP = attacker->get_HP();
		a_Energy = attacker->get_energy();
	}
	else {
		d_HP = defender->get_HP();
		d_Energy = defender->get_energy();
	}
}