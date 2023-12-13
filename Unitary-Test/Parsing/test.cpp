#include "doctest.hpp"
#include "../../../Mandatory/Parsing/Parsing.cpp"

#include "Server.hpp"
#include "Client.hpp"

TEST_SUITE( "PARSING" ) {

//    Server  server2;
//    Client* Charles = server2.GetClient( "Charles" );
//
//    TEST_CASE( "HandleCommand" ) {
//        // CHECK ( handleCommand( "KICK", server2, *Charles ) == KICK );
//        // CHECK ( handleCommand( "INVITE", server2, *Charles ) == INVITE );
//        // CHECK ( handleCommand( "PRIVMSG", server2, *Charles ) == PRIVMSG );
//        // CHECK ( handleCommand( "TOPIC", server2, *Charles ) == TOPIC );
//        // CHECK ( handleCommand( "JOIN", server2, *Charles ) == JOIN );
//        // CHECK ( handleCommand( "MODE", server2, *Charles ) == MODE );
//        //
//        // CHECK ( handleCommand( "PASS", server2, *Charles ) == CLIENT );
//        //CHECK ( handleCommand( "NICK", server2, *Charles ) == CLIENT );
//        // CHECK ( handleCommand( "USER", server2, *Charles ) == CLIENT );
//    }
	TEST_CASE("wayChooser function"){
		std::string	target;

		SUBCASE("TOPIC_OK_1"){
			target = "TOPIC #mdr";
			CHECK(wayChooser(target) == TOPIC);
		}
		SUBCASE("TOPIC_OK_2"){
			target = "TOPIC      #mdr";
			CHECK(wayChooser(target) == TOPIC);
		}
		SUBCASE("TOPIC_OK_3"){
			target = "TOPIC";
			CHECK(wayChooser(target) == TOPIC);
		}
		SUBCASE("TOPIC_KO_1"){
			target = "TOPICC #mdr";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("TOPIC_KO_2"){
			target = " TOPIC #mdr";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("TOPIC_KO_3"){
			target = "TTOPIC #mdr";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("TOPIC_KO_4"){
			target = "TOPI #mdr";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("INVITE_OK_1"){
			target = "INVITE #mdr";
			CHECK(wayChooser(target) == INVITE);
		}
		SUBCASE("INVITE_OK_2"){
			target = "INVITE";
			CHECK(wayChooser(target) == INVITE);
		}
		SUBCASE("INVITE_KO_1"){
			target = "INVITEE";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("INVITE_KO_2"){
			target = "IINVITE";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("INVITE_KO_3"){
			target = " INVITE";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("USER_OK_1"){
			target = "USER jduval";
			CHECK(wayChooser(target) == sUSER);
		}
		SUBCASE("USER_OK_2"){
			target = "USER";
			CHECK(wayChooser(target) == sUSER);
		}
		SUBCASE("USER_KO_1"){
			target = "  USER";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("USER_KO_2"){
			target = "UUSER";
			CHECK(wayChooser(target) == -1);
		}
		SUBCASE("USER_KO_3"){
			target = "USERR";
			CHECK(wayChooser(target) == -1);
		}
	}
}
