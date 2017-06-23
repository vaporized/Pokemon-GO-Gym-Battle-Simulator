#ifndef INIT_H_
#define INIT_H_

#include <map>
#include <vector>
#include <string>
#include <stdlib.h>

#include "csv.h"
#include "parameters.h"
#include "pokemon.h"

using namespace std;



// This map stores the CPM data from level 1 to 40 including halfed level
extern map<PokemonLevel, double> CPM_TABLE;

// This is the list of all available Pokemon Types
extern vector<PokemonType> TYPE_LIST;

// This nested map stores the type advantage/disvantage info
extern map<PokemonType, map<PokemonType, double>> TYPE_TABLE;

// These two maps stores every move (by pointer)
extern map<string, QuickMove*> QUICK_MOVES_LIST;
extern map<string, ChargeMove*> CHARGE_MOVES_LIST;


// This map stores every Pokemon's info including name, types and stats
// The key is the Pokemon's name
extern map<string, Pokemon> POKEMON_LIST;

// This function loads basic game parameters from a specified .csv file
void init_game_parameters(string filepath = GAME_PARAMETERS_FILEPATH);

// This function loads CPM data from a specifed .csv file
void init_cpm_table(string = CPM_TABLE_FILEPATH);

// This function loads type table from a specifed .csv file
void init_types(string = TYPE_TABLE_FILEPATH);

// This function loads moves data from a specifed .csv file
void init_moves_list(string = QUICK_MOVES_LIST_FILEPATH, string = CHARGE_MOVES_LIST_FILEPATH);

// This function frees up the memory
void uninit_moves_list();

// This function loads Pokemon data from a specifed .csv file
void init_pokemon_list(string = POKEMON_DATA_FILEPATH);



#endif // !INIT_H_


