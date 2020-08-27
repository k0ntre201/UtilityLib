#ifndef __UTILITY_H
#define __UTILITY_H

#include <iterator>
#include <type_traits>
#include <utility>
#include <string>
#include <sstream>
#include <iomanip>

/*
Check is _Iter an iterator type. True if is an iterator or pointer type.
*/

template <typename _Iter, typename = void>
struct is_iterator
{
	static constexpr bool value = false;
};

template <typename _Iter>
struct is_iterator<_Iter, typename std::enable_if<!std::is_same<typename std::iterator_traits<_Iter>::value_type, void>::value>::type>
{
	static constexpr bool value = true;
};

template <typename _Iter>
inline constexpr bool is_iterator_v = is_iterator<_Iter>::value;

template< typename T >
std::string int_to_hex(T i)
{
	std::stringstream stream;
	stream << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;
	return stream.str();
}

template <typename T> T hex_to_decimal(std::string s)
{
	T i;
	std::stringstream stream;
	stream << s;
	stream >> std::hex >> i;
	return i;
}

#endif // !__UTILITY_H

