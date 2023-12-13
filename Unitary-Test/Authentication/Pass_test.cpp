#include "doctest.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Authentication.hpp"

TEST_CASE("PASS function"){
	Server	serv;
	Client	*ptr = new Client;

	serv.AddClient(ptr);

	SUBCASE("No password"){
		std::string	cmd("PASS");
		Pass(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 PASS :Need more parameters\r\n") == 0);
	}
	SUBCASE("Wrong password"){
		std::string	cmd("PASS aled");
		Pass(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 PASS :Invalid password\r\n") == 0);
	}
	SUBCASE("good password"){
		std::string	cmd("PASS password");
		Pass(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 PASS :Password accepted\r\n") == 0);
		CHECK(ptr->GetStatementStep(PASSWD) == true);
	}
	SUBCASE("Already registered"){
		std::string	cmd("PASS password");
		ptr->SetPasswd();
		ptr->SetNickname("jduval");
		ptr->SetUsername("jduval");
		Pass(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 462 PASS :Password already accepted\r\n") == 0);
	}
	SUBCASE("Password not needed"){
		std::string	cmd("PASS password");
		ptr->SetPasswd();
		serv.SetPassword("");
		ptr->SetNickname("jduval");
		ptr->SetUsername("jduval");
		Pass(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 PASS :Password not needed\r\n") == 0);
	}
}
