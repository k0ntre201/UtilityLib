#pragma once

#include <chrono>
#include <random>
#include "../types.hpp"

namespace UtilityLib::Generators
{
	class uuidGenerator
	{
	public:
		static Types::uint128_t generate();
	};
}