#include <iostream>
#include "ui.h"

using namespace std;



int main(int argc, char** argv) {

	init_cpm_table();
	init_types();
	init_moves_list();
	init_pokemon_list();



	UI ui;
	ui.parse(argc, argv);
	ui.run();



	uninit_moves_list();

	return 0;
}