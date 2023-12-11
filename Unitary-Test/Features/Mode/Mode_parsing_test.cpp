#include "doctest.hpp"
#include "../../../Mandatory/Commands/MODE/Mode_parsing.cpp"

TEST_SUITE("Test of Mode_parsing.cpp file")
{
	TEST_CASE("CheckModeFormat_function")
	{
		std::string	Modes;

		SUBCASE("TEST_1 true")
		{
			Modes = "+itkol";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_2 true")
		{
			Modes = "+i";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_3 true")
		{
			Modes = "-itkol";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_4 true")
		{
			Modes = "-i";
			CHECK(CheckModeFormat(Modes) == true);
		}
		SUBCASE("TEST_4 false")
		{
			Modes = "";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_5 false")
		{
			Modes = "-";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_6 false")
		{
			Modes = "+";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_7 false")
		{
			Modes = "+j";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_8 false")
		{
			Modes = "-j";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_9 false")
		{
			Modes = "-itp";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_10 false")
		{
			Modes = "-itk+l";
			CHECK(CheckModeFormat(Modes) == false);
		}
		SUBCASE("TEST_11 false")
		{
			Modes = "-itkl+";
			CHECK(CheckModeFormat(Modes) == false);
		}
	}
	TEST_CASE("CreateNode_function")
	{
		CmdNode	Node;
		char	c;
		bool	Op;

		SUBCASE("TEST_1")
		{
			c = 'i';
			Op = false;
			Node = CreateNode(Op, c);
			CHECK(Node.Op == Op);
			CHECK(Node.Mode == c);
		}
		SUBCASE("TEST_2")
		{
			c = 'k';
			Op = true;
			Node = CreateNode(Op, c);
			CHECK(Node.Op == Op);
			CHECK(Node.Mode == c);
		}
	}
	TEST_CASE("InsertParameter_function")
	{
		CmdNode	Node;
		std::string	Param;

		SUBCASE("TEST_1, don't need param")
		{
			Node.Op = false;
			Node.Mode = 'i';
			CHECK(InsertParameter(Node, "") == 0);
			CHECK(Node.Param == "");
		}
		SUBCASE("TEST_2, don't need param, but send one")
		{
			Node.Op = false;
			Node.Mode = 't';
			CHECK(InsertParameter(Node, "HEY HEY HEY") == 0);
			CHECK(Node.Param == "");
		}
		SUBCASE("TEST_3, need param")
		{
			Node.Op = false;
			Node.Mode = 'l';
			CHECK(InsertParameter(Node, "The param") == 1);
			CHECK(Node.Param == "The param");
		}
		SUBCASE("TEST_4, need param")
		{
			Node.Op = true;
			Node.Mode = 'k';
			CHECK(InsertParameter(Node, "The param") == 1);
			CHECK(Node.Param == "The param");
		}
	}
	TEST_CASE("CreateModes_function")
	{
		std::vector<std::string>		Cmd;
		std::list<CmdNode>				Modes;
		std::list<CmdNode>::iterator	It;

		SUBCASE("Test_1, no parameters, one mode")
		{
//			asm("int $3");
			Cmd = split("#chan +i", " ");
			REQUIRE(Cmd.empty() == false);
			CreateModes(Cmd, Modes);
			REQUIRE(Modes.size() == 1);
			It = Modes.begin();
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
		}
		SUBCASE("Test_2, no parameters, 2 modes")
		{
//			asm("int $3");
			Cmd = split("#chan +it", " ");
			REQUIRE(Cmd.empty() == false);
			CreateModes(Cmd, Modes);
			REQUIRE(Modes.size() == 2);
			It = Modes.begin();
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 't');
			CHECK((*It).Param == "");
		}
		SUBCASE("Test_3, one parameter, 2 modes")
		{
//			asm("int $3");
			Cmd = split("#chan +il 5", " ");
			REQUIRE(Cmd.empty() == false);
			CreateModes(Cmd, Modes);
			REQUIRE(Modes.size() == 2);
			It = Modes.begin();
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'l');
			CHECK((*It).Param == "5");
		}
		SUBCASE("Test_4, more parameter, more modes")
		{
//			asm("int $3");
			Cmd = split("#chan +ilk 5,mdp", " ");
			REQUIRE(Cmd.empty() == false);
			CreateModes(Cmd, Modes);
			REQUIRE(Modes.size() == 3);
			It = Modes.begin();
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'l');
			CHECK((*It).Param == "5");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'k');
			CHECK((*It).Param == "mdp");
		}
		SUBCASE("Test_5, no parameter, more modes")
		{
//			asm("int $3");
			Cmd = split("#chan +ilk", " ");
			REQUIRE(Cmd.empty() == false);
			CreateModes(Cmd, Modes);
			REQUIRE(Modes.size() == 3);
			It = Modes.begin();
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'l');
			CHECK((*It).Param == "");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'k');
			CHECK((*It).Param == "");
		}
		SUBCASE("Test_6, no parameter, same modes")
		{
//			asm("int $3");
			Cmd = split("#chan +ii", " ");
			REQUIRE(Cmd.empty() == false);
			CreateModes(Cmd, Modes);
			REQUIRE(Modes.size() == 2);
			It = Modes.begin();
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
		}
		SUBCASE("Test_7, more parameter, more modes")
		{
//			asm("int $3");
			Cmd = split("#chan +ilk 5", " ");
			REQUIRE(Cmd.empty() == false);
			CreateModes(Cmd, Modes);
			REQUIRE(Modes.size() == 3);
			It = Modes.begin();
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'i');
			CHECK((*It).Param == "");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'l');
			CHECK((*It).Param == "5");
			It++;
			CHECK((*It).Op == true);
			CHECK((*It).Mode == 'k');
			CHECK((*It).Param == "");
		}
	}
	TEST_CASE("ParsingModeCmd_function")
	{
		Server						serv;
		Client						*client;
		std::vector<std::string>	Cmd;
		std::list<CmdNode>			Modes;

		SUBCASE("TEST_0 ERR_NEEDMOREPARAMS")
		{
//			asm("int $3");
			Cmd = split("/MODE", " ");
			client = serv.GetClient("Jean");
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == ERR_NEEDMOREPARAMS);	
		}
		SUBCASE("TEST_1 ERR_NOSUCHCHANNEL")
		{
//			asm("int $3");
			Cmd = split("/MODE lol", " ");
			client = serv.GetClient("Jean");
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == ERR_NOSUCHCHANNEL);	
		}
		SUBCASE("TEST_2 ERR_NOTONCHANNEL")
		{
//			asm("int $3");
			Cmd = split("/MODE #Test", " ");
			client = new Client();
			client->SetNickname("George");
			serv.AddClient(client);
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == ERR_NOTONCHANNEL);	
		}
		SUBCASE("TEST_3 ERR_CHANOPRIVSNEEDED")
		{
//			asm("int $3");
			Cmd = split("/MODE #Test", " ");
			client = serv.GetClient("Jean");
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == ERR_CHANOPRIVSNEEDED);	
		}
		SUBCASE("TEST_4 RPL_CHANNELMODEIS")
		{
//			asm("int $3");
			Cmd = split("/MODE #Test", " ");
			client = serv.GetClient("sudo");
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == RPL_CHANNELMODEIS);	
		}
		SUBCASE("TEST_5 ERR_UNKNOWNMODE")
		{
//			asm("int $3");
			Cmd = split("/MODE #Test fmmf", " ");
			client = serv.GetClient("sudo");
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == ERR_UNKNOWNMODE);	
		}
		SUBCASE("TEST_6 0")
		{
//			asm("int $3");
			Cmd = split("/MODE #Test +itklo", " ");
			client = serv.GetClient("sudo");
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == 0);	
		}
		SUBCASE("TEST_7 0.1")
		{
//			asm("int $3");
			Cmd = split("/MODE #Test +itklo chichi,beignet,churros", " ");
			client = serv.GetClient("sudo");
			if (client == NULL)
				REQUIRE(true == false);
			CHECK(ParsingModeCmd(serv, *client, Cmd, Modes) == 0);	
		}
	}
}
