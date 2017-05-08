#ifndef COMBATANT_H_
#define COMBATANT_H_

#include <list>
#include <iomanip>
#include "formulas.h"
#include "resources.h"

using namespace std;

/* 
 * PART I: COMBATANT 
 */

class Event;
class Action;
class Strategy;

// An base class for Attacker and Defender
class Combatant : public Pokemon {
public:
	Combatant(const Pokemon&);
	~Combatant();

	/* Accessors */
	virtual string get_name(bool = false) const;
	int get_energy() const;
	int get_HP() const;
	virtual int get_full_HP() const;
	int get_CP() const;
	int get_quick_move_damage(const Combatant*, bool) const;
	int get_charge_move_damage(const Combatant*, bool) const;
	
	bool is_alive() const;
	bool has_dodged() const;

	friend ostream& operator << (ostream&, const Combatant&);


	/* Modifiers (actions) */
	void full_heal();

	void use_quick_move();
	void use_charge_move();
	void dodge(const Combatant*);

	void open_damage_window();

	// This functions determines how much damage the enemy will take and 
	// makes it happen by calling the function take_damage(int) of the enemy;
	int close_damage_window(Combatant*);

	void take_damage(int);
	

	/* Public members */

	// strategy is implemented as an object
	Strategy* strategy_obj;


	list<Action> actions; // Queued actions to be handled

private:
	int HP;
	int energy;
	
	bool dodged_next_attack; // Indicates whether sucessfully dodged the next attack or not
	bool is_current_attack_quick; // If the current attack is a quick attack
	bool damage_window_opened; // Indicates whether the damage window of an attack is open or not
	
	double get_cAtk() const;
	double get_cDef() const;
	double get_cStm() const;

	static double get_effe(const PokemonType&, const PokemonType&);
	int get_move_damage(const Move *, const Combatant*, bool) const;

};




// Class for gym attacker
class Attacker : public Combatant {
public:
	Attacker(const Pokemon&);

	string get_name(bool = false) const;

private:

};



// Class for gym defender
class Defender : public Combatant {
public:
	Defender(const Pokemon&);

	string get_name(bool = false) const;
	int get_full_HP() const;

private:

};







/*
 * PART II: EVENT FLOW
 */


// 1. From a Combatant's point of view, there are actions
enum ActionType {
	ACTIONTYPE_QUICK,
	ACTIONTYPE_SPECIAL,
	ACTIONTYPE_DODGE,
	ACTIONTYPE_WAIT
};


class Action {
public:
	Action(ActionType action_type, int delay);

	ActionType action_type;
	int delay;
};




// 2. From the system's point of view, actions are broken down into sub-events
enum EventType {
	EVENTTYPE_COMBATANT_ENTER,
	EVENTTYPE_COMBATANT_LEAVE,
	EVENTTYPE_QUICK_ATTACK_START,
	EVENTTYPE_QUICK_ATTACK_DW_OPEN,
	EVENTTYPE_QUICK_ATTACK_DW_CLOSE,
	EVENTTYPE_SPECIAL_ATTACK_START,
	EVENTTYPE_SPECIAL_ATTACK_DW_OPEN,
	EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE,
	EVENTTYPE_COOLED_DOWN,
	EVENTTYPE_DODGE,
	EVENTTYPE_TIME_OUT
};


class Event {
public:
	friend class Gym;

	Event(EventType event_type, int timeMs, Combatant* subject, Combatant* object);

	// Predicate
	static bool is_dw_open(const Event&);
	static bool is_dw_close(const Event&);


	// Output the event
	friend ostream& operator<<(ostream&, const Event&);


	EventType event_type;
	int timeMs;
	Combatant* subject;
	Combatant* object;
};


// 3. Result Events are what will be presented to users
enum resEventType {
	RESEVENTTYPE_ENTER,
	RESEVENTTYPE_FAINT,
	RESEVENTTYPE_USE_ATTACK,
	RESEVENTTYPE_DODGE,
	RESEVENTTYPE_TAKE_DAMAGE,
};

class resEvent {
public:
	resEvent(resEventType, const int, const Combatant*, const Move* = NULL, const int = 0);

	friend ostream& operator<<(ostream&, const resEvent&);


	// Common attacker and defender
	static const Combatant* attacker;
	static const Combatant* defender;

private:

	// Individual parameters
	resEventType event_type;
	const int timeMs;
	const Combatant* subject;

	int a_HP;
	int a_Energy;
	int d_HP;
	int d_Energy;

	const Move* move;
	const int damage;
};







#endif