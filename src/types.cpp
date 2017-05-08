#include "types.h"

PokemonType::PokemonType(string name) : name(name) {

}

// Compare whether they are the same type
bool PokemonType::operator==(const PokemonType& rhs) const {
	return name == rhs.name;
}

bool PokemonType ::operator!=(const PokemonType& rhs) const {
	return name != rhs.name;
}

bool PokemonType::operator>(const PokemonType & rhs) const
{
	return name > rhs.name;
}

bool PokemonType::operator>=(const PokemonType & rhs) const
{
	return name >= rhs.name;
}

bool PokemonType::operator<(const PokemonType & rhs) const
{
	return name < rhs.name;
}

bool PokemonType::operator<=(const PokemonType & rhs) const
{
	return name <= rhs.name;
}