#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "Kick.hpp"
#include "Tools.hpp"
#include "Parsing.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
topic( const Server& server, Client& client, const std::string& cmd )
{
    (void)server;
    (void)client;
    Channel* channel;

    std::vector<std::string> data = split( cmd, " " );
    data.erase( data.begin() );

    if ( !data.size() )
        return ; // TODO Manage error msg

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

    if ( data.size() == 1 && (*check)[0] == ':' && (*check).length() == 1 ) {
        topicReaply( client, channel, TOPICERR );
        return ( TOPICERR );
    }

    if ( (*check)[0] != ':' ) {
        topicReaply( client, channel, TOPICERR );
        return ( TOPICERR );
    }

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

    if ( !channel ) {
        topicReaply( client, channel, TOPICNOCHANNEL );
        return ( TOPICNOCHANNEL );
    }

    key = channel->GetUser();
    if ( !channel->GetMode( TOPIC_CHANGE_SET ) )
        right = true;
    else if ( !key.count( &client ) ) {
        topicReaply( client, channel, TOPICCLIENTNOINCHANNEL );
        return ( TOPICCLIENTNOINCHANNEL );
    }

    else if ( key[ &client ] )
        right = true;

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
        reply = ": 331 " + clientName + " " + channelName
              + ":TOPIC the channel topic is \"" + channel->GetTopic()
              + "\"\r\n";
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
