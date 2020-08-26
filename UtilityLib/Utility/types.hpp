#pragma once

#include <array>
#include <iomanip>
#include <ostream>

namespace UtilityLib::Types
{
	class uint128_t
	{
	public:
		uint128_t();
		uint128_t(int val);
		uint128_t(std::array<uint64_t, 2> val);
		uint128_t(uint128_t& val);
		uint128_t(uint128_t&& val) noexcept;

		uint128_t& operator+=(uint128_t val);
		uint128_t& operator-=(uint128_t val);

		uint128_t& operator+=(int val);
		uint128_t& operator-=(int val);

		uint128_t& operator=(uint128_t val);
		uint128_t operator+(uint128_t val);
		uint128_t operator-(uint128_t val);

		uint128_t& operator=(int val);
		uint128_t operator+(int val);
		uint128_t operator-(int val);

		uint128_t& operator++();
		uint128_t& operator++(int);

		uint128_t& operator--();
		uint128_t& operator--(int);

		friend std::ostream& operator << (std::ostream& os, const uint128_t& val)
		{
			if (val.m_bytes[0] > 0)
			{
				os.fill('0');
				os << val.m_bytes[0];
				os << std::setw(20) << val.m_bytes[1];
			}

			else
			{
				os << val.m_bytes[1];
			}
			return os;
		}

	public:
		std::array<uint64_t, 2> m_bytes;
	};
}
