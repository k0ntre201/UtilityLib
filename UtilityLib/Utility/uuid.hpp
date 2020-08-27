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
		uuid(const std::string& id);

		uuid& operator=(uuid& id);

		bool operator<(uuid& id);
		bool operator>(uuid& id);
		
		bool operator<=(uuid& id);
		bool operator>=(uuid& id);

		bool operator==(uuid& id);
		bool operator!=(uuid& id);


		std::string toString();
	
		static uuid v4();
		static uuid tryParse(const std::string& id);

		friend std::ostream& operator<<(std::ostream& os, uuid id)
		{
			os << id.toString();
			return os;
		}
	private:
		Types::uint128_t bytes;
	};
}