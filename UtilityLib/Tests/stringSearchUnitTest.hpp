#ifndef __STRING_SEARCH_UNIT_TEST
#define __STRING_SEARCH_UNIT_TEST

#include <functional>
#include <map>
#include <string>
#include <vector>

template <typename TestFunction>
bool testSubStringSearchAlgorithm(TestFunction f)
{
	std::string text("sialababamakniewiedzialajak");
	std::vector<std::pair<std::string, bool>> patterns =
	{
		{"baba", true},
		{"jak", true},
		{"makne", false},
		{"jaknababe", false}\
	};

	
	for (auto pattern : patterns)
	{
		std::string::const_iterator it = f(text, pattern.first);
		if (pattern.second != (std::end(text) != it))
		{
			std::cout << "Failed on pattern " << pattern.first << "\n";
			return false;
		}
	}

	return true;
}

#endif // !__STRING_SEARCH_UNIT_TEST

