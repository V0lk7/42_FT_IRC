#include "doctest.hpp"

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include "../../../Mandatory/Commands/TOPIC/Topic.cpp"

TEST_CASE ( "topicParsing" ) 
{
    Server                      server;
    Channel*                    channel = server.GetChannel( "Test" );
    Client*                     sudo = server.GetClient( "sudo" );
    Client*                     Jean = server.GetClient( "Jean" );
    std::vector<std::string>    data;
    SUBCASE ( "true" )
    {
        data.push_back("#Test");
        CHECK(topicParsing( data, *sudo, *channel ) == TOPICNONE );
    }
    SUBCASE ( "no channel" )
    {
        data.push_back("#test");
        CHECK(topicParsing( data, *sudo, *channel ) != TOPICNONE );
    }
    SUBCASE ( "Channel right but no right user" )
    {
        data.push_back("#Test");
        CHECK(topicParsing( data, *Jean, *channel ) == TOPICNONE );
    }
    SUBCASE ( "TOPIC_CHANGE_SET false but no right user" )
    {
        data.push_back("#Test");
        CHECK(topicParsing( data, *Jean, *channel ) == TOPICNONE );
    }
    SUBCASE ( "TOPIC_CHANGE_SET true but no right user" )
    {
        data.push_back("#Test");
        channel->SetMode( TOPIC_CHANGE_SET, true );
        CHECK(topicParsing( data, *Jean, *channel ) != TOPICNONE );
    }
    SUBCASE ( "Channel TOPIC_CHANGE_SET true but right user ok" )
    {
        data.push_back("#Test");
        channel->SetMode( TOPIC_CHANGE_SET, true );
        CHECK(topicParsing( data, *sudo, *channel ) == TOPICNONE );
    }
}
