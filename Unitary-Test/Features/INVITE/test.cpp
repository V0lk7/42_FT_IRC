#include "doctest.hpp"

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "../../../Mandatory/Commands/INVITE/Invite.cpp"

TEST_CASE( "function: FINDTARGET" ) 
{
    Server server;
    std::string inServer( "Jimmy" );
    std::string notInServer( "Henri" );
    SUBCASE( "true" ) {
        CHECK( findTargetInServer( inServer, server) == NEXT );
    }
    SUBCASE( "false" ) {
        CHECK( findTargetInServer( notInServer, server) == NOTARGETINSERVER );
    }
}

TEST_CASE( "function: TARGETALREADYINCHANNEL" )
{
    Server      server;
    Channel*    channel( server.GetChannel( "#Test" ) );
    server.AddClient( new Client ( "Henri", CLIENT ) );
    std::string inServer( "Henri" );
    std::string notInServer( "Jimmy" );
    SUBCASE( "NEXT" ) {
        CHECK( targetAlreadyInChannel( *channel, inServer ) == NEXT );
    }
    SUBCASE( "TARGETALREADYINCHANNEL" ) {
        CHECK( targetAlreadyInChannel( *channel, "Jean" ) == TARGETALREADYINCHANNEL );
    }
    SUBCASE( "TARGETINWAITLIST" ) {
        CHECK( targetAlreadyInChannel( *channel, "Jimmy" ) == TARGETINWAITLIST );
    }
}
