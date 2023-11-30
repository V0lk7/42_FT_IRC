#include "Kick.hpp"
#include "doctest.hpp"
#include "../../../Mandatory/Commands/KICK/ParsingKick.cpp"

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

// ########################################################################## //
// # TODO___________________________________________________________________# //
// # find error on findChannel                                               # //
// #                                                                        # //
// ########################################################################## //

TEST_SUITE ( "KICK TESTING" )
{
    Server   server;
    Channel* channel = server.GetChannel( "#Test" );
    Client*  kicker = server.GetClient( "sudo" );
    Client*  target = server.GetClient( "Jean" );
    Client*  notTarget = server.GetClient( "Jimmy" );
    std::vector<std::string> data;

// ########################################################################## //
// # FINDTARGET TRUE________________________________________________________# //
    TEST_CASE ( "findTarget: true" )
    {
        data.clear();
        data.push_back("#Test");
        data.push_back("Jean");
        CHECK ( findTarget( data, *channel, *kicker ) == CONTINUE );
    }
// ########################################################################## //

// ########################################################################## //
// # FINDTARGET FALSE_______________________________________________________# //
    TEST_CASE ( "findTarget: false" )
    {
        data.clear();
        data.push_back("#Test");
        SUBCASE( "Jimmy not in list" ) {
            data.push_back("Jimmy");
            CHECK ( findTarget( data, *channel, *notTarget ) == NOTARGET );
        }
        SUBCASE( "empty case" ) {
            data.push_back("");
            CHECK ( findTarget( data, *channel, *kicker ) == NOTARGET );
        }
        SUBCASE ( "himself" ) {
            data.push_back( "sudo" );
            CHECK ( findTarget( data, *channel, *kicker ) == NOTARGET );
        }
    }
// ########################################################################## //

// ########################################################################## //
// # CHECKRIGHT_____________________________________________________________# //
    TEST_CASE ( "checkRight: true" ) {
        CHECK ( checkRight( *channel, *kicker ) == CONTINUE );
    }
    TEST_CASE ( "checkRight: false" ) {
        CHECK ( checkRight( *channel, *target ) == NORIGHT );
    }
// ########################################################################## //
// # PARSECMD_______________________________________________________________# //
    TEST_CASE ( "ParseCmd: true" )
    {
        CHECK ( parseCmd( "KICK #Test Jean", channel, *kicker  ) == NONE );
        CHECK ( parseCmd( "KICK #test Jimmy", NULL, *kicker ) == NOCHANNEL );
        CHECK ( parseCmd( "KICK #Test Jimmy", channel, *kicker ) == NOTARGET );
        CHECK ( parseCmd( "KICK #Test Jean", channel, *target  ) == NORIGHT );

        CHECK ( parseCmd( "KICK Jean #Test ", channel, *kicker  ) != NONE );

        CHECK ( parseCmd( "KICK Test #Jean", channel, *kicker  ) != NONE );

        CHECK ( parseCmd( "KICK TTest #Jean", channel, *kicker  ) != NONE );
    }
// ########################################################################## //
}
