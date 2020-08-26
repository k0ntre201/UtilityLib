#include <iostream>

#include "Utility/uuid.hpp"

int main()
{
	UtilityLib::uuid id = UtilityLib::uuid::get();

	std::cout << id.toString() << "\n";
	std::cout << UtilityLib::uuid::get().toString() << "\n";
	std::cout << UtilityLib::uuid::get().toString() << "\n";

	return 0;
}