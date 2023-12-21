#include "Tools.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
	if (channel == NULL){
		client.SetMessageToSend(": 403 : " + data[0] + " :No such channel\r\n");
		return ;
	}
	if (channel->UserInChannel(client) == false){
		topicReaply(client, channel, TOPICCLIENTNOINCHANNEL);
		return ;
	}
    if ( data.size() == 1 ) {
        topicReaply( client, channel, TOPICSEND );
        return ;
    }

    if ( topicParsing( client, channel) != TOPICNONE )
        return ;

    topicChange( data, channel, client );
}

static enum TErr
topicChange( std::vector<std::string>& data, Channel* channel, Client& client )
{
    std::string                         topic;
    std::string                         output;
    std::vector<std::string>::iterator  check = data.begin();

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
    while ( check != data.end() ) {
        topic += " " + *check;
        check++;
    }
    topic = topic.substr( 1, std::string::npos );

    channel->SetTopic( topic );
    topicReaply( client, channel, TOPICCHANGED );

    return ( TOPICNONE );
}

static enum TErr
topicParsing( Client& client, Channel* channel )
{
    std::map<Client*, bool> key; 
    bool                    right = false;

    key = channel->GetUsers();
    if ( !channel->GetMode( TOPIC_CHANGE_SET ) ) {
        right = true;
    }

    if ( !key.count( &client ) ) {
        topicReaply( client, channel, TOPICCLIENTNOINCHANNEL );
        return ( TOPICCLIENTNOINCHANNEL );
    }
    else if ( key[ &client ] ) {
        right = true;
    }
    else if ( !right ) {
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
        channelName = channel->GetName() ;

    if ( flag == TOPICCLIENTNOINCHANNEL) {
        reply = ": 403 : " + channelName
              + " :You're not on that channel"
              + "\r\n";
    }

    else if ( flag == TOPICNORIGHT ) {
        reply = ": 482 : " + channelName
              + " :You're not channel operator"
              + "\r\n";
    }

    else if ( flag == TOPICERR ) {
        reply = ": 461 : TOPIC :Not enough parameters\r\n";
    }

    else if ( flag == TOPICSEND ) {
        if ( !channel->GetTopic().empty() ) {
            reply = ": 332 " + clientName + " " + channelName + " :"
                  + channel->GetTopic() + "\r\n";
        }
        else {
            reply += ": 331 " + clientName + " " + channelName
                  + " :No topic is set.\r\n";
        }
    }

    else if ( flag == TOPICCHANGED ) {
        reply =  ":" + clientName + " TOPIC " + channelName + " :" + channel->GetTopic()
              + "\r\n";
    }

    else
        reply = "" ;

    client.SetMessageToSend( reply );
}
