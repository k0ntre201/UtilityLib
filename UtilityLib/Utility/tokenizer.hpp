#ifndef UTILITYLIB_UTILITY_TOCKENIZER_HPP
#define UTILITYLIB_UTILITY_TOCKENIZER_HPP

#include "utility.hpp"

#include <algorithm>
#include <string>

namespace UtilityLib::tokenizer 
{

	template <typename Vec, typename Iter, typename Trimmer, typename = typename
		std::enable_if<is_iterator<Iter>::value>::type>
	Vec tokenize(Iter start, Iter end, Trimmer trim)
	{
		Vec vec;

		std::string tr(trim);

		while (start != end)
		{
			Iter it = std::find_first_of(start, end, tr.begin(), tr.end());
			std::string token(start, it);
			start = it;
			if (!token.empty())
			{
				vec.push_back(token.c_str());
				if (start != end)
				{
					start += tr.size();
				}

				else
					break;
			}
		}

		return vec;
	}
}

#endif // !UTILITYLIB_UTILITY_TOCKENIZER_HPP

