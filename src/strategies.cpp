#include "strategies.h"
#include <random>
#include <stdlib.h>
#include <time.h>


linear_congruential_engine<unsigned int, 1, 1, 0> random_generator;

// This is a helper function
// Returns a random true or false, for AI strategy usage
bool _helper_function_2() {
	random_generator.seed(time(0));
	bernoulli_distribution dist(0.5);
	return dist(random_generator);
}

// This is a helper function
// Returns a random delay of action in [EXPECTED_DELAY - 400, EXPECTED_DELAY + 400] ms, for AI strategy usage
int _helper_function_3() {
	random_generator.seed(time(0));
	uniform_int_distribution<int> dist(-8, 8);
	return DEFENDER_EXPECTED_DELAY_MS + dist(random_generator) * 50;
}


Strategy::Strategy(const Combatant * self, char dodge_s, char special_s) :
	self(self), dodge_strat(dodge_s), special_strat(special_s)
{
}

bool Strategy::is_valid_atk(char dodge_s, char special_s)
{
	switch (dodge_s)
	{
	case 'n':
	case 's':
	case 'a':
		break;
	default:
		return false;
	}

	switch (special_s)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
		break;
	default:
		return false;
	}

	return true;
}

bool Strategy::is_valid_def(char dodge_s, char special_s)
{
	switch (dodge_s)
	{
	case 'd':
		break;
	default:
		return false;
	}

	switch (special_s)
	{
	case '1':
	case '2':
	case '3':
		break;
	default:
		return false;
	}

	return true;
}

ostream & operator<<(ostream & os, const Strategy & s)
{
	os << s.dodge_strat << s.special_strat;
	return os;
}



AttackerStrategy::AttackerStrategy(const Combatant* s, char dodge_s, char special_s):
	Strategy(s,dodge_s,special_s)
{
	if (!Strategy::is_valid_atk(dodge_s, special_s)) {
		dodge_s = 'n';
		special_s = '0';
	}
}


void AttackerStrategy::get(list<Action>& actions, const int time, const Combatant* enemy, const list<Event>* enemy_events)
{
	energy_left = self->get_energy();

	if (dodge_strat != 'n' && !self->has_dodged()) {
		list<Event>::const_iterator dw_close = find_if(enemy_events->begin(), enemy_events->end(), Event::is_dw_close);

		if (dw_close != enemy_events->end() && dodge_this_attack(*dw_close)) { // Perform dodging
			list<Event>::const_iterator dw_open = find_if(enemy_events->begin(), enemy_events->end(), Event::is_dw_open);
			handle_dodge(actions, dw_open != enemy_events->end() ? dw_open->timeMs - time : 0,
				dw_close->timeMs - time);

			// After dodging, decide what to do next
			if (energy_left + self->get_charge_move()->energy >= 0) {

				switch (special_strat)
				{
				case '0':
				case '1': // Use special attack righr after dodging

					actions.push_back(Action(ACTIONTYPE_SPECIAL, 0)); break;
				case '2': // Use special attack righr after dodging enemy's special attack
					if (dw_close->event_type == EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE)
						actions.push_back(Action(ACTIONTYPE_SPECIAL, 0));
					else
						actions.push_back(Action(ACTIONTYPE_QUICK, 0));
					break;
				case '3':
					handle_special_safe(actions, enemy);
					break;
				}
			}
		}
	}
	else if (energy_left + self->get_charge_move()->energy >= 0 && special_strat != '4') { // No dodging
		actions.push_back(Action(ACTIONTYPE_SPECIAL, 0));
	}

	// make sure at least a quick attack is queued
	if (actions.empty()) {
		actions.push_back(Action(ACTIONTYPE_QUICK, 0));
	}

}

bool AttackerStrategy::dodge_this_attack(const Event & e)
{
	switch (this->dodge_strat)
	{
	case 'n':
		return false;
	case 's':
		return e.event_type == EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE;
	case 'a':
		return e.event_type == EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE ||
			e.event_type == EVENTTYPE_QUICK_ATTACK_DW_CLOSE;
	default:
		return false;
	}
}

