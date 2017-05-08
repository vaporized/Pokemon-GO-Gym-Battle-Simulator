#ifndef STRATEGIES_H_
#define STRATEGIES_H_

#include "combatant.h"
#include <algorithm>


// An abstract base class
class Strategy {
public:
	Strategy(const Combatant* self, char dodge_s, char special_s);

	// Exam whether the strategy codes are valid for attacker or defender strategy respectively
	static bool is_valid_atk(char dodge_s, char special_s);
	static bool is_valid_def(char dodge_s, char special_s);

	// Output the codes
	friend ostream& operator<<(ostream&, const Strategy&);

	// A Pure Virtual function. To be implemented in AttackerStrategy and DefenderStrategy
	virtual void get(list<Action>& actions, const int time, const Combatant* enemy, const list<Event>* enemy_events) = 0;


protected:
	const Combatant * self;

	char dodge_strat;
	char special_strat;

};



class AttackerStrategy: public Strategy{
public:
	AttackerStrategy(const Combatant* self, char dodge_s, char special_s);

	void get(list<Action>& actions, const int time, const Combatant* enemy, const list<Event>* enemy_events);

private:
	int time_to_dw_close;
	int energy_left;

	bool dodge_this_attack(const Event&);

	void handle_dodge(list<Action>& actions, const int dw_open_dt, const int dw_close_dt);
	void handle_special_safe(list<Action>& actions, const Combatant* enemy);

};




class DefenderStrategy : public Strategy {
public:
	DefenderStrategy(const Combatant* self, char dodge_s, char special_s);

	void get(list<Action>& actions, const int time, const Combatant* enemy, const list<Event>* enemy_events);


private:

};





#endif // !STRATEGIES_H_

