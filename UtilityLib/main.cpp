#include <iostream>

#include "Utility/stringSearch.hpp"
#include "Tests/stringSearchUnitTest.hpp"

int main()
{
	if (testSubStringSearchAlgorithm([](const std::string& s1, const std::string& s2) { return UtilityLib::stringSearcher::boyer_moore_horspool(s1, s2); }))
		std::cout << "Passed!\n";
	else
		std::cout << "Failed!\n";

	return 0;
}