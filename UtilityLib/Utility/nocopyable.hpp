#ifndef UTILITYLIB_UTILITY_NOCOPYABLE_HPP
#define UTILITYLIB_UTILITY_NOCOPYABLE_HPP

namespace UtilityLib
{
	class nocopyable
	{
	protected:
		constexpr nocopyable() = default;
		~nocopyable() = default;
	public:
		nocopyable(const nocopyable&) = delete;
		nocopyable& operator=(const nocopyable&) = delete;
	};
}

#endif // !UTILITYLIB_UTILITY_NOCOPYABLE_HPP

