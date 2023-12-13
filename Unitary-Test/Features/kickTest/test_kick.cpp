#include "doctest.hpp"
#include "../../../Mandatory/Commands/KICK/Kick.cpp"

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

TEST_SUITE ( "KICK HANDLING" )
{
    Server                      server3;
    Channel*                    channel3 = server3.GetChannel( "#Test" );
    Client*                     kicker3 = server3.GetClient( "sudo" );
    Client*                     target3 = server3.GetClient( "Jean" );
    std::vector<std::string>    msg;

// ########################################################################## //
// #_msgMaker_______________________________________________________________# //
    TEST_CASE ( "msgMaker: no reason" )
    {
        msg.push_back( "#Test" );
        msg.push_back( "Jean" );
        CHECK ( msgMaker( *kicker3, *channel3, msg ) ==
                                                 ":sudo KICK #Test Jean\r\n");
    }

    TEST_CASE ( "msgMaker: reason" )
    {
        msg.push_back( "#Test" );
        msg.push_back( "Jean" );
        msg.push_back( "because" );
        msg.push_back( "racism" );
        CHECK ( msgMaker( *kicker3, *channel3, msg ) ==
                                  ":sudo KICK #Test Jean : because racism\r\n");
    }
// ########################################################################## //

// ########################################################################## //
// #_rmClientOfChannel______________________________________________________# //
    TEST_CASE ( "rmClientOfChannel" )
    {
        rmClientOfChannel( *channel3, "Jean", "ok" );
        CHECK ( channel3->GetUsers().count( &*target3 ) == 0 );
    }
}
// ########################################################################## //
