#ifndef __UTILITY_H
#define __UTILITY_H

#include <iterator>
#include <type_traits>
#include <utility>


/*
Chect whether _Iter is an iterator. Value true if it is an iterator or poiter type.
Otherwise value is equal to false;
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

#endif // !__UTILITY_H

