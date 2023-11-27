#include "doctest.hpp"
#include "../../../Mandatory/Parsing/Parsing.cpp"

#include "Server.hpp"
#include "Client.hpp"

TEST_SUITE( "PARSING" ) {

    Server  server2;
    Client* Charles = server2.GetClient( "Charles" );

    TEST_CASE( "HandleCommand" ) {
        // CHECK ( handleCommand( "KICK", server2, *Charles ) == KICK );
        // CHECK ( handleCommand( "INVITE", server2, *Charles ) == INVITE );
        // CHECK ( handleCommand( "PRIVMSG", server2, *Charles ) == PRIVMSG );
        // CHECK ( handleCommand( "TOPIC", server2, *Charles ) == TOPIC );
        // CHECK ( handleCommand( "JOIN", server2, *Charles ) == JOIN );
        // CHECK ( handleCommand( "MODE", server2, *Charles ) == MODE );
        //
        // CHECK ( handleCommand( "PASS", server2, *Charles ) == CLIENT );
        CHECK ( handleCommand( "NICK", server2, *Charles ) == CLIENT );
        // CHECK ( handleCommand( "USER", server2, *Charles ) == CLIENT );
    }
}
