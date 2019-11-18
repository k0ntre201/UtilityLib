#ifndef __STRING_SEARCH_h
#define __STRING_SEARCH_h

#include <exception>
#include <iterator>
#include <map>
#include <set>


namespace UtilityLib::stringSearcher {

	/*
	Function search occurence of pattern in string.
	Function return iterator to string where pattern exist
	If pattern do not occure in string then return end iterator of string.
	*/
	template <typename Iter>
	Iter boyer_moore_horspool(Iter sBegin, Iter sEnd, Iter patternBegin, Iter patternEnd)
	{
		auto stringDistance = sEnd - sBegin;
		auto patternDistance = patternEnd - patternBegin;

		if (stringDistance < 0 && patternDistance < 0)
		{
			throw std::runtime_error("string or pattern has swapped iterators");
		}

		if (patternDistance > stringDistance)
			throw std::runtime_error("Searching pattern is longer than string");

		// Get only one occurence of each letter in pattern
		std::set<char> patternLetterSet(patternBegin, patternEnd);

		// Calculate bad-0match table
		std::map<char, size_t> badMachTable;
		for (char letter : patternLetterSet)
		{
			badMachTable.insert(std::make_pair(letter, patternDistance));
		}
		for (size_t i{ 0 }; i < patternDistance - 1; ++i)
		{
			badMachTable[*(patternBegin + i)] = patternDistance - 1 - i;
		}

		size_t lastStringPosition = patternDistance - 1;

		while (true)
		{
			// If pattern does not exist in string then return end iterator
			if (lastStringPosition >= stringDistance)
				return sEnd;
			
			for (size_t i{0}; i < patternDistance; ++i)
			{
				char letterInPattern = *(patternBegin + patternDistance - 1 - i);
				char letterInString = *(sBegin + lastStringPosition - i);

				if (letterInPattern != letterInString)
				{
					auto it = badMachTable.find(letterInString);
					it != std::end(badMachTable) ? lastStringPosition += (badMachTable[letterInString] - i) : lastStringPosition += patternDistance;
					break;
				}

				// If we reach to the end of pattern then return iterator to beggining of pattern in string
				if (i == patternDistance - 1)
				{
					return sBegin + lastStringPosition - patternDistance + 1;
				}
			}
		}
	}

	template<typename _Str1, typename _Str2>
	auto boyer_moore_horspool(const _Str1& str,const  _Str2& pattern)
	{
		return boyer_moore_horspool(std::begin(str), std::end(str), std::begin(pattern), std::end(pattern));
	}
}

#endif // !__STRING_SEARCH_h
