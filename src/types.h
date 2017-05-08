#pragma once
#ifndef TYPE_H_
#define TYPE_H_

#include <string>

using namespace std;



class PokemonType {
public:
	PokemonType(string name = "None");

	// Comparison
	bool operator==(const PokemonType& rhs) const;
	bool operator!=(const PokemonType& rhs) const;
	bool operator>(const PokemonType& rhs) const;
	bool operator>=(const PokemonType& rhs) const;
	bool operator<(const PokemonType& rhs) const;
	bool operator<=(const PokemonType& rhs) const;

private:
	string name;

};



#endif // !TYPE_H_
