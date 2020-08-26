#pragma once

#include <array>
#include <string>

#include "types.hpp"

namespace UtilityLib
{
	class uuid
	{
	public:
		uuid() = default;
		uuid(uuid&);
		uuid(uuid&&) noexcept;
		uuid(Types::uint128_t val);

		static uuid get();

		std::string toString();
	private:
		Types::uint128_t bytes;
	};
}