void AttackerStrategy::handle_special_safe(list<Action>& actions, const Combatant * enemy)
{

	// Decided whether to queue a special attack after dodging or not
	// Depends on the duration of the charge move and enemy's. Tricky!
	int cool_down = self->get_charge_move()->duration - time_to_dw_close;
	int enemy_next_qm_close = DEFENDER_EXPECTED_DELAY_MS + enemy->get_quick_move()->dmg_wd_end;
	int enemy_next_cm_close = DEFENDER_EXPECTED_DELAY_MS + enemy->get_charge_move()->dmg_wd_end;

	if (dodge_strat == 's' && cool_down < enemy_next_cm_close) {
		actions.push_back(Action(ACTIONTYPE_SPECIAL, 0));
	}
	else if (dodge_strat == 'a' && cool_down < enemy_next_qm_close && cool_down < enemy_next_cm_close) {
		actions.push_back(Action(ACTIONTYPE_SPECIAL, 0));
	}
}

void AttackerStrategy::handle_dodge(list<Action>& actions, const int dw_open_dt, const int dw_close_dt)
{
	QuickMove* qm = self->get_quick_move();
	ChargeMove* cm = self->get_charge_move();
	int qm_t = qm->duration, cm_t = CHARGING_TIME_MS + cm->duration;
	int passed_t = 0;

	while (true) {
		if (energy_left + cm->energy >= 0 && special_strat != '4' && 
			(passed_t + cm_t < dw_close_dt || special_strat == '0')) { // First try to fit in charge attacks
			actions.push_back(Action(ACTIONTYPE_SPECIAL, 0));
			passed_t += cm_t;
			energy_left += cm->energy;
		}
		else if (passed_t + qm_t < dw_close_dt) { // Then quick attacks
			actions.push_back(Action(ACTIONTYPE_QUICK, 0));
			passed_t += qm_t;
			energy_left += qm->energy;
			energy_left = energy_left > MAX_ENERGY ? MAX_ENERGY : energy_left;
		}
		else { // If not any attacks can be fitted in, dodge
			int dodge_delay = dw_open_dt - passed_t;
			if (dodge_delay < 0)
				dodge_delay = 0;
			actions.push_back(Action(ACTIONTYPE_DODGE, dodge_delay));
			passed_t += DODGE_COOLDOWN_MS;
			break;
		}
	}
	time_to_dw_close = dw_close_dt - passed_t;
}


DefenderStrategy::DefenderStrategy(const Combatant * self, char dodge_s, char special_s):
	Strategy(self,dodge_s,special_s)
{
	if (!Strategy::is_valid_def(dodge_s, special_s)) {
		dodge_s = 'd';
		special_s = '1';
	}
}

void DefenderStrategy::get(list<Action>& actions, const int time, const Combatant * enemy, const list<Event>* enemy_events)
{
	if(self->get_energy() + self->get_charge_move()->energy >= 0){
		switch (special_strat)
		{
		case '1':
			actions.push_back(Action(ACTIONTYPE_QUICK, DEFENDER_EXPECTED_DELAY_MS));
			actions.push_back(Action(ACTIONTYPE_SPECIAL, DEFENDER_EXPECTED_DELAY_MS));
			break;
		case '2':
			actions.push_back(Action(ACTIONTYPE_SPECIAL, DEFENDER_EXPECTED_DELAY_MS));
			break;
		case '3':
			actions.push_back(Action(ACTIONTYPE_SPECIAL, _helper_function_3()));
			break;
		}
	}
	else {
		switch (special_strat)
		{
		case '1':
		case '2':
			actions.push_back(Action(ACTIONTYPE_QUICK, DEFENDER_EXPECTED_DELAY_MS));
			break;
		case '3':
			actions.push_back(Action(ACTIONTYPE_QUICK, _helper_function_3()));
			break;
		}
	}

	
}
