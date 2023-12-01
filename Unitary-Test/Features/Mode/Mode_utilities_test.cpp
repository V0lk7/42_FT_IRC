#include "doctest.hpp"
#include "../../../Mandatory/Commands/MODE/Mode.hpp"
#include "Error_code.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

static CmdNode	CreateNode(bool a, char Mode, std::string Param);

TEST_SUITE("Utilities_test")
{
	TEST_CASE("ChangeInvitOnlyMode_function"){
		
		Server		serv;
		Channel		*ChanPtr = serv.GetChannel("#Test");
		Client		*CliPtr;
		CmdNode		Node;

		SUBCASE("Test_1 : Same state"){
			Node = CreateNode(false, 'i', "");
			CHECK(ChangeInvitOnlyMode(ChanPtr, Node) == SILENT);
			CHECK(ChanPtr->GetMode(INVITE_ONLY_SET) == false);
		}
		SUBCASE("Test_2 : Change the state"){
			Node = CreateNode(true, 'i', "");
			CHECK(ChangeInvitOnlyMode(ChanPtr, Node) == INVITONLY_CHANGED);
			CHECK(ChanPtr->GetMode(INVITE_ONLY_SET) == true);
		}
		SUBCASE("Test_3 : Change the state: return SILENT"){
			Node = CreateNode(false, 'i', "");
			ChanPtr->SetMode(INVITE_ONLY_SET, true);
			CHECK(ChangeInvitOnlyMode(ChanPtr, Node) == INVITONLY_CHANGED);
			CHECK(ChanPtr->GetMode(INVITE_ONLY_SET) == false);
		}
	}
	TEST_CASE("ChangeTopicMode_function"){
		
		Server		serv;
		Channel		*ChanPtr = serv.GetChannel("#Test");
		Client		*CliPtr;
		CmdNode		Node;

		SUBCASE("Test_1 : Same state"){
			Node = CreateNode(false, 't', "");
			CHECK(ChangeTopicMode(ChanPtr, Node) == SILENT);
			CHECK(ChanPtr->GetMode(TOPIC_CHANGE_SET) == false);
		}
		SUBCASE("Test_2 : Change the state"){
			Node = CreateNode(true, 'i', "");
			CHECK(ChangeTopicMode(ChanPtr, Node) == TOPIC_CHANGED);
			CHECK(ChanPtr->GetMode(TOPIC_CHANGE_SET) == true);
		}
		SUBCASE("Test_3 : Change the state"){
			Node = CreateNode(false, 'i', "");
			ChanPtr->SetMode(TOPIC_CHANGE_SET, true);
			CHECK(ChangeTopicMode(ChanPtr, Node) == TOPIC_CHANGED);
			CHECK(ChanPtr->GetMode(TOPIC_CHANGE_SET) == false);
		}
	}
	TEST_CASE("ChangePasswordMode_function"){
		
		Server		serv;
		Channel		*ChanPtr = serv.GetChannel("#Test");
		Client		*CliPtr;
		CmdNode		Node;

		SUBCASE("Test_1 : Same state"){
			Node = CreateNode(false, 'k', "");
			CHECK(ChangePasswordMode(ChanPtr, Node) == ERR_KEYSET);
			CHECK(ChanPtr->GetMode(PASSWORD_SET) == false);
		}
		SUBCASE("Test_2 : No param"){
			Node = CreateNode(true, 'k', "");
			CHECK(ChangePasswordMode(ChanPtr, Node) == ERR_NEEDMOREPARAMS);
			CHECK(ChanPtr->GetMode(PASSWORD_SET) == false);
		}
		SUBCASE("Test_3 : Change the state"){
			Node = CreateNode(true, 'k', "aled");
			ChanPtr->SetMode(PASSWORD_SET, false);
			CHECK(ChangePasswordMode(ChanPtr, Node) == PASSWORD_CHANGED);
			CHECK(ChanPtr->GetMode(PASSWORD_SET) == true);
		}
		SUBCASE("Test_4 : Same state"){
			Node = CreateNode(true, 'k', "aled");
			ChanPtr->SetMode(PASSWORD_SET, true);
			CHECK(ChangePasswordMode(ChanPtr, Node) == ERR_KEYSET);
			CHECK(ChanPtr->GetMode(PASSWORD_SET) == true);
		}
	}
	TEST_CASE("ChangeOperatorPrivilege_function"){
		
		Server		serv;
		Channel		*ChanPtr = serv.GetChannel("#Test");
		Client		*CliPtr;
		CmdNode		Node;

		SUBCASE("TEST_1 : No param"){
			Node = CreateNode(true, 'o', "");
			CHECK(ChangeOperatorPrivilege(serv, ChanPtr, Node) == ERR_NEEDMOREPARAMS);
		}
		SUBCASE("TEST_2 : Wrong client"){
			Node = CreateNode(true, 'o', "WrongClient");
			CHECK(ChangeOperatorPrivilege(serv, ChanPtr, Node) == ERR_NOSUCHNICK);
		}
		SUBCASE("TEST_3 : same state"){
			Node = CreateNode(true, 'o', "sudo");
			CHECK(ChangeOperatorPrivilege(serv, ChanPtr, Node) == SILENT);
		}
		SUBCASE("TEST_4 : change into Op"){
			Node = CreateNode(true, 'o', "Jean");
			CHECK(ChangeOperatorPrivilege(serv, ChanPtr, Node) == CLIENTRIGHT_CHANGED);
			CHECK(ChanPtr->IsClientOperator(*(serv.GetClient("Jean"))) == true);
		}
		SUBCASE("TEST_5 : change into normal"){
			Node = CreateNode(false, 'o', "sudo");
			CHECK(ChangeOperatorPrivilege(serv, ChanPtr, Node) == CLIENTRIGHT_CHANGED);
			CHECK(ChanPtr->IsClientOperator(*(serv.GetClient("sudo"))) == false);
		}
		SUBCASE("TEST_6 : same state 2"){
			Node = CreateNode(false, 'o', "Jean");
			CHECK(ChangeOperatorPrivilege(serv, ChanPtr, Node) == SILENT);
		}
	}
	TEST_CASE("ChangeLimitMode_function"){
		
		Server		serv;
		Channel		*ChanPtr = serv.GetChannel("#Test");
		Client		*CliPtr;
		CmdNode		Node;
		//limit set a 5
		SUBCASE("Test_1 : need more params"){
			Node = CreateNode(true, 'l', "");
			CHECK(ChangeLimitMode(ChanPtr, Node) == ERR_NEEDMOREPARAMS);
		}
		SUBCASE("Test_2 : same state, wrong param"){
			Node = CreateNode(true, 'l', "5efsef");
			CHECK(ChangeLimitMode(ChanPtr, Node) == ERR_NEEDMOREPARAMS);
		}
		SUBCASE("Test_3 : same state, same limit"){
			Node = CreateNode(true, 'l', "5");
			CHECK(ChangeLimitMode(ChanPtr, Node) == LIMIT_CHANGED);
			CHECK(ChanPtr->GetLimitUsers() == 5);
		}
		SUBCASE("Test_4 : same state, change limit"){
			Node = CreateNode(true, 'l', "2");
			CHECK(ChangeLimitMode(ChanPtr, Node) == LIMIT_CHANGED);
			CHECK(ChanPtr->GetLimitUsers() == 2);
		}
		SUBCASE("Test_5 : same state, overflow limit"){
			Node = CreateNode(true, 'l', "214483648");
			CHECK(ChangeLimitMode(ChanPtr, Node) == OUT_OF_RANGE);
		}
		SUBCASE("Test_6 : same state, underfow limit"){
			Node = CreateNode(true, 'l', "0");
			CHECK(ChangeLimitMode(ChanPtr, Node) == OUT_OF_RANGE);
		}
		SUBCASE("Test_7 : same state, negative"){
			Node = CreateNode(true, 'l', "-1");
			CHECK(ChangeLimitMode(ChanPtr, Node) == ERR_NEEDMOREPARAMS);
		}
		SUBCASE("Test_8 : false state, change it"){
			Node = CreateNode(true, 'l', "10");
			ChanPtr->SetLimitUsers(0);
			CHECK(ChangeLimitMode(ChanPtr, Node) == LIMIT_CHANGED);
			CHECK(ChanPtr->GetLimitUsers() == 10);
		}
	}
}

static CmdNode	CreateNode(bool a, char Mode, std::string Param)
{
	CmdNode	node;

	node.Op = a;
	node.Mode = Mode;
	node.Param = Param;
	return (node);
}
