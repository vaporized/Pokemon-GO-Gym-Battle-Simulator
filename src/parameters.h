#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>

/* PROJECT PARAMETERS */

const std::string CPM_TABLE_FILEPATH = "PoGoData/cpm.csv";
const std::string TYPE_TABLE_FILEPATH = "PoGoData/typeTable.csv";
const std::string QUICK_MOVES_LIST_FILEPATH = "PoGoData/quickMoves.csv";
const std::string CHARGE_MOVES_LIST_FILEPATH = "PoGoData/chargeMoves.csv";
const std::string POKEMON_DATA_FILEPATH = "PoGoData/pokemonStats.csv";
const std::string GAME_PARAMETERS_FILEPATH = "PoGoData/gameParameters.csv";

const char* const DEFAULT_INPUT_FILENAME = "PoGoGymSim_Default_Input.csv";
const char* const DEFAULT_OUTPUT_FILENAME = "PoGoGymSim_Default_Output.csv";
const char* const DEFAULT_OUTPUT_FILENAME2 = "PoGoGymSim_Default_Output.txt";

const int COLUMN_WIDTH = 12;


/* GAME PARAMETERS*/
// These variables are defined in resources.cpp

extern int DODGE_MODIFIER;

extern double STAB_MODIFIER;
extern int MAX_ENERGY;

extern int BATTLE_START_TIME_MS;
extern int MAX_BATTLE_TIME_MS;
extern int SWITCHING_TIME_MS;

extern int DEFENDER_DELAY_MEAN_MS;
extern int DEFENDER_DELAY_STDEV_MS;

extern int DODGE_COOLDOWN_MS;
extern int CHARGING_TIME_MS;
extern int DODGE_WINDOW_MS;


#endif // !


