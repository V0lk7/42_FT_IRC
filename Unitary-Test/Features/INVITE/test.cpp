#include "JOIN/Join.hpp"
#include "doctest.hpp"

#include <algorithm>
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "../../../Mandatory/Commands/INVITE/Invite.cpp"

TEST_CASE( "function: FindTargetInServer" ) 
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

TEST_CASE( "function: TargetAlreadyInChannel" )
{
    Server      server;
    Channel*    channel( server.GetChannel( "#Test" ) );
    Client*     client = server.GetClient( "sudo" );
    server.AddClient( new Client ( "Henri", CLIENT ) );
    std::string inServer( "Henri" );
    std::string notInServer( "Jimmy" );
    SUBCASE( "NEXT" ) {
        CHECK( targetAlreadyInChannel( *channel, inServer, *client ) == NEXT );
    }
    SUBCASE( "TARGETALREADYINCHANNEL" ) {
        CHECK( targetAlreadyInChannel( *channel, "Jean", *client ) == TARGETALREADYINCHANNEL );
    }
    SUBCASE( "TARGETINWAITLIST" ) {
        CHECK( targetAlreadyInChannel( *channel, "Jimmy", *client ) == TARGETINWAITLIST );
    }
}

TEST_CASE( "function: hostRight" )
{
    Server      server;
    Channel*    channel( server.GetChannel( "#Test" ) );
    Client*     validRight = server.GetClient( "sudo" );
    Client*     noRight = server.GetClient( "Charles" );
    SUBCASE( "valid right" ) {
        CHECK( hostRight(*channel, *validRight) == NEXT );
    }
    SUBCASE( "unvalid Right" ) {
            CHECK( hostRight(*channel, *noRight) == BADRIGHT );
    }
}

TEST_CASE( "function: isValidRight" )
{
    Server      server;
    server.AddClient( new Client ( "Henri", CLIENT ) );
    Channel*    channel( server.GetChannel( "#Test" ) );
    Client*     sudo = server.GetClient( "sudo" );
    Client*     Charles = server.GetClient( "Charles" );
    std::string henri( "Henri" );
    std::string itself( "sudo" );
    std::string charles( "Charles" );

    SUBCASE( "NEXT" ) {
        CHECK( isValidRight( *sudo, *channel, henri ) == NEXT );
    }
    SUBCASE( "CLIENTISTARGET" ) {
        CHECK( isValidRight( *sudo, *channel, itself ) == CLIENTISTARGET );
    }
    SUBCASE( "TARGETALREADYINCHANNEL" ) {
        CHECK( isValidRight( *sudo, *channel, charles ) == TARGETALREADYINCHANNEL );
    }
    SUBCASE( "check right. INVITE_ONLY_SET true" ) {
        channel->SetMode( INVITE_ONLY_SET, true );
        CHECK( isValidRight( *Charles, *channel, henri ) == BADRIGHT );
    }
    SUBCASE( "check right. INVITE_ONLY_SET false" ) {
        CHECK( isValidRight( *Charles, *channel, henri ) == NEXT );
    }
}

TEST_CASE( "function: invitePArsing" )
{
    Server      server;
    server.AddClient( new Client ( "Henri", CLIENT ) );
    Channel*    channel( server.GetChannel( "#Test" ) );
    Client*     sudo = server.GetClient( "sudo" );
    Client*     Charles = server.GetClient( "Charles" );
    std::string henri( "Henri" );
    std::string itself( "sudo" );
    std::string charles( "Charles" );
    std::vector<std::string> cmd;
    SUBCASE( "true" )
    {
        cmd.push_back("Henri");
        cmd.push_back("#Test");
        CHECK( inviteParsing(cmd, server, *sudo, *channel ) == true );
        CHECK( inviteParsing(cmd, server, *Charles, *channel ) == true );
    }
    SUBCASE( "targetNotInServer" )
    {
        cmd.push_back("Caramel");
        cmd.push_back("#Test");
        CHECK( inviteParsing(cmd, server, *sudo, *channel ) == false );
    }
    SUBCASE( "Right part" )
    {
        cmd.push_back("Charles");
        cmd.push_back("#Test");
        CHECK( inviteParsing(cmd, server, *sudo, *channel ) == false );
    }
    SUBCASE( "Right part false Chrales already in Channel" )
    {
        cmd.push_back("Charles");
        cmd.push_back("#Test");
        channel->SetMode( INVITE_ONLY_SET , true );
        CHECK( inviteParsing(cmd, server, *Charles, *channel ) == false );
        CHECK( inviteParsing(cmd, server, *sudo, *channel ) == false );
    }
    SUBCASE( "Right part Henri" )
    {
        cmd.push_back("Henri");
        cmd.push_back("#Test");
        channel->SetMode( INVITE_ONLY_SET , true );
        CHECK( inviteParsing(cmd, server, *Charles, *channel ) == false );
        CHECK( inviteParsing(cmd, server, *sudo, *channel ) == true );
    }
}

TEST_CASE( "extractTarget" )
{
    Server      server;
    Client*     Henri = new Client ( "Henri", CLIENT );
    server.AddClient( Henri );
    Channel*    channel( server.GetChannel( "#Test" ) );
    Client*     sudo = server.GetClient( "sudo" );
    Client*     Charles = server.GetClient( "Charles" );
    std::string henri( "Henri" );
    std::string itself( "sudo" );
    std::string charles( "Charles" );
    SUBCASE( "OK" ) {
        if ( &extractTarget( henri, server ) != &*Henri  )
            CHECK( true == false );
    }
}

TEST_CASE( "invite" )
{
    Server      server;
    Client*     Henri = new Client ( "Henri", CLIENT );
    server.AddClient( Henri );
    Channel*    channel( server.GetChannel( "#Test" ) );
    Client*     sudo = server.GetClient( "sudo" );
    Client*     Charles = server.GetClient( "Charles" );
    std::string henri( "Henri" );
    std::string itself( "sudo" );
    std::string charles( "Charles" );
    SUBCASE( "true" )
    {
        invite( server, *sudo, "INVITE Henri #Test" );
        std::list<Client*> WCL(channel->GetWaitingList());
        CHECK( std::find( WCL.begin(), WCL.end(), Henri ) != WCL.end() );
    }
    SUBCASE( "true" )
    {
        invite( server, *sudo, "INVITE BOB #Test" );
        std::list<Client*> WCL(channel->GetWaitingList());
        CHECK( std::find( WCL.begin(), WCL.end(), Henri ) == WCL.end() );
    }
}

TEST_CASE( "inviteReaply" )
{
    Server      server;
    Client*     Henri = new Client ( "Henri", CLIENT );
    server.AddClient( Henri );
    Channel*    channel( server.GetChannel( "#Test" ) );
    Client*     sudo = server.GetClient( "sudo" );
    Client*     Charles = server.GetClient( "Charles" );
    std::string henri( "Henri" );
    std::string itself( "sudo" );
    std::string charles( "Charles" );
    SUBCASE( "targetGood" ) {
        invite( server, *sudo, "INVITE Henri #Test" );
        CHECK( Henri->GetMessage() == ": 301 sudo #Test:INVITE you've been invited in channel.\r\n");
        CHECK( sudo->GetMessage() == "" );
    }
    SUBCASE( "NOTARGETINSERVER" ) {
        invite( server, *sudo, "INVITE Damien #Test" );
        CHECK( sudo->GetMessage() ==
              ": 401 sudo #Test:INVITE cannot access to the target mentioned in server.\r\n" );
    }
}
