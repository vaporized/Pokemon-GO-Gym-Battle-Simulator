#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>

/* PROJECT PARAMETERS */

const std::string CPM_TABLE_FILEPATH = "PoGoData/cpm.csv";
const std::string TYPE_TABLE_FILEPATH = "PoGoData/typeTable.csv";
const std::string QUICK_MOVES_LIST_FILEPATH = "PoGoData/quickMoves.csv";
const std::string CHARGE_MOVES_LIST_FILEPATH = "PoGoData/chargeMoves.csv";
const std::string POKEMON_DATA_FILEPATH = "PoGoData/pokemonStats.csv";

const char* const DEFAULT_INPUT_FILENAME = "PoGoGymSim_Default_Input.csv";
const char* const DEFAULT_OUTPUT_FILENAME = "PoGoGymSim_Default_Output.csv";
const char* const DEFAULT_OUTPUT_FILENAME2 = "PoGoGymSim_Default_Output.txt";

const int COLUMN_WIDTH = 12;


/* GAME PARAMETERS*/

const int DODGE_MODIFIER = 4;

const double STAB_MODIFIER = 1.25;
const int MAX_ENERGY = 100;

const int BATTLE_START_TIME_MS = 0;
const int MAX_BATTLE_TIME_MS = 100000;
const int SWITCHING_TIME_MS = 800;
const int DEFENDER_EXPECTED_DELAY_MS = 2000;

const int DODGE_COOLDOWN_MS = 500;
const int CHARGING_TIME_MS = 600;
const int DODGE_WINDOW_MS = 700;


#endif // !


