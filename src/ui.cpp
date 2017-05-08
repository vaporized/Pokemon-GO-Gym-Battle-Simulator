#include "ui.h"


ostream& operator<<(ostream& os, const list<resEvent>& log) {
	for (list<resEvent>::const_iterator It = log.begin(); It != log.end(); ++It)
		os << *It << endl;
	return os;
}

ostream& operator<<(ostream& os, const list<Event>& log) {
	for (list<Event>::const_iterator It = log.begin(); It != log.end(); ++It)
		os << *It << endl;
	return os;
}




ostream & operator<<(ostream & os, const Combatant & c)
{
	os << c.name << ',';
	os << c.level << ',';
	os << c.ivAtk << ',';
	os << c.ivDef << ',';
	os << c.ivDef << ',';
	os << (c.quick_move ? c.quick_move->name : "") << ',';
	os << (c.charge_move ? c.charge_move->name : "") << ',';

	// TODO
	os << (*c.strategy_obj);


	return os;
}




BattleResult::BattleResult(Attacker * attacker, Defender * defender) :
	attacker(attacker), defender(defender), timeS(0), time_to_winS(0), KD_first(0), KD_final(0)
{
	gym = new Gym();
}

BattleResult::~BattleResult()
{
	delete gym;
}

void BattleResult::run_simulation()
{
	if (!attacker || !defender)
		return;
	attacker->full_heal();
	defender->full_heal();
	gym->go_attacker(attacker);
	gym->go_defender(defender);

	// Run the first battle
	gym->battle();

	// Check whether there's gym error. If so, return
	if (gym->get_gym_error_code())
		return;

	timeS = (double)gym->get_timeMs() / 1000;
	KD_first = get_simple_KD();

	if (attacker->is_alive()) { // If the attacker wins the first battle
		time_to_winS = timeS;
		prestige = formula_prestige(attacker->get_CP(), defender->get_CP());

		// Bring back the defender and see how many more can the attacker kill
		int num_defenders_beaten = 0;
		while (attacker->is_alive()) {
			++num_defenders_beaten;
			defender->full_heal();
			gym->go_defender(defender);
			if (!gym->battle_not_end()) // Check whether overtime
				break;
			gym->battle();
		}
		KD_final = num_defenders_beaten + get_simple_KD();
	}
	else { // If the attacker loses the first battle
		KD_final = KD_first;
		prestige = 0;

		// Bring back the attacker and see how long does it take to beat the defender
		while (defender->is_alive()) {
			attacker->full_heal();
			gym->go_attacker(attacker);
			if (!gym->battle_not_end())
				break;
			gym->battle();
		}
		time_to_winS = gym->get_timeMs() / 1000;
	}
}

double BattleResult::get_KD_first() const
{
	return KD_first;
}

double BattleResult::get_KD_final() const
{
	return KD_final;
}

double BattleResult::get_timeS() const
{
	return timeS;
}

double BattleResult::get_time_to_winS() const
{
	return time_to_winS;
}

list<resEvent> BattleResult::get_battle_log() const
{
	return gym->get_battle_log();
}

double BattleResult::get_simple_KD() const
{
	// divisor == 0 check
	if (attacker->get_HP() == attacker->get_full_HP())
		return -1;

	return (1 - (double)defender->get_HP() / defender->get_full_HP()) /
		(1 - (double)attacker->get_HP() / attacker->get_full_HP());
}

ostream & operator<<(ostream & os, const BattleResult & res)
{
	os << res.timeS << ','
		<< res.time_to_winS << ','
		<< res.KD_first << ','
		<< res.KD_final << ','
		<< res.prestige;
	return os;
}

ostream & operator<<(ostream & os, const BattleResult2 & res)
{
	if (res.attacker) {
		os << "Attacker: " << *res.attacker << endl;
	}
	else {
		os << "Attacker: NULL" << endl;
	}


	if (res.defender) {
		os << "Defender: " << *res.defender << endl;
	}
	else {
		os << "Defender: NULL" << endl;
	}

	os << "Time: " << res.timeS << endl;
	os << "KD_first: " << res.KD_first << endl;
	os << "Time to win: " << res.time_to_winS << endl;
	os << "KD_final: " << res.KD_final << endl << endl;
	os << "Battle log: " << endl;
	os << left;
	os << setw(COLUMN_WIDTH) << "TimeMs" << setw(COLUMN_WIDTH) << "Dfdr HP" << setw(COLUMN_WIDTH)
		<< "Dfdr Energy" << setw(COLUMN_WIDTH) << "Atkr HP" << setw(COLUMN_WIDTH)
		<< "Atkr Energy" << "Event" << endl;
	os << res.get_battle_log() << endl << endl << endl;

	return os;
}




BattleResult2::BattleResult2(Attacker * a, Defender * d) : BattleResult(a, d)
{
	delete gym;
	gym = new Gym2();
}








/* Application Functions */



