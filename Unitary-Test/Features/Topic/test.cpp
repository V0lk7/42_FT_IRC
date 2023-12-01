#include "doctest.hpp"

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include "../../../Mandatory/Commands/TOPIC/Topic.cpp"

TEST_CASE ( "topicParsing" ) 
{
    Server                      server;
    Channel*                    channel = server.GetChannel( "#Test" );
    Client*                     sudo = server.GetClient( "sudo" );
    Client*                     Jean = server.GetClient( "Jean" );
    std::vector<std::string>    data;
    SUBCASE ( "true" )
    {
        data.push_back("#Test");
        CHECK(topicParsing( *sudo, channel ) == TOPICNONE );
    }
    SUBCASE ( "no channel" )
    {
        data.push_back("#test");
        CHECK(topicParsing( *sudo, NULL ) != TOPICNONE );
    }
    SUBCASE ( "Channel right but no right user" )
    {
        data.push_back("#Test");
        CHECK(topicParsing( *Jean, channel ) == TOPICNONE );
    }
    SUBCASE ( "TOPIC_CHANGE_SET false but no right user" )
    {
        data.push_back("#Test");
        CHECK(topicParsing( *Jean, channel ) == TOPICNONE );
    }
    SUBCASE ( "TOPIC_CHANGE_SET true but no right user" )
    {
        data.push_back("#Test");
        channel->SetMode( TOPIC_CHANGE_SET, true );
        CHECK(topicParsing( *Jean, channel ) != TOPICNONE );
    }
    SUBCASE ( "Channel TOPIC_CHANGE_SET true but right user ok" )
    {
        data.push_back("#Test");
        channel->SetMode( TOPIC_CHANGE_SET, true );
        CHECK( topicParsing( *sudo, channel ) == TOPICNONE );
    }
}

TEST_CASE ( "topicParsing" )
{
    Server                      server;
    Channel*                    channel = server.GetChannel( "#Test" );
    Client*                     sudo = server.GetClient( "sudo" );
    Client*                     Jean = server.GetClient( "Jean" );
    std::vector<std::string>    data;
    SUBCASE ( "true" )
    {
        data.push_back( ":the" );
        data.push_back( "new" );
        data.push_back( "Topic" );
        topicChange( data, channel, *sudo );
        CHECK( channel->GetTopic() == "the new Topic" );
    }
    SUBCASE ( "false" )
    {
        data.push_back( ":" );
        CHECK( topicChange( data, channel, *sudo) == TOPICERR );
    }
    SUBCASE ( "false" )
    {
        data.push_back( "the" );
        data.push_back( "new" );
        data.push_back( "Topic" );
        CHECK( topicChange( data, channel, *sudo) == TOPICERR );
    }
}
