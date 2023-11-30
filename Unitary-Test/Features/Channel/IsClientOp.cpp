#include "doctest.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

TEST_CASE("IsClientOp_method")
{
	Server	serv;
	Client	*Cliptr;
	Channel	*Chanptr = serv.GetChannel("Test");

	SUBCASE("Client not on channel"){	
		Client	client;
		CHECK(Chanptr->IsClientOperator(client) == false);
	}
	SUBCASE("Client not OP"){
		Cliptr = serv.GetClient("Jimmy");
		CHECK(Chanptr->IsClientOperator(*Cliptr) == false);
	}
	SUBCASE("Client is OP"){
		Cliptr = serv.GetClient("sudo");
		CHECK(Chanptr->IsClientOperator(*Cliptr) == true);
	}
}
