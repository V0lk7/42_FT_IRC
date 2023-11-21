#include "doctest.hpp"

#include "../../../Mandatory/Commands/JOIN/Join.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Utils.hpp"

TEST_SUITE("Test Reply message")
{
	TEST_CASE("CreateReply_function")
	{
		Server			server("Password");
		Client			*CliPtr;
		Channel			*ChanPtr;
		std::string		reply;	

		Setup(server, 2, 3);

		SUBCASE("NEW_CHANNEL")
		{
			reply = ":Client0 JOIN #Chan0\r\n";
			reply += ": 353 Client0 = #Chan0 :@Client0\r\n";
			reply += ": 366 Client0 #Chan0 :End of /NAMES list.\r\n";
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			CreateReply(*CliPtr, *ChanPtr, NEW_CHANNEL);
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("EXISTING_CHANNEL")
		{
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			ChanPtr->AddClientToChannel(*CliPtr, true);
			CliPtr = server.GetClient("Client1");
			ChanPtr->AddClientToChannel(*CliPtr, false);
			ChanPtr->SetTopic("Hello");

			reply = ":Client1 JOIN #Chan0\r\n";
			reply += ": 332 Client1 #Chan0 :Hello\r\n";
			reply += ": 353 Client1 = #Chan0 :Client1 @Client0\r\n";
			reply += ": 366 Client1 #Chan0 :End of /NAMES list.\r\n";
			CreateReply(*CliPtr, *ChanPtr, EXISTING_CHANNEL);
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("BAD_KEY")
		{
			reply = ": 475 Client0 #Chan0 :Cannot join channel (+k) - bad key\r\n";
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			CreateReply(*CliPtr, *ChanPtr, BAD_KEY);
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("TOO_MANY_CLIENT")
		{
			reply = ": 471 Client0 #Chan0 :Cannot join channel (+l) - channel full\r\n";
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			CreateReply(*CliPtr, *ChanPtr, TOO_MANY_CLIENT);
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("NOT_INVITED")
		{
			reply = ": 473 Client0 #Chan0 :Cannot join channel (+i) - not invited\r\n";
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			CreateReply(*CliPtr, *ChanPtr, NOT_INVITED);
			CHECK(CliPtr->GetMessage() == reply);
		}
		SUBCASE("ALREADY_IN")
		{
			reply = ": 338 Client0 #Chan0 :Cannot join channel, you're already in\r\n";
			CliPtr = server.GetClient("Client0");
			ChanPtr = server.GetChannel("Chan0");
			if (CliPtr == NULL || ChanPtr == NULL)
				REQUIRE(true == false);
			CreateReply(*CliPtr, *ChanPtr, ALREADY_IN);
			CHECK(CliPtr->GetMessage() == reply);
		}
	}
}
