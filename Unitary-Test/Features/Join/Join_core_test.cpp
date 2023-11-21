#include "doctest.hpp"

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "../../../Mandatory/Commands/JOIN/Join.cpp"
#include "Utils.hpp"


TEST_SUITE("Test of Join Main core")
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
}
