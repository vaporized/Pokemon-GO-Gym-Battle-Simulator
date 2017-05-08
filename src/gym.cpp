#include "gym.h"



Gym::Gym() : attacker(NULL), defender(NULL)
{
	clockMs = BATTLE_START_TIME_MS;
	battle_mode = BATTLE_MODE_STANDARD;
	attacker_events = new list<Event>;
	defender_events = new list<Event>;
	sys_event_log = new list<Event>;
	battle_log = new list<resEvent>;
}

Gym::~Gym()
{
	delete attacker_events;
	delete defender_events;
	delete sys_event_log;
	delete battle_log;
}

void Gym::reset()
{
	clockMs = 0;
	attacker = NULL;
	defender = NULL;
	battle_log->clear();
	sys_event_log->clear();
}

void Gym::go_attacker(Attacker * attacker)
{
	if (this->attacker) {
		process_event(Event(EVENTTYPE_COMBATANT_LEAVE, clockMs, this->attacker, NULL));
	}
	resEvent::attacker = attacker;
	this->attacker = attacker;
	attacker_events->clear();
	process_event(Event(EVENTTYPE_COMBATANT_ENTER, clockMs + SWITCHING_TIME_MS, attacker, NULL));
	
}

void Gym::go_defender(Defender * defender)
{
	if (this->defender) {
		process_event(Event(EVENTTYPE_COMBATANT_LEAVE, clockMs, this->defender, NULL));

		if (this->attacker) {
			for (list<Event>::iterator It = attacker_events->begin(); It != attacker_events->end(); ++It){ 
				// TODO (refund energy)
			}
			attacker_events->clear();
			this->attacker->actions.clear();
		}
	}
	clockMs = BATTLE_START_TIME_MS; // A new defender entering will reset clockMs
	resEvent::defender = defender;
	this->defender = defender;
	defender_events->clear();

	// Hold on to each combatant for SWITCHING_TIME_MS
	attacker_events->push_back(Event(EVENTTYPE_COOLED_DOWN, clockMs + SWITCHING_TIME_MS, defender, NULL));
	defender_events->push_back(Event(EVENTTYPE_COMBATANT_ENTER, clockMs + SWITCHING_TIME_MS, defender, NULL));

	// The defender always starts with three quick moves
	// Note: The delays may not be accurate. To be adjusted in the future
	defender->actions.push_back(Action(ACTIONTYPE_QUICK, 1000));
	defender->actions.push_back(Action(ACTIONTYPE_QUICK, 0));
	defender->actions.push_back(Action(ACTIONTYPE_QUICK, DODGE_WINDOW_MS));


}

void Gym::set_battle_mode(const BattleMode & battle_mode)
{
	this->battle_mode = battle_mode;
}

int Gym::get_timeMs() const
{
	return clockMs;
}


int Gym::get_gym_error_code() const
{
	return gym_error_code;
}

list<resEvent> Gym::get_battle_log() const
{
	return *battle_log;
}

list<Event> Gym::get_sys_log() const
{
	return *sys_event_log;
}


void Gym::battle()
{
	pre_battle_check();
	if (gym_error_code){
		cerr << "Error: pre-battle checking returned code " << gym_error_code << endl;
		return;
	}
		

	while (battle_not_end()) {
		
		if (defender_events->empty()) {
			if (defender->actions.empty())
				defender->strategy_obj->get(defender->actions, clockMs, attacker, attacker_events);
			reg_action_to_event(defender_events, defender, attacker);
		}

		if (attacker_events->empty()) {
			if (attacker->actions.empty()){
				attacker->strategy_obj->get(attacker->actions, clockMs, defender, defender_events);
			}
			reg_action_to_event(attacker_events, attacker, defender);
		}

		Event attackerEvent(attacker_events->front()), defenderEvent(defender_events->front());
		if (attackerEvent.timeMs < defenderEvent.timeMs) {
			process_event(attackerEvent);
			attacker_events->pop_front(); // remove the event just handled
		}
		else {
			process_event(defenderEvent);
			defender_events->pop_front();
		}

	}



}



bool Gym::battle_not_end()
{
	switch (battle_mode)
	{
	case BATTLE_MODE_STANDARD:
		return attacker->get_HP() > 0 && defender->get_HP() > 0 && clockMs < MAX_BATTLE_TIME_MS;
	case BATTLE_MODE_IMMORTAL_ATTACKER:
		return defender->get_HP() > 0 && clockMs < MAX_BATTLE_TIME_MS;
	case BATTLE_MODE_UNLIMITED_TIME:
		return attacker->get_HP() > 0 && defender->get_HP() > 0;
	default:
		return true;
	}
}





void Gym::pre_battle_check()
{
	gym_error_code = 0;
	
	if (!attacker)
		gym_error_code = 1;
	if (!attacker->get_quick_move())
		gym_error_code = 2;
	if (!attacker->get_charge_move())
		gym_error_code = 3;

	if (!defender)
		gym_error_code = -1;
	if (!defender->get_quick_move())
		gym_error_code = -2;
	if (!defender->get_charge_move())
		gym_error_code = -3;	
}

