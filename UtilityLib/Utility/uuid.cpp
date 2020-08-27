#include <algorithm>
#include <regex>
#include "tokenizer.hpp"
#include "utility.hpp"
#include "uuid.hpp"
#include "UUIDImplementation/uuidGenerator.hpp"
#include <vector>

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

UtilityLib::uuid::uuid(const std::string& id)
{
    auto val = tryParse(id);
    (*this) = val;
}

UtilityLib::uuid& UtilityLib::uuid::operator=(uuid& id)
{
    bytes = id.bytes;
    return *this;
}

bool UtilityLib::uuid::operator<(uuid& id)
{
    return toString() < id.toString();
}

bool UtilityLib::uuid::operator>(uuid& id)
{
    return toString() > id.toString();
}

bool UtilityLib::uuid::operator<=(uuid& id)
{
    return toString() <= id.toString();
}

bool UtilityLib::uuid::operator>=(uuid& id)
{
    return toString() >= id.toString();
}

bool UtilityLib::uuid::operator==(uuid& id)
{
    return toString() == id.toString();
}

bool UtilityLib::uuid::operator!=(uuid& id)
{
    return toString() != id.toString();
}

UtilityLib::uuid UtilityLib::uuid::v4()
{
    return Generators::uuidGenerator::v4();
}

UtilityLib::uuid UtilityLib::uuid::tryParse(const std::string& id)
{
    auto pattern = "[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}";
    auto rx = std::regex{ pattern, std::regex_constants::icase };

    if (std::regex_match(id, rx))
    {
        auto temp = id;
        temp.erase(std::remove(std::begin(temp), std::end(temp), '-'), std::end(temp));
        uint64_t v1 = hex_to_decimal<uint64_t>(temp.substr(0, 16));
        uint64_t v2 = hex_to_decimal<uint64_t>(temp.substr(16));
        return uuid(Types::uint128_t{ { v1, v2 } });
    }
   
    throw std::runtime_error("Wrong uuid format!");
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
