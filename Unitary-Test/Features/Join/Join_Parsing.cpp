#include "doctest.hpp"

#include <vector>
#include "../../../Mandatory/Commands/JOIN/Join_parsing.cpp"

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

static bool	CheckNode(	std::map<std::string, std::string>::iterator Node,
						std::string	first, std::string second)
{
	if (Node->first != first)
		return (false);
	if (Node->second != second)
		return (false);
	return (true);
}

TEST_SUITE("PARSING")
{
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
			AssignChannel(Request, Channel, Key);
			CHECK(Request.size() == 1);
			CHECK((Request.begin())->first == "#chan1");
			CHECK((Request.begin())->second == "");
		}
		SUBCASE("Two Differents Channel")
		{
			Channel.push_back("#chan1");
			Channel.push_back("&chan2");
			AssignChannel(Request, Channel, Key);
			CHECK(Request.size() == 2);
			It = Request.begin();
			CHECK(It->first == "#chan1");
			CHECK(It->second == "");
			It++;
			CHECK(It->first == "&chan2");
			CHECK(It->second == "");
		}
		SUBCASE("Two type ofChannel")
		{
			Channel.push_back("#chan1");
			Channel.push_back("&chan1");
			AssignChannel(Request, Channel, Key);
			CHECK(Request.size() == 2);
			It = Request.begin();
			CHECK(It->first == "#chan1");
			CHECK(It->second == "");
			It++;
			CHECK(It->first == "&chan1");
			CHECK(It->second == "");
		}
	}

	TEST_CASE("OrganiseRequest_function")
	{
		std::map<std::string, std::string>				Request;
		std::map<std::string, std::string>::iterator	It;
		std::vector<std::string>						Raw;
		std::string										Cmd;

		SUBCASE("TEST1: /JOIN #chan1 key1")
		{
			Cmd = "/JOIN #chan1 key1";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);

		}
		SUBCASE("TEST2: /JOIN")
		{
			Cmd = "/JOIN";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == false);

		}
		SUBCASE("TEST3: /JOIN #chan1 key1 key2")
		{
			Cmd = "/JOIN #chan1 key1 key2";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
		}
		SUBCASE("TEST4: /JOIN #chan1,,&chan2 key")
		{
			Cmd = "/JOIN #chan1,,&chan2 key";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
		}
		SUBCASE("TEST5: /JOIN #chan1,chan2 key")
		{
			Cmd = "/JOIN #chan1,chan2 key";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
		}
		SUBCASE("TEST6: /JOIN #chan1,&chan2 key")
		{
			Cmd = "/JOIN #chan1,&chan2 key";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
			CHECK(Request.size() == 2);
			It = Request.find("#chan1");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key");

			It = Request.find("&chan2");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key");
		}
		SUBCASE("TEST7: /JOIN #chan1,&chan2,#chan3 key1,key2")
		{
			Cmd = "/JOIN #chan1,&chan2,#chan3 key1,key2";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
			CHECK(Request.size() == 3);
			It = Request.find("#chan1");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key1");

			It = Request.find("&chan2");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key2");

			It = Request.find("#chan3");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "");
		}
		SUBCASE("TEST8: /JOIN #chan1,&chan2,#chan3 key1,key2,key3")
		{
			Cmd = "/JOIN #chan1,&chan2,#chan3 key1,key2,key3";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
			CHECK(Request.size() == 3);
			It = Request.find("#chan1");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key1");

			It = Request.find("&chan2");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key2");

			It = Request.find("#chan3");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key3");
		}
		SUBCASE("TEST9: /JOIN #chan,&chan1,#chan")
		{
			Cmd = "/JOIN #chan,&chan1,#chan";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
			CHECK(Request.size() == 2);
			It = Request.find("#chan");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "");

			It = Request.find("&chan1");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "");
		}
		SUBCASE("TEST10: /JOIN #,&,#chan1 key1,key2")
		{
			Cmd = "/JOIN ,#,,,,&,#chan1,, key1,key2";
			Raw = split(Cmd, " ");
			CHECK(OrganiseRequest(Request, Raw) == true);
			CHECK(Request.size() == 3);
			It = Request.find("#");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key1");

			It = Request.find("&");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "key2");

			It = Request.find("#chan1");
			if (It == Request.end())
				REQUIRE(true == false);
			CHECK(It->second == "");
		}
	}
}