void Gym::reg_action_to_event(list<Event>* events, Combatant* self, Combatant* enemy)
{
	int timeMs = clockMs + self->actions.front().delay;
	int temp;

	switch (self->actions.front().action_type) {

	case ACTIONTYPE_QUICK:
		events->push_back(Event(EVENTTYPE_QUICK_ATTACK_START, timeMs, self, enemy));

		// cannot have "previous" event
		temp = self->get_quick_move()->dmg_wd_start - DODGE_WINDOW_MS;
		if (temp < 0)
			temp = 0;
		events->push_back(Event(EVENTTYPE_QUICK_ATTACK_DW_OPEN,	timeMs + temp, self, enemy));

		events->push_back(Event(EVENTTYPE_QUICK_ATTACK_DW_CLOSE, timeMs + self->get_quick_move()->dmg_wd_end,
			self, enemy));

		events->push_back(Event(EVENTTYPE_COOLED_DOWN, timeMs + self->get_quick_move()->duration,
			self, NULL));

		break;

	case ACTIONTYPE_SPECIAL:
		temp = CHARGING_TIME_MS + timeMs;

		events->push_back(Event(EVENTTYPE_SPECIAL_ATTACK_START, temp, self, enemy));

		events->push_back(Event(EVENTTYPE_SPECIAL_ATTACK_DW_OPEN, temp + self->get_charge_move()->dmg_wd_end - DODGE_WINDOW_MS,
			self, enemy));

		events->push_back(Event(EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE, temp + self->get_charge_move()->dmg_wd_end,
			self, enemy));

		events->push_back(Event(EVENTTYPE_COOLED_DOWN, temp + self->get_charge_move()->duration,
			self, NULL));

		break;

	case ACTIONTYPE_DODGE:
		events->push_back(Event(EVENTTYPE_DODGE, timeMs, self, enemy));

		events->push_back(Event(EVENTTYPE_COOLED_DOWN, timeMs + DODGE_COOLDOWN_MS, self, NULL));

		break;

	case ACTIONTYPE_WAIT:
		events->push_back(Event(EVENTTYPE_COOLED_DOWN, timeMs, self, NULL));

		break;
	}

	self->actions.pop_front(); // Remove the action which was just handled
}



void Gym::process_event(const Event & curEvent)
{
	Combatant* subj = curEvent.subject, *obj = curEvent.object;
	clockMs = curEvent.timeMs; // update the gym clock

	switch (curEvent.event_type) {
	case EVENTTYPE_COOLED_DOWN:
		break;

	case EVENTTYPE_QUICK_ATTACK_START:
		subj->use_quick_move();
		break;

	case EVENTTYPE_SPECIAL_ATTACK_START:
		subj->use_charge_move();
		break;

	case EVENTTYPE_DODGE:
		subj->dodge(obj);
		break;

	case EVENTTYPE_QUICK_ATTACK_DW_OPEN:
	case EVENTTYPE_SPECIAL_ATTACK_DW_OPEN:
		subj->open_damage_window();
		break;

	case EVENTTYPE_QUICK_ATTACK_DW_CLOSE:
	case EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE:
		subj->close_damage_window(obj);
		break;

	case EVENTTYPE_COMBATANT_ENTER:

	case EVENTTYPE_COMBATANT_LEAVE:

		break;
	}

}










Gym2::Gym2()
{
}

void Gym2::battle()
{
	Gym::battle();

	if (clockMs > MAX_BATTLE_TIME_MS)
		sys_event_log->push_back(Event(EVENTTYPE_TIME_OUT, clockMs, NULL, NULL));
	if (!attacker->is_alive())
		battle_log->push_back(resEvent(RESEVENTTYPE_FAINT, clockMs, attacker));
	if (!defender->is_alive())
		battle_log->push_back(resEvent(RESEVENTTYPE_FAINT, clockMs, defender));
}


void Gym2::process_event(const Event & curEvent)
{
	Combatant* subj = curEvent.subject, *obj = curEvent.object;
	clockMs = curEvent.timeMs; // update the gym clock

	switch (curEvent.event_type) {

	case EVENTTYPE_COOLED_DOWN:
		break;

	case EVENTTYPE_QUICK_ATTACK_START:
		subj->use_quick_move();
		battle_log->push_back(resEvent(RESEVENTTYPE_USE_ATTACK, clockMs, subj, subj->get_quick_move()));
		break;

	case EVENTTYPE_SPECIAL_ATTACK_START:
		subj->use_charge_move();
		battle_log->push_back(resEvent(RESEVENTTYPE_USE_ATTACK, clockMs, subj, subj->get_charge_move()));
		break;

	case EVENTTYPE_DODGE:
		subj->dodge(obj);
		battle_log->push_back(resEvent(RESEVENTTYPE_DODGE, clockMs, subj));
		break;

	case EVENTTYPE_QUICK_ATTACK_DW_OPEN:
	case EVENTTYPE_SPECIAL_ATTACK_DW_OPEN:
		subj->open_damage_window();
		break;

	case EVENTTYPE_QUICK_ATTACK_DW_CLOSE:
	case EVENTTYPE_SPECIAL_ATTACK_DW_CLOSE:
		battle_log->push_back(resEvent(RESEVENTTYPE_TAKE_DAMAGE, clockMs, obj, NULL,
			subj->close_damage_window(obj)));

		break;

	case EVENTTYPE_COMBATANT_ENTER:
		battle_log->push_back(resEvent(RESEVENTTYPE_ENTER, clockMs, subj));
		break;

	case EVENTTYPE_COMBATANT_LEAVE:

		break;
	}

	sys_event_log->push_back(curEvent);
}
