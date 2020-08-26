#include "uuid.hpp"

#include "UUIDImplementation/uuidGenerator.hpp"

#include "utility.hpp"

UtilityLib::uuid::uuid(uuid& id)
{
    bytes = id.bytes;
}

UtilityLib::uuid::uuid(uuid&& id) noexcept
{
    bytes = std::move(id.bytes);
}

UtilityLib::uuid::uuid(Types::uint128_t val)
{
    bytes = val;
}

UtilityLib::uuid UtilityLib::uuid::get()
{
    return Generators::uuidGenerator::generate();
}

std::string UtilityLib::uuid::toString()
{
    std::array<uint32_t, 4> divider{ 8,4,4,4 };
    std::string hexRepresentation = int_to_hex<uint64_t>(bytes.m_bytes[0]) + int_to_hex<uint64_t>(bytes.m_bytes[1]);
    std::string hex2Representation;

    uint32_t pos{ 0 };
    for (auto d : divider)
    {
        hex2Representation += hexRepresentation.substr(pos, d);
        hex2Representation.push_back('-');
        pos += d;
    }
    hex2Representation += hexRepresentation.substr(pos, 12);

    return hex2Representation;
}
