#include "resources.h"

map<PokemonLevel, double> CPM_TABLE;
vector<PokemonType> TYPE_LIST;
map<PokemonType, map<PokemonType, double>> TYPE_TABLE;
map<string, QuickMove*> QUICK_MOVES_LIST;
map<string, ChargeMove*> CHARGE_MOVES_LIST;
map<string, Pokemon> POKEMON_LIST;



void init_cpm_table(string filepath) {

	ifstream ifs(filepath);
	CSVIterator csvIt(ifs);

	while (csvIt != CSVIterator()) {
		size_t found = (*csvIt)[0].find('.'); // if the level is halfed, found is true
		CPM_TABLE[PokemonLevel(int(stod((*csvIt)[0])), found != string::npos)] = stod((*csvIt)[1]);
		csvIt++;
	}

	ifs.close();
}



void init_types(string filepath) {

	ifstream ifs(filepath);
	CSVIterator csvIt(ifs);

	for (size_t i = 0; i < csvIt->size(); ++i) {
		PokemonType curType((*csvIt)[i]);
		TYPE_LIST.push_back(curType);
		TYPE_TABLE[curType] = map<PokemonType, double>();
	}

	while (csvIt != CSVIterator()) {
		++csvIt;
		PokemonType curType((*csvIt)[0]);
		for (size_t i = 1; i < csvIt->size(); ++i)
			TYPE_TABLE[curType][TYPE_LIST[i - 1]] = stod((*csvIt)[i]);
	}

	ifs.close();
}




void init_moves_list(string filepath1, string filepath2) {
	ifstream ifs_qm(filepath1);
	ifstream ifs_cm(filepath2);
	CSVIterator csvIt1(ifs_qm), csvIt2(ifs_cm);

	// Skip the first line (headers)
	csvIt1++;
	csvIt2++;

	while (csvIt1 != CSVIterator()) {
		QUICK_MOVES_LIST[(*csvIt1)[0]] = new QuickMove((*csvIt1)[0], PokemonType((*csvIt1)[1]),
			stoi((*csvIt1)[2]), stoi((*csvIt1)[3]), stoi((*csvIt1)[4]), stoi((*csvIt1)[5]), stoi((*csvIt1)[6]));
		++csvIt1;
	}

	while (csvIt2 != CSVIterator()) {
		CHARGE_MOVES_LIST[(*csvIt2)[0]] = new ChargeMove((*csvIt2)[0], PokemonType((*csvIt2)[1]),
			stoi((*csvIt2)[2]), stoi((*csvIt2)[3]), stoi((*csvIt2)[4]), stoi((*csvIt2)[5]), stoi((*csvIt2)[6]));
		++csvIt2;
	}

	ifs_qm.close();
	ifs_cm.close();
}


void uninit_moves_list() {
	for (map<string, QuickMove*>::iterator it = QUICK_MOVES_LIST.begin(); it != QUICK_MOVES_LIST.end(); ++it)
		delete it->second;
	for (map<string, ChargeMove*>::iterator it = CHARGE_MOVES_LIST.begin(); it != CHARGE_MOVES_LIST.end(); ++it)
		delete it->second;
}


void init_pokemon_list(string filepath) {
	ifstream ifs(filepath);
	CSVIterator csvIt(ifs);

	// Skip the first row (headers)
	csvIt++;

	while (csvIt != CSVIterator()) {
		POKEMON_LIST[(*csvIt)[1]] = Pokemon((*csvIt)[1], PokemonType((*csvIt)[2]), PokemonType((*csvIt)[3]),
			stoi((*csvIt)[4]), stoi((*csvIt)[5]), stoi((*csvIt)[6]));
		++csvIt;
	}

	ifs.close();
}