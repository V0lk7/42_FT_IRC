#include "doctest.hpp"

#include "../../../Mandatory/Commands/JOIN/Join.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Utils.hpp"

TEST_SUITE("Utilities")
{
	TEST_CASE("CreateNewChannel_function")
	{
		Server											server("Password");
		Client											*CliPtr;
		Channel											*ChanPtr;
		std::map<std::string, std::string>				JoinCmd;
		std::map<std::string, std::string>::iterator	It;

		std::map<Client *, bool>						Users;
		std::map<Client *, bool>::iterator				It2;

		SUBCASE("Create Channel with no password")
		{
			JoinCmd["Chan0"] = "";
			Setup(server, 0, 1);

			CliPtr = server.GetClient("Client0"); 
			if (CliPtr == NULL)
				REQUIRE(true == false);
			It = JoinCmd.begin();
			CreateNewChannel(server, *CliPtr, It);
			ChanPtr = server.GetChannel("Chan0"); 
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			Users = ChanPtr->GetUsers();
			CHECK(ChanPtr->GetPassword() == "");
			CHECK(Users.size() == 1);
			It2 = Users.find(CliPtr);
			REQUIRE(It2 != Users.end());
			CHECK(It2->second == true);
			for (int i = 0; i < 3; i++)
				CHECK(ChanPtr->GetMode(i) == false);
		}
		SUBCASE("Create Channel with passwd")
		{
			JoinCmd["Chan0"] = "lol";
			Setup(server, 0, 1);

			CliPtr = server.GetClient("Client0"); 
			if (CliPtr == NULL)
				REQUIRE(true == false);
			It = JoinCmd.begin();
			CreateNewChannel(server, *CliPtr, It);
			ChanPtr = server.GetChannel("Chan0"); 
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			Users = ChanPtr->GetUsers();
			CHECK(ChanPtr->GetPassword() == "lol");
			CHECK(Users.size() == 1);
			It2 = Users.find(CliPtr);
			REQUIRE(It2 != Users.end());
			CHECK(It2->second == true);
			CHECK(ChanPtr->GetMode(0) == false);
			CHECK(ChanPtr->GetMode(1) == true);
			CHECK(ChanPtr->GetMode(2) == false);
		}
	}

	TEST_CASE("UserInChannel_Method_Channel")
	{
		Server											server("Password");
		Client											*CliPtr;
		Channel											*ChanPtr;

		SUBCASE("YES")
		{
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			if (CliPtr == NULL)
				REQUIRE(true == false);
			ChanPtr = server.GetChannel("Chan0"); 
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->AddClientToChannel(*CliPtr, false);
			CHECK(ChanPtr->UserInChannel(*CliPtr) == true);
		}
		SUBCASE("NO")
		{
			Setup(server, 1, 1);
			CliPtr = server.GetClient("Client0");
			if (CliPtr == NULL)
				REQUIRE(true == false);
			ChanPtr = server.GetChannel("Chan0"); 
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			CHECK(ChanPtr->UserInChannel(*CliPtr) == false);
		}
	}

	TEST_CASE("VerifyChannelLimit_function")
	{
		Server	server("Password");
		Client	*CliPtr;
		Channel	*ChanPtr;


		SUBCASE("No limit")
		{
			Setup(server, 1, 0);
			ChanPtr = server.GetChannel("Chan0");
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			CHECK(VerifyChannelLimit(*ChanPtr) == true);
		}
		SUBCASE("Limit = 1, 0 client in it")
		{
			Setup(server, 1, 0);
			ChanPtr = server.GetChannel("Chan0");
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->SetLimitUsers(1);
			CHECK(VerifyChannelLimit(*ChanPtr) == true);
		}
		SUBCASE("Limit = 1, 1 client in it")
		{
			Setup(server, 1, 1);
			ChanPtr = server.GetChannel("Chan0");
			CliPtr = server.GetClient("Client0");
			if (ChanPtr == NULL || CliPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->AddClientToChannel(*CliPtr, false);
			ChanPtr->SetLimitUsers(1);
			CHECK(VerifyChannelLimit(*ChanPtr) == false);
		}
	}

	TEST_CASE("VerifyInvitOnly_function")
	{
		Server	server("Password");
		Client	*CliPtr;
		Channel	*ChanPtr;


		SUBCASE("Not on Invit Only mode")
		{
			Setup(server, 1, 1);
			ChanPtr = server.GetChannel("Chan0");
			CliPtr = server.GetClient("Client0");
			if (ChanPtr == NULL || CliPtr == NULL)
				REQUIRE(true == false);
			CHECK(VerifyInvitOnly(*ChanPtr, *CliPtr) == true);
		}
		SUBCASE("on Invit Only mode, client not on waiting list")
		{
			Setup(server, 1, 1);
			ChanPtr = server.GetChannel("Chan0");
			CliPtr = server.GetClient("Client0");
			if (ChanPtr == NULL || CliPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->SetMode(INVITE_ONLY, true);
			CHECK(VerifyInvitOnly(*ChanPtr, *CliPtr) == false);
		}
		SUBCASE("on Invit Only mode, client on waiting list")
		{
			Setup(server, 1, 1);
			ChanPtr = server.GetChannel("Chan0");
			CliPtr = server.GetClient("Client0");
			if (ChanPtr == NULL || CliPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->SetMode(INVITE_ONLY, true);
			ChanPtr->PutClientOnWaitingList(*CliPtr);
			CHECK(VerifyInvitOnly(*ChanPtr, *CliPtr) == true);
		}
	}

	TEST_CASE("VerifyPasswordNeed_function")
	{
		Server	server("Password");
		Client	*CliPtr;
		Channel	*ChanPtr;


		SUBCASE("No password needed")
		{
			Setup(server, 1, 0);
			ChanPtr = server.GetChannel("Chan0");
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			CHECK(VerifyPasswordNeed(*ChanPtr, "") == true);
		}
		SUBCASE("Password needed, and valid")
		{
			Setup(server, 1, 0);
			ChanPtr = server.GetChannel("Chan0");
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->SetMode(PASSWORD_SET, true);
			ChanPtr->SetPassword("Aled");
			CHECK(VerifyPasswordNeed(*ChanPtr, "Aled") == true);
		}
		SUBCASE("Password needed, and invalid")
		{
			Setup(server, 1, 0);
			ChanPtr = server.GetChannel("Chan0");
			if (ChanPtr == NULL)
				REQUIRE(true == false);
			ChanPtr->SetMode(PASSWORD_SET, true);
			ChanPtr->SetPassword("Aled");
			CHECK(VerifyPasswordNeed(*ChanPtr, "Ale") == false);
		}
	}
}