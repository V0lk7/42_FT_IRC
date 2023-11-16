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
		SUBCASE("one Channel, one Key")
		{
			InitVectorTest(CmdParts, "#chan1", "key1", 2);
			DivideParamsType(CmdParts, Channel, Key);
			CHECK(Channel.size() == 1);
			CHECK(Key.size() == 1);
			CHECK(Channel[0] == "#chan1");
			CHECK(Key[0] == "key1");
		}
		SUBCASE("one Channel, two Key")
		{
			InitVectorTest(CmdParts, "#chan1", "key1,key2", 2);
			DivideParamsType(CmdParts, Channel, Key);
			CHECK(Channel.size() == 1);
			CHECK(Key.size() == 2);
			CHECK(Channel[0] == "#chan1");
			CHECK(Key[0] == "key1");
			CHECK(Key[1] == "key2");
		}
		SUBCASE("two Channel, two Key")
		{
			InitVectorTest(CmdParts, "#chan1,#chan2", "key1,key2", 2);
			DivideParamsType(CmdParts, Channel, Key);
			CHECK(Channel.size() == 2);
			CHECK(Key.size() == 2);
			CHECK(Channel[0] == "#chan1");
			CHECK(Channel[1] == "#chan2");
			CHECK(Key[0] == "key1");
			CHECK(Key[1] == "key2");
		}
	}


	TEST_CASE("AssignChannel_function")
	{
		std::map<std::string, std::string>				Request;
		std::map<std::string, std::string>::iterator	It;
		std::vector<std::string>						Channel;
		std::vector<std::string>						Key;

		SUBCASE("One Channel")
		{
			Channel.push_back("#chan1");
			AssignChannel(Request, Channel);
			CHECK(Request.size() == 1);
			CHECK((Request.begin())->first == "#chan1");
			CHECK((Request.begin())->second == "");
		}
		SUBCASE("Two Differents Channel")
		{
			Channel.push_back("#chan1");
			Channel.push_back("#chan2");
			AssignChannel(Request, Channel);
			CHECK(Request.size() == 2);
			It = Request.begin();
			CHECK(It->first == "#chan1");
			CHECK(It->second == "");
			It++;
			CHECK(It->first == "#chan2");
			CHECK(It->second == "");
		}
		SUBCASE("Two same Channel")
		{
			Channel.push_back("#chan1");
			Channel.push_back("#chan1");
			AssignChannel(Request, Channel);
			CHECK(Request.size() == 1);
			It = Request.begin();
			CHECK(It->first == "#chan1");
			CHECK(It->second == "");
		}
	}

	TEST_CASE("AssignKeyToChan_function")
	{
		std::map<std::string, std::string>				Request;
		std::map<std::string, std::string>::iterator	It;
		std::vector<std::string>						Key;

		SUBCASE("One Channel, zero Key")
		{
			Request["#chan1"] = "";
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "");
		}
		SUBCASE("One Channel, one Key")
		{
			Request["#chan1"] = "";
			Key.push_back("Key1");
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "Key1");
		}
		SUBCASE("One Channel, two Keys")
		{
			Request["#chan1"] = "";
			Key.push_back("Key1");
			Key.push_back("Key2");
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "Key1");
		}
		SUBCASE("two Channels, zero Key")
		{
			Request["#chan1"] = "";
			Request["#chan2"] = "";
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "");
			It++;
			CHECK(It->second == "");
		}
		SUBCASE("two Channels, one Key")
		{
			Request["#chan1"] = "";
			Request["#chan2"] = "";
			Key.push_back("Key1");
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "Key1");
			It++;
			CHECK(It->second == "Key1");
		}
		SUBCASE("two Channels, two Keys")
		{
			Request["#chan1"] = "";
			Request["#chan2"] = "";
			Key.push_back("Key1");
			Key.push_back("Key2");
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "Key1");
			It++;
			CHECK(It->second == "Key2");
		}
		SUBCASE("two Channels, two Keys")
		{
			Request["#chan1"] = "";
			Request["#chan2"] = "";
			Key.push_back("Key1");
			Key.push_back("Key2");
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "Key1");
			It++;
			CHECK(It->second == "Key2");
		}
		SUBCASE("three Channels, two Keys")
		{
			Request["#chan1"] = "";
			Request["#chan2"] = "";
			Request["#chan3"] = "";
			Key.push_back("Key1");
			Key.push_back("Key2");
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "Key1");
			It++;
			CHECK(It->second == "Key2");
			It++;
			CHECK(It->second == "");
		}
		SUBCASE("three Channels, three Keys")
		{
			Request["#chan1"] = "";
			Request["#chan2"] = "";
			Request["#chan3"] = "";
			Key.push_back("Key1");
			Key.push_back("Key2");
			Key.push_back("Key3");
			AssignKeyToChan(Request, Key);
			It = Request.begin();
			CHECK(It->second == "Key1");
			It++;
			CHECK(It->second == "Key2");
			It++;
			CHECK(It->second == "Key3");
		}
	}
}
