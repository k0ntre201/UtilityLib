#include "pch.h"
#include "../UtilityLib/Utility/tokenizer.hpp"

TEST(TokenizerTest, TokenizeSimple)
{
	std::string testString("Ala ma kota a kot ma Ale");

	std::vector<std::string> tokens = UtilityLib::tokenizer::tokenize<std::vector<std::string>>(std::begin(testString), std::end(testString), " ");

	EXPECT_FALSE(tokens.empty());

	EXPECT_EQ(tokens.size(), 7);

	if (!tokens.empty())
	{
		EXPECT_EQ(tokens[2], "kota");
	}
}

TEST(TokenizerTest, TokenizeEndLine)
{
	std::string testString("Ala ma kota a kot ma Ale\r\nCo by sie stalo gdy sie nie stanie");

	std::vector<std::string> tokens = UtilityLib::tokenizer::tokenize<std::vector<std::string>>(std::begin(testString), std::end(testString), "\r\n");

	EXPECT_FALSE(tokens.empty());

	EXPECT_EQ(tokens.size(), 2);

	if (!tokens.empty())
	{
		EXPECT_EQ(tokens[0], "Ala ma kota a kot ma Ale");
	}
}
