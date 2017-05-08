#ifndef CPM_H_
#define CPM_H_

#include <iostream>
using namespace std;


// To avoid "double as key" issue
class PokemonLevel {
public:
	PokemonLevel(int floored = 1, bool half = false);

	bool operator==(const PokemonLevel& other) const;
	bool operator!=(const PokemonLevel& other) const;
	bool operator>(const PokemonLevel& other) const;
	bool operator>=(const PokemonLevel& other) const;
	bool operator<(const PokemonLevel& other) const;
	bool operator<=(const PokemonLevel& other) const;

	friend ostream& operator<<(ostream&, const PokemonLevel&);


private:
	int floored;
	bool half;

	int cmp(const PokemonLevel& other) const;

};



#endif // !CPM_H_

