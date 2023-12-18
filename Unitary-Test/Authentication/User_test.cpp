#include "doctest.hpp"
#include "Client.hpp"
#include "Authentication.hpp"

TEST_CASE("User function"){

	Client	*ptr = new Client;

	SUBCASE("No password set"){
		std::string	cmd("USER aled");
		User(*ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 USER :Password needed before\r\n") == 0);
		delete ptr;	
	}
	SUBCASE("No nick set"){
		std::string	cmd("USER aled");
		ptr->SetPasswd();
		User(*ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 USER :Nick needed before\r\n") == 0);
		delete ptr;	
	}
	SUBCASE("User already set"){
		std::string	cmd("USER");
		ptr->SetPasswd();
		ptr->SetNickname("jduval");
		ptr->SetUsername("jduval");
		User(*ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 USER :User already set\r\n") == 0);
		delete ptr;	
	}
	SUBCASE("no parameter"){
		std::string	cmd("USER");
		ptr->SetPasswd();
		ptr->SetNickname("jduval");
		User(*ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 431 USER :Need more parameters\r\n") == 0);
		delete ptr;	
	}
	SUBCASE("no parameter 2"){
		std::string	cmd("USER      ");
		ptr->SetPasswd();
		ptr->SetNickname("jduval");
		User(*ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 431 USER :Need more parameters\r\n") == 0);
		delete ptr;	
	}
	SUBCASE("good parameter"){
		std::string	cmd("USER jduval");
		ptr->SetPasswd();
		ptr->SetNickname("jduval");
		User(*ptr, cmd);
		CHECK(ptr->GetMessage().compare(": 461 USER :Username set\r\n") == 0);
		CHECK(ptr->GetStatementStep(USER) == true);
		CHECK(ptr->GetStatementStep(OK) == true);
		delete ptr;	
	}
}
