#include "pch.h"
#include "../UtilityLib/Utility/stringSearch.hpp"

TEST(UtilityStringSearchTest, BoyerMooreHorspol) 
{
	using namespace UtilityLib::stringSearcher;

	std::string text("sialababamakniewiedzialajak");
	std::vector<std::pair<std::string, bool>> patterns =
	{
		{"baba", true},
		{"jak", true},
		{"makne", false},
		{"jaknababe", false},
		{"lab", true}
	};


	for (auto pattern : patterns)
	{
		std::string::const_iterator it = boyer_moore_horspool(text, pattern.first);
		EXPECT_FALSE(pattern.second != (std::end(text) != it));
	}
}