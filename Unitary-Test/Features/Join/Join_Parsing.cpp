#include "doctest.hpp"

#include <vector>

#include "../../../Mandatory/Commands/JOIN/Parsing.cpp"

static void	InitVectorTest(	std::vector<std::string> &src,
							std::string const &Chan,
							std::string const &Key,
							size_t size)
{
	src.clear();
	src.push_back(Chan);
	if (size > 1)
		src.push_back(Key);
}

TEST_SUITE("PARSING")
{
	TEST_CASE("CheckDelimFormat_function")
	{
		CHECK(CheckDelimFormat("#chan1,#chan2,#chan3", ',') == true);
		CHECK(CheckDelimFormat("#chan1", ',') == true);
		CHECK(CheckDelimFormat(",#chan1", ',') == false);
		CHECK(CheckDelimFormat(",,,,", ',') == false);
		CHECK(CheckDelimFormat("#chan1,,#chan2", ',') == false);
		CHECK(CheckDelimFormat("#chan1,#chan2,", ',') == false);
		CHECK(CheckDelimFormat("#chan1,#chan2,,", ',') == false);
	}

	TEST_CASE("VerifyParamsFormat_function")
	{
		std::vector<std::string>	Test;

		InitVectorTest(Test, "#chan1,#chan2", "key1,key2", 2);
		CHECK(VerifyParamsFormat(Test) == true);

		InitVectorTest(Test, "#chan1,#chan2", "key1", 2);
		CHECK(VerifyParamsFormat(Test) == true);

		InitVectorTest(Test, "#chan1,#chan2", "", 2);
		CHECK(VerifyParamsFormat(Test) == true);

		InitVectorTest(Test, "#chan1", "", 2);
		CHECK(VerifyParamsFormat(Test) == true);

		InitVectorTest(Test, "#chan1,#chan2", ",key1", 2);
		CHECK(VerifyParamsFormat(Test) == false);

		InitVectorTest(Test, "#chan1,#chan2", "key1,,key2", 2);
		CHECK(VerifyParamsFormat(Test) == false);

		InitVectorTest(Test, "#chan1,#chan2", "key2,", 2);
		CHECK(VerifyParamsFormat(Test) == false);

		InitVectorTest(Test, "#chan1,#chan2", "", 1);
		CHECK(VerifyParamsFormat(Test) == true);

		InitVectorTest(Test, "#chan1,,#chan2", "", 1);
		CHECK(VerifyParamsFormat(Test) == false);

		InitVectorTest(Test, ",#chan1,#chan2", "", 1);
		CHECK(VerifyParamsFormat(Test) == false);
	}

	TEST_CASE("DivideParamsType_function")
	{
		std::vector<std::string>	CmdParts;
		std::vector<std::string>	Channel;
		std::vector<std::string>	Key;

		SUBCASE("One Channel, No Key")
		{
			InitVectorTest(CmdParts, "#chan1", "", 1);
			DivideParamsType(CmdParts, Channel, Key);
			CHECK(Channel.size() == 1);
			CHECK(Channel[0] == "#chan1");
			CHECK(Key.empty() == true);
		}
		SUBCASE("Two Channel, No Key")
		{
			InitVectorTest(CmdParts, "#chan1,#chan2", "", 1);
			DivideParamsType(CmdParts, Channel, Key);
			CHECK(Channel.size() == 2);
			CHECK(Channel[0] == "#chan1");
			CHECK(Channel[1] == "#chan2");
			CHECK(Key.empty() == true);
		}
	}
}
