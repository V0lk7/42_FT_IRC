#include "doctest.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include "Commands.hpp"

static bool	CheckMessage(Channel &chan, Client &cli, std::string reply);

TEST_SUITE("TEST Mode")
{
	TEST_CASE("Invalid command"){
		Server	serv;
		Client	*CliPtr = serv.GetClient("sudo");
		Channel	*ChanPtr = serv.GetChannel("#Test");
		std::string	cmd;
		std::string	reply;

		SUBCASE("Wrong channel"){
			cmd = "/MODE #mdr +il param";
			Mode(serv, *CliPtr, cmd);
			CHECK(CliPtr->GetMessage() == ": 403 MODE : no such channel.\r\n");
		}
		SUBCASE("Not on channel"){
			cmd = "/MODE #Test +il param";
			ChanPtr->EraseClientFromChannel(*CliPtr);
			Mode(serv, *CliPtr, cmd);
			CHECK(CliPtr->GetMessage() == ": 442 MODE : not on channel.\r\n");
		}
		SUBCASE("Unknow mode"){
			cmd = "/MODE #Test +ilkfioh param";
			Mode(serv, *CliPtr, cmd);
			CHECK(CliPtr->GetMessage() == ": 472 MODE : unknow mode.\r\n");
		}
		SUBCASE("Not such nickname"){
			cmd = "/MODE #Test +o param";
			Mode(serv, *CliPtr, cmd);
			CHECK(CliPtr->GetMessage() == ": 401 MODE #Test: no such nickname.\r\n");
		}
		SUBCASE("mode +o already set"){
			cmd = "/MODE #Test +o sudo";
			Mode(serv, *CliPtr, cmd);
			CHECK(CliPtr->GetMessage() == ": 467 MODE #Test: mode already set.\r\n");
		}
		SUBCASE("err parameters"){
			cmd = "/MODE #Test +o";
			Mode(serv, *CliPtr, cmd);
			CHECK(CliPtr->GetMessage() == ": 461 MODE #Test: parameter error.\r\n");
		}
		SUBCASE("set jimmy SuperUser + set limit a 10"){
			cmd = "/MODE #Test +ol Jimmy,10";
			reply = ": 324 sudo MODE #Test: set Jimmy operator.\r\n";
			reply += ": 324 sudo MODE #Test: set limit user.\r\n";
			Mode(serv, *CliPtr, cmd);
			CheckMessage(*ChanPtr, *CliPtr, reply);
			reply = ": 324 MODE #Test: set Jimmy operator.\r\n";
			reply += ": 324 MODE #Test: set limit user.\r\n";
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("Unset limit"){
			cmd = "/MODE #Test -l";
			reply = ": 324 sudo MODE #Test: unset limit user.\r\n";
			Mode(serv, *CliPtr, cmd);
			CheckMessage(*ChanPtr, *CliPtr, reply);
			reply = ": 324 MODE #Test: unset limit user.\r\n";
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("Set password"){
			cmd = "/MODE #Test +k lol";
			reply = ": 324 sudo MODE #Test: set channel password.\r\n";
			Mode(serv, *CliPtr, cmd);
			CheckMessage(*ChanPtr, *CliPtr, reply);
			reply = ": 324 MODE #Test: set channel password.\r\n";
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("Set already set password"){
			cmd = "/MODE #Test +k lol";
			ChanPtr->SetMode(PASSWORD_SET, true);
			Mode(serv, *CliPtr, cmd);
			reply = ": 467 MODE #Test: mode already set.\r\n";
			CHECK(CliPtr->GetMessage() == reply);
		}
	}
}

static bool	CheckMessage(Channel &chan, Client &cli, std::string reply)
{
	std::map<Client *, bool>			clmap = chan.GetUsers();
	std::map<Client *, bool>::iterator	It = clmap.begin();

	while (It != clmap.end()){
		if ((It->first) != &cli)
			if (It->first->GetMessage() != reply)
				return (false);
		It++;
	}
	return (true);
}
