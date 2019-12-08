#include <iostream>
#include <vector>
#include "Utility/tokenizer.hpp"

int main()
{
	std::string testString("Ala ma kota a kot ma Ale");
	std::vector<std::string> tokens = UtilityLib::tockenizer::tokenize<std::vector<std::string>>(std::begin(testString), std::end(testString), " ");
}