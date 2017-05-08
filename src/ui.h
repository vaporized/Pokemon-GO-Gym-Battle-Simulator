#ifndef ANALYSIS_H_
#define ANALYSIS_H_


#include "gym.h"




ostream& operator<<(ostream& os, const list<resEvent>& log);
ostream& operator<<(ostream& os, const list<Event>& log);



// This class will run the gym simulation on its own,
// and presents you the metrics for results (All metrics are from the attacker's prospective)
// But still, you need to take care of both the attacker and the defender outside this class
class BattleResult {
public:
	BattleResult(Attacker*, Defender*);
	virtual ~BattleResult();

	// Gym simulation will be done here;
	void run_simulation();

	// Get the KD (in decimal number) of the first battle
	double get_KD_first() const;

	// Get the KD until the attacker faints. Same as KD_first if the attacker loses the first battle
	double get_KD_final() const;

	// Get the battle time (in seconds) of the first battle
	double get_timeS() const;

	// Get the battle time until the defender faints. Same as timeS if the defender loses the first battle
	double get_time_to_winS() const;

	// Get the battle result
	list<resEvent> get_battle_log() const;


	// Output the result to a csv file, with the format:
	// [@Time], [@time to win], [@KD_first], [@KD_final], [@prestige]
	friend ostream& operator<<(ostream&, const BattleResult&);


protected:
	double KD_first;
	double KD_final;

	double timeS;
	double time_to_winS;

	int prestige;

	Gym* gym;
	Attacker* attacker;
	Defender* defender;

	double get_simple_KD() const;
};





// Using Gym2 to support battle log
class BattleResult2: public BattleResult {
public:
	BattleResult2(Attacker*, Defender*);

	friend ostream& operator<<(ostream&, const BattleResult2&);

private:

};






/* Helper functions */

bool set_combatant(const CSVIterator&, Combatant*, int, int);

void print_headers(ostream&, const string&);



class UI {
public:
	UI();
	~UI();

	void parse(const int, const char* const *);


	// Run multiple simulations specified by a csv file (input_filename),
	// output the results of each matchup to another csv file (output_filename)
	// If [with_log] is set true, it will output each matchup's battle log; otherwise, just five basic metrics
	void run();

private:
	bool d_flag;
	const char* input_filename;
	const char* output_filename;

};






#endif // ! ANALYSIS_H_

