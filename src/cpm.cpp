#include "cpm.h"


PokemonLevel::PokemonLevel(int floored, bool half) : floored(floored), half(half) {

}

bool PokemonLevel::operator==(const PokemonLevel& other) const {
	return cmp(other) == 0;
}
bool PokemonLevel::operator!=(const PokemonLevel& other) const {
	return cmp(other) != 0;
}
bool PokemonLevel::operator>(const PokemonLevel& other) const {
	return cmp(other) > 0;
}
bool PokemonLevel::operator>=(const PokemonLevel& other) const {
	return cmp(other) >= 0;
}
bool PokemonLevel::operator<(const PokemonLevel& other) const {
	return cmp(other) < 0;
}
bool PokemonLevel::operator<=(const PokemonLevel& other) const {
	return cmp(other) <= 0;
}


int PokemonLevel::cmp(const PokemonLevel& other) const {
	return floored == other.floored ? half - other.half : floored - other.floored;
}

std::ostream & operator<<(std::ostream & os, const PokemonLevel & obj)
{
	os << obj.floored;
	if (obj.half)
		os << ".5";
	return os;
}
