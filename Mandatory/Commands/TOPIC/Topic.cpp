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
// ########################################################################## //

typedef enum TErr { TOPICNONE, NOTOPIC, TOPICNORIGHT, TOPICNOCHANNEL,
                                                                TOPICERR } TErr;

static enum TErr
topicParsing( std::vector<std::string>& data, Client& client, Channel& channel );
static enum TErr
topicChange( std::vector<std::string>& data, Channel& channel );

void
topic( const Server& server, Client& client, Channel& channel,
                                                        const std::string& cmd )
{
    (void)server;
    (void)client;
    (void)channel;

    std::vector<std::string> data = split( cmd, " " );
    data.erase( data.begin() );

    if ( data.size() && topicParsing( data, client, channel) != TOPICNONE ) {
        // TODO error handling
    }
    if ( data.size() == 1 )
    {
        send ( client.GetSocket(), channel.GetTopic().c_str(),
                                                 channel.GetTopic().size(), 0 );
        return ;
    }
    if ( topicChange( data, channel ) != TOPICNONE ) {
        // TODO sending error;
        return ;
    }
}

static enum TErr
topicChange( std::vector<std::string>& data, Channel& channel )
{
    std::string topic;
    std::vector<std::string>::iterator check = data.begin();

    if ( data.size() == 1 && (*check)[0] != ':' && (*check).length() == 1 )
        return ( TOPICERR );

    while ( check++ != data.end() )
        topic += " " + *check;
    topic = topic.substr( 1, std::string::npos );
    channel.SetTopic( topic );

    return ( TOPICNONE );
}

static enum TErr
topicParsing( std::vector<std::string>& data, Client& client, Channel& channel )
{
    std::map<Client*, bool> key( channel.GetUser() );
    std::vector<std::string>::iterator check = data.begin();

    if ( !check->empty() && ( (*check)[0] != '#' || (*check)[0] != '&' )
                && (*check).substr( 1, std::string::npos ) != channel.GetName() )
        return ( TOPICNOCHANNEL );

    if ( !key.count( &client ) || !key[ &client ] )
        return ( TOPICNORIGHT );

    return ( TOPICNONE );
}
