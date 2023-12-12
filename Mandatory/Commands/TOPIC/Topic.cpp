#include "Tools.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include <iostream>

// ########################################################################## //
// #_TODO___________________________________________________________________# //
// #-> two functions:-> change TOPIC if arguments were send                 # //
// #                 -> display TOPIC if no argument was send               # //
// #-> make send msg when errro input user was detected                     # //
// #-> question about management need i inform all user when topic change   # //
// #-> TOPIC DONE NEED MSG MANAGEMENT                                       # //
// ########################################################################## //

typedef enum TErr {
    TOPICNONE, NOTOPIC,
    TOPICNORIGHT, TOPICNOCHANNEL,
    TOPICERR, TOPICCLIENTNOINCHANNEL,
    TOPICSEND, TOPICCHANGED
} TErr;

static enum TErr
topicParsing( Client& client, Channel* channel );
static enum TErr
topicChange( std::vector<std::string>& data, Channel* channel, Client& client );
static void
topicReaply( Client& client, Channel* channel, int flag );

void
Topic( const Server& server, Client& client, const std::string& cmd )
{
    Channel* channel;

    std::vector<std::string> data = split( cmd, " " );
    data.erase( data.begin() );

    if ( !data.size() ) {
        topicReaply( client, NULL, TOPICNOCHANNEL );
        return ;
    }

    channel = server.GetChannel( data[0] );

    if ( topicParsing( client, channel) != TOPICNONE )
        return ;

    if ( data.size() == 1 ) {
        topicReaply( client, channel, TOPICSEND );
        return ;
    }

    topicChange( data, channel, client );
}

static enum TErr
topicChange( std::vector<std::string>& data, Channel* channel, Client& client )
{
    std::string topic;
    std::string output;
    std::vector<std::string>::iterator check = data.begin();
    check++;

    if ( data.size() == 1 && (*check)[0] == ':' && (*check).length() == 1 ) {
        topicReaply( client, channel, TOPICERR );
        return ( TOPICERR );
    }

    if ( (*check)[0] != ':' ) {
        topicReaply( client, channel, TOPICERR );
        return ( TOPICERR );
    }

    topic = *check;
    check++;
    while ( check != data.end() )
    {
        topic += " " + *check;
        check++;
    }
    topic = topic.substr( 2, std::string::npos );
    channel->SetTopic( topic );
    topicReaply( client, channel, TOPICCHANGED );
    return ( TOPICNONE );
}

static enum TErr
topicParsing( Client& client, Channel* channel )
{
    std::map<Client*, bool>             key; 
    bool                                right = false;

    std::cout << client.GetNickname() << std::endl;
    if ( !channel ) {
        topicReaply( client, channel, TOPICNOCHANNEL );
        return ( TOPICNOCHANNEL );
    }

    key = channel->GetUsers();
    if ( !channel->GetMode( TOPIC_CHANGE_SET ) ) {
        right = true;
    }
    else if ( !key.count( &client ) ) {
        topicReaply( client, channel, TOPICCLIENTNOINCHANNEL );
        return ( TOPICCLIENTNOINCHANNEL );
    }

    else if ( key[ &client ] ) {
        right = true;
    }

    else {
        topicReaply( client, channel, TOPICNORIGHT );
        return ( TOPICNORIGHT );
    }

    return ( right ? TOPICNONE : TOPICNORIGHT );
}

// ########################################################################## //
// #_topicReaply____________________________________________________________# //
static void
topicReaply( Client& client, Channel* channel, int flag )
{
    std::string reply;
    std::string clientName( client.GetNickname() );
    std::string channelName;
    if ( channel )
        std::string channelName = (*channel).GetName() ;

    if ( flag == TOPICCLIENTNOINCHANNEL ) {
        reply = ": 442 " + clientName + " " + channelName
              + ":TOPIC You are not currently in the channel."
              + "\r\n";
    }

    else if ( flag == TOPICNORIGHT ) {
        reply = ": 477 " + clientName + " " + channelName
              + ":TOPIC this channel is in MODE invite only and"
              + " you are not a valid operator"
              + "\r\n";
    }

    else if ( flag == TOPICNOCHANNEL || flag == TOPICERR ) {
        reply = ": 442 " + clientName +
              + ":TOPIC command is invalid or improperly formatted."
              + "\r\n";
    }

    else if ( flag == TOPICSEND ) {
        reply = ":" + clientName + " 331 dan " + channelName;
        if ( !channel->GetTopic().empty() )
              reply += " " + channel->GetTopic() + "\r\n";
        else
              reply += ":NO topic is set.\r\n";
    }

    else if ( flag == TOPICCHANGED ) {
        reply = ": 332 " + clientName + " " + channelName
              + ":TOPIC channel topic changed"
              + "\r\n";
    }

    else if ( TOPICNONE )
        reply = "" ;

    client.SetMessageToSend( reply );
}
