#include "doctest.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Authentication.hpp"


TEST_CASE("Nick function"){
	Server	serv;
	Client	*ptr = new Client;

	serv.AddClient(ptr);
	SUBCASE("password not set"){
		std::string	cmd("NICK");
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 NICK :Password needed before\r\n") == 0);
	}
	SUBCASE("Nick already set"){
		std::string	cmd("NICK aled");
		ptr->SetPasswd();
		ptr->SetNickname("hey");
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 NICK :Nick already set\r\n") == 0);
	}

	SUBCASE("Not enough parameter"){
		std::string	cmd("NICK");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 431 NICK :Need more parameters\r\n") == 0);
	}
	SUBCASE("Bad nick_1"){
		std::string	cmd("NICK aled   ");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 432 NICK :Wrong format nickname\r\n") == 0);
	}
	SUBCASE("Bad nick_2"){
		std::string	cmd("NICK ale#d");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 432 NICK :Wrong format nickname\r\n") == 0);
	}
	SUBCASE("Bad nick_3"){
		std::string	cmd("NICK &aled");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 432 NICK :Wrong format nickname\r\n") == 0);
	}
	SUBCASE("Bad nick_4"){
		std::string	cmd("NICK a:led");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 432 NICK :Wrong format nickname\r\n") == 0);
	}
	SUBCASE("Already in use nick_1"){
		std::string	cmd("NICK sudo");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 433 NICK :Nickname already in use\r\n") == 0);
	}
	SUBCASE("Already in use nick_2"){
		std::string	cmd("NICK Jean");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 433 NICK :Nickname already in use\r\n") == 0);
	}
	SUBCASE("Already in use nick_3"){
		std::string	cmd("NICK      Charles");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 433 NICK :Nickname already in use\r\n") == 0);
	}
	SUBCASE("Good nick_1"){
		std::string	cmd("NICK aled");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 NICK :Nickname set\r\n") == 0);
		CHECK(ptr->GetNickname() == "aled");
	}
	SUBCASE("Good nick_2"){
		std::string	cmd("NICK    jduval");
		ptr->SetPasswd();
		Nick(serv, *ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 NICK :Nickname set\r\n") == 0);
		CHECK(ptr->GetNickname() == "jduval");
		CHECK(ptr->GetStatementStep(NICK) == true);
	}
}
