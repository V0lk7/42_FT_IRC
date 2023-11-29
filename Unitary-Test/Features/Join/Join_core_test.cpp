#include "doctest.hpp"

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "../../../Mandatory/Commands/JOIN/Join.cpp"
#include "Utils.hpp"


TEST_SUITE("Test of Join Main file")
{
	TEST_CASE("Join_function")
	{
		Server	server("");
		Client	*cli;
		Channel	*chan;
		std::string	reply;
		std::string	cmd;

		Setup(server, 1, 3);
		chan = server.GetChannel("#Chan0");
		chan->AddClientToChannel(*(server.GetClient("Client0")), true);
		chan->AddClientToChannel(*(server.GetClient("Client1")), true);
		cli = server.GetClient("Client2");

		SUBCASE("Test1: Valid client, join existing channel.")
		{
			cmd = "/JOIN #Chan0\r\n";
			Join(server, *cli, cmd);
			reply = ":Client2 JOIN #Chan0.\r\n";
			CHECK(server.GetClient("Client0")->GetMessage() == reply);
			CHECK(server.GetClient("Client1")->GetMessage() == reply);
			CHECK(chan->GetUsers().size() == 3);
		}
		SUBCASE("Test2: Valid client, join new channel.")
		{
			cmd = "/JOIN #Chan1\r\n";
			Join(server, *cli, cmd);
			chan = server.GetChannel("#Chan1");
			CHECK(chan->GetUsers().size() == 1);
		}
		SUBCASE("Test3: Valid client, join existing and new")
		{
			cmd = "/JOIN #Chan0,&Chan1\r\n";
			Join(server, *cli, cmd);
			reply = ":Client2 JOIN #Chan0.\r\n";
			CHECK(server.GetClient("Client0")->GetMessage() == reply);
			CHECK(server.GetClient("Client1")->GetMessage() == reply);
			CHECK(chan->GetUsers().size() == 3);
			chan = server.GetChannel("&Chan1");
			CHECK(chan->GetUsers().size() == 1);
		}
		SUBCASE("Test4: Valid client, wrong format cmd")
		{
			cmd = "/JOIN Chan0\r\n";
			reply = ": 461 Client2 JOIN :Bad Channel name.\r\n";
			Join(server, *cli, cmd);
			CHECK(cli->GetMessage() == reply);
		}
		SUBCASE("Test5: Valid client, wrong format cmd")
		{
			cmd = "/JOIN\r\n";
			reply = ": 461 Client2 JOIN :Need more parameters.\r\n";
			Join(server, *cli, cmd);
			CHECK(cli->GetMessage() == reply);
		}
	}
}
