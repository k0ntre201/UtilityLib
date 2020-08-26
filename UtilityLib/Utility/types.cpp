#include "types.hpp"

UtilityLib::Types::uint128_t::uint128_t():
	m_bytes{0,0}
{
}

UtilityLib::Types::uint128_t::uint128_t(int val)
{
	m_bytes[0] = 0;
	m_bytes[1] = val;
}

UtilityLib::Types::uint128_t::uint128_t(std::array<uint64_t, 2> val) :
	m_bytes{val}
{
}

UtilityLib::Types::uint128_t::uint128_t(uint128_t& val)
{
	m_bytes = val.m_bytes;
}


UtilityLib::Types::uint128_t::uint128_t(uint128_t&& val) noexcept
{
	m_bytes = std::move(val.m_bytes);
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator+=(uint128_t val)
{
	uint64_t val1 = val.m_bytes[1] + m_bytes[1];
	uint64_t val0 = val.m_bytes[0] + m_bytes[0];

	if (val1 < m_bytes[1])
	{
		val0 += val1;
		m_bytes[1] = std::numeric_limits<uint64_t>::max();
	}

	else
	{
		m_bytes[1] = val1;
	}

	if (val0 < m_bytes[0])
	{
		m_bytes[1] = val0;
		return *this;
	}

	m_bytes[0] = val0;

	return *this;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator-=(uint128_t val)
{
	uint64_t val1 = m_bytes[1] - val.m_bytes[1];
	uint64_t val0 = m_bytes[0] - val.m_bytes[0];

	if (val0 > m_bytes[0])
	{
		uint64_t diff = std::numeric_limits<uint64_t>::max() - val0;
		val1 -= diff;
		m_bytes[0] = 0;
	}

	else
	{
		m_bytes[0] = val0;
	}

	if (val1 > m_bytes[1])
	{
		m_bytes[1] = 0;
		m_bytes[0] = val1;
	}

	else
	{
		m_bytes[1] = val1;
	}

	return *this;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator+=(int val)
{
	uint128_t temp = val;
	(*this) += temp;
	return *this;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator-=(int val)
{
	uint128_t temp = val;
	(*this) -= temp;
	return *this;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator=(uint128_t val)
{
	m_bytes = val.m_bytes;

	return *this;
}

UtilityLib::Types::uint128_t UtilityLib::Types::uint128_t::operator+(uint128_t val)
{
	uint128_t ret = *this;
	ret += val;
	return ret;
}

UtilityLib::Types::uint128_t UtilityLib::Types::uint128_t::operator-(uint128_t val)
{
	uint128_t ret = *this;
	ret -= val;
	return ret;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator=(int val)
{
	m_bytes[1] = val;
	m_bytes[0] = 0;
	return *this;
}

UtilityLib::Types::uint128_t UtilityLib::Types::uint128_t::operator+(int val)
{
	return uint128_t();
}

UtilityLib::Types::uint128_t UtilityLib::Types::uint128_t::operator-(int val)
{
	return uint128_t();
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator++()
{
	(*this) += 1;
	return *this;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator++(int)
{
	uint128_t temp = (*this);
	temp += 1;
	(*this) = temp;
	return *this;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator--()
{
	(*this) -= 1;
	return *this;
}

UtilityLib::Types::uint128_t& UtilityLib::Types::uint128_t::operator--(int)
{
	uint128_t temp = (*this);
	temp -= 1;
	(*this) = temp;
	return *this;
}
