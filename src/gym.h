#ifndef GYM_H_
#define GYM_H_

#include "strategies.h"



enum BattleMode {
	BATTLE_MODE_STANDARD,
	BATTLE_MODE_IMMORTAL_ATTACKER,
	BATTLE_MODE_UNLIMITED_TIME
};


// This class is responsible for controlling the battle


class Gym {
public:
	Gym();
	~Gym();

	/* Modifying Operations */
	// Clear the battle_log, set both attacker and defender to NULL
	void reset();

	// Let an attacker enter the battle
	void go_attacker(Attacker*);

	// Let an defender enter the battle
	void go_defender(Defender*);

	// Set the battle mode
	void set_battle_mode(const BattleMode&);


	/* Accessing */

	// Get the gym clock time in ms
	int get_timeMs() const;

	/*
		Code = > Explanation:
		0	= > All set
		1	= > Attacker is NULL
		2	= > Attacker has NULL quick move
		3	= > Attacker has NULL charge move
		-1	= > Defender is NULL
		-2	= > Defender has NULL quick move
		-3	= > Defender has NULL charge move
	*/
	int get_gym_error_code() const;

	// Get a copy of the battle log
	virtual list<resEvent> get_battle_log() const;

	// Get a copy of the system event log for debug usage
	virtual list<Event> get_sys_log() const;


	// Simulates a battle between attacker and defender and directly modifies them
	// Ends depends on the battle mode
	// You should check both parties' status and remove them from the gym if suitable outside this function
	virtual void battle();

	// Indicates whether the battle has ended based on the status of attacker and defender
	bool battle_not_end();
	

protected:
	// support multiple attackers in the future
	// list<Attacker*> attackers;

	BattleMode battle_mode;
	Attacker* attacker;
	Defender* defender;
	int clockMs;
	int gym_error_code; // Indicates some invalid inputs

	list<resEvent>* battle_log; // Present results to users
	list<Event>* attacker_events; // Queued events parsed from actions of attacker
	list<Event>* defender_events; // Queued events parsed from actions of defender
	list<Event>* sys_event_log; // Log the handled events for debug usage
	
	void pre_battle_check();
	void reg_action_to_event(list<Event>*, Combatant*, Combatant*);

	virtual void process_event(const Event&);

};




// Supports battle log, but slower
class Gym2: public Gym {
public:
	Gym2();

	void battle();

protected:
	void process_event(const Event&);

};



#endif

