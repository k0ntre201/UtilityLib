#include "uuidGenerator.hpp"

UtilityLib::Types::uint128_t UtilityLib::Generators::uuidGenerator::v4()
{
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());
    return Types::uint128_t({ dist(gen), dist(gen) });
}