bool set_combatant(const CSVIterator & It, Combatant * subj, int start, int rowCount)
{
	subj->set_level(stod((*It)[start + 1]));
	subj->set_iv(stoi((*It)[start + 2]), stoi((*It)[start + 3]), stoi((*It)[start + 4]));

	subj->set_quick_move((*It)[start + 5]);
	if (subj->get_quick_move() == NULL) {
		cout << "Error: In Row " << rowCount << ": No quick move named [" << (*It)[start + 5] << "]" << endl;
		return false;
	}

	subj->set_charge_move((*It)[start + 6]);
	if (subj->get_charge_move() == NULL) {
		cout << "Error: In Row " << rowCount << ": No charge move named [" << (*It)[start + 6] << "]" << endl;
		return false;
	}

	// Handle strategy. Tricky!

	if ((*It)[start + 7].size() == 2) {
		char dodge_strat = (*It)[start + 7][0];
		char special_strat = (*It)[start + 7][1];
		if (Strategy::is_valid_atk(dodge_strat, special_strat)) {
			subj->strategy_obj = new AttackerStrategy(subj, dodge_strat, special_strat);
		}
		else if (Strategy::is_valid_def(dodge_strat, special_strat)) {
			subj->strategy_obj = new DefenderStrategy(subj, dodge_strat, special_strat);
		}
		else {
			cout << "Error: In Row " << rowCount
				<< ": Invalid Strategy Code [" << (*It)[start + 7] << "]" << endl;
			return false;
		}
	}
	else {
		cout << "Error: In Row " << rowCount
			<< ": Invalid Strategy Code [" << (*It)[start + 7] << "]" << endl;
		return false;
	}

	return true;
}



void print_headers(ostream & os, const string & prefix)
{
	os << prefix + "_name,";
	os << prefix + "_level,";
	os << prefix + "_atkIV,";
	os << prefix + "_defIV,";
	os << prefix + "_stmIV,";
	os << prefix + "_QuickMove,";
	os << prefix + "_ChargeMove,";
	os << prefix + "_Strategy";
}



UI::UI() : input_filename(NULL), output_filename(NULL)
{
	d_flag = false;
}

UI::~UI()
{
}

void UI::parse(const int argc, const char * const * argv)
{
	for (int i = 1; i < argc; ++i) {
		const char * cur_arg = argv[i];
		if (strlen(cur_arg) == 2 && cur_arg[0] == '/') {
			switch (cur_arg[1]) {
			case 'o':
				if (i + 1 < argc)
					output_filename = argv[++i];
				break;
			case 'i':
				if (i + 1 < argc)
					input_filename = argv[++i];
				break;
			case 'd':
				d_flag = true;
				break;
			default:
				break;
			}
		}
	}
	if (!input_filename)
		input_filename = DEFAULT_INPUT_FILENAME;
	if (!output_filename) {
		if (!d_flag)
			output_filename = DEFAULT_OUTPUT_FILENAME;
		else
			output_filename = DEFAULT_OUTPUT_FILENAME2;
	}
}

void UI::run()
{
	cout << "Input: " << input_filename << endl;
	cout << "Output: " << output_filename << endl;

	ifstream ifs(input_filename);
	ofstream ofs(output_filename);
	CSVIterator It(ifs);

	Attacker* attacker = NULL;
	Defender* defender = NULL;

	int rowCount = 0;

	// The input format should be: (A - attacker; D - defender)
	// [@A_ID], [@A_level], [@A_ivAtk], [@A_ivDef], [@A_ivStm], [@A_QuickMove], [@A_ChargeMove], [@A_strategy],
	// [@D_ID], [@D_level], [@D_ivAtk], [@D_ivDef], [@D_ivStm], [@D_QuickMove], [@D_ChargeMove], [@D_strategy]

	if (!d_flag) {
		// The output format for non-detailed is
		// [@time], [@time_to_win], [@KD_first], [@KD_final], [@prestige]
		print_headers(ofs, "Attacker");
		ofs << ',';
		print_headers(ofs, "Defender");
		ofs << ',';
		ofs << "Time,Time_to_win,KD_fisrt,KD_final,Prestige" << endl;
	}


	for (; It != CSVIterator(); ++It, ++rowCount) {

		delete attacker;
		delete defender;

		if (It->size() < 16) {
			cout << "Error: In Row " << rowCount << ": This row doesn't have enough data. Minimum is 16" << endl;
		}

		// Setting the attacker
		map<string, Pokemon>::iterator mapIt = POKEMON_LIST.find((*It)[0]);
		if (mapIt != POKEMON_LIST.end()) {
			attacker = new Attacker(mapIt->second);
			if (!set_combatant(It, attacker, 0, rowCount)) {
				continue;
			}
		}
		else {
			if (rowCount)
				cout << "Error: In Row " << rowCount << ": No Pokemon named [" << (*It)[0] << "]" << endl;
			continue;
		}


		// Setting the defender
		mapIt = POKEMON_LIST.find((*It)[8]);
		if (mapIt != POKEMON_LIST.end()) {
			defender = new Defender(mapIt->second);
			if (!set_combatant(It, defender, 8, rowCount)) {
				continue;
			}
		}
		else {
			if (rowCount)
				cout << "Error: In Row " << rowCount << ": No Pokemon named [" << (*It)[8] << "]" << endl;
			continue;
		}

		// Run the simulation and print it to output
		if (d_flag) {
			// With battle log
			BattleResult2 res(attacker, defender);
			res.run_simulation();
			ofs << res;
		}
		else {
			// Without battle log
			BattleResult res(attacker, defender);
			res.run_simulation();
			ofs << *attacker << ',' << *defender << ',' << res << endl;
		}


	}

	delete attacker;
	delete defender;
	ifs.close();
	ofs.close();
}
