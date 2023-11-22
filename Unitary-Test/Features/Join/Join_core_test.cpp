#include "doctest.hpp"

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "../../../Mandatory/Commands/JOIN/Join.cpp"
#include "Utils.hpp"


TEST_SUITE("Test of Join Main file")
{
	TEST_CASE("HandleJoinChannel_function")
	{
		Server											server("Password");
		Client											*CliPtr;
		Channel											*ChanPtr;
		std::map<std::string, std::string>				JoinCmd;
		std::map<std::string, std::string>::iterator	It;
		
		SUBCASE("Not such a Channel, create a new one")
		{
			JoinCmd["Chan0"]="";
			Setup(server, 0, 1);
			CliPtr = server.GetClient("Client0");
			if (CliPtr == NULL)
				REQUIRE(true == false);
			It = JoinCmd.begin();
			HandleJoinChannel(server, *CliPtr, It);
			ChanPtr = server.GetChannel("Chan0");
			if (ChanPtr == NULL)
				REQUIRE(true == false);
		}

		SUBCASE("User already in channel")
		{
			JoinCmd["Chan0"]="";
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->AddClientToChannel(*CliPtr, false);
			It = JoinCmd.begin();
			HandleJoinChannel(server, *CliPtr, It);
		}

		SUBCASE("Channel is full")
		{
			JoinCmd["Chan0"]="";
			Setup(server, 1, 2);
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->AddClientToChannel(*CliPtr, false);
			ChanPtr->SetLimitUsers(1);
			CliPtr = server.GetClient("Client1");
			if (CliPtr == NULL)
				REQUIRE(true == false);
			It = JoinCmd.begin();
			HandleJoinChannel(server, *CliPtr, It);
		}

		SUBCASE("Wrong Password")
		{
			JoinCmd["Chan0"]="Badkey";
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->SetMode(PASSWORD_SET, true);
			ChanPtr->SetPassword("GoodKey");
			It = JoinCmd.begin();
			HandleJoinChannel(server, *CliPtr, It);
		}
		SUBCASE("Success_1")
		{
			JoinCmd["Chan0"]="Goodkey";
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->SetMode(PASSWORD_SET, true);
			ChanPtr->SetPassword("Goodkey");
			It = JoinCmd.begin();
			HandleJoinChannel(server, *CliPtr, It);
		}
		SUBCASE("Success_2")
		{
			JoinCmd["Chan0"]="";
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			It = JoinCmd.begin();
			HandleJoinChannel(server, *CliPtr, It);
		}
		SUBCASE("Success_3")
		{
			JoinCmd["Chan0"]="";
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			It = JoinCmd.begin();
			ChanPtr->SetMode(INVITE_ONLY, true);
			ChanPtr->PutClientOnWaitingList(*CliPtr);
			HandleJoinChannel(server, *CliPtr, It);
			CHECK(ChanPtr->UserInWaitingList(*CliPtr) == false);	
		}
		SUBCASE("Success_4")
		{
			JoinCmd["Chan0"]="key";
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			It = JoinCmd.begin();
			ChanPtr->SetMode(PASSWORD_SET, true);
			ChanPtr->SetMode(INVITE_ONLY, true);
			ChanPtr->SetPassword("key");
			ChanPtr->PutClientOnWaitingList(*CliPtr);
			HandleJoinChannel(server, *CliPtr, It);
			CHECK(ChanPtr->UserInWaitingList(*CliPtr) == false);	
		}
	}

	TEST_CASE("Join_function")
	{
		Server	server("");
		Client	*cli;
		Channel	*chan;
		std::string	reply;
		std::string	cmd;

		Setup(server, 1, 3);
		chan = server.GetChannel("Chan0");
		chan->AddClientToChannel(*(server.GetClient("Client0")), true);
		chan->AddClientToChannel(*(server.GetClient("Client1")), true);
		cli = server.GetClient("Client2");

		SUBCASE("Test1: Valid client, join existing channel.")
		{
			cmd = "/JOIN #Chan0\r\n";
			Join(server, *cli, cmd);
			reply = ":Client2 JOIN #Chan0\r\n";
			CHECK(server.GetClient("Client0")->GetMessage() == reply);
			CHECK(server.GetClient("Client1")->GetMessage() == reply);
			CHECK(chan->GetUsers().size() == 3);
		}
		SUBCASE("Test2: Valid client, join new channel.")
		{
			cmd = "/JOIN #Chan1\r\n";
			Join(server, *cli, cmd);
			chan = server.GetChannel("Chan1");
			CHECK(chan->GetUsers().size() == 1);
		}
		SUBCASE("Test3: Valid client, join existing and new")
		{
			cmd = "/JOIN #Chan0,&Chan1\r\n";
			Join(server, *cli, cmd);
			reply = ":Client2 JOIN #Chan0\r\n";
			CHECK(server.GetClient("Client0")->GetMessage() == reply);
			CHECK(server.GetClient("Client1")->GetMessage() == reply);
			CHECK(chan->GetUsers().size() == 3);
			chan = server.GetChannel("Chan1");
			CHECK(chan->GetUsers().size() == 1);
		}
		SUBCASE("Test4: Valid client, wrong format cmd")
		{
			cmd = "/JOIN Chan0\r\n";
			reply = ": 461 Client2 JOIN :Syntax error. Proper usage /JOIN [# | &]<channel_name> <key>\r\n";
			Join(server, *cli, cmd);
			CHECK(cli->GetMessage() == reply);
		}
		SUBCASE("Test5: Invalid client, Good format cmd")
		{
			cmd = "/JOIN #Chan0\r\n";
			Client	Invalid;
			reply = ": 451 :You have not registered. Please authenticate before executing commands.\r\n";
			Join(server, Invalid, cmd);
			CHECK(Invalid.GetMessage() == reply);
		}
	}
}
