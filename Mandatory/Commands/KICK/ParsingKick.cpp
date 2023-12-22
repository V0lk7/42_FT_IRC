#include "Kick.hpp"
#include "Tools.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include <vector>

static enum Err
checkRight( Channel& channel, Client& client );
static enum Err
findTarget( std::string& key, Channel& channel, Client& client );
static void
kickReaply( Client& client, Channel* channel, int flag );

// ########################################################################## //
// #_PARSER_________________________________________________________________# //

enum Err
parseCmd( const std::string& cmd, Channel* channel, Client& client )
{
    std::vector<std::string> splitOnSpace = split( cmd , " " );

    splitOnSpace.erase( splitOnSpace.begin() );
	if (splitOnSpace[0] == "Irc42_JCJ"){
		splitOnSpace.erase(splitOnSpace.begin());
		splitOnSpace[1].erase(splitOnSpace[1].begin());
	}

    if ( !splitOnSpace.size() )
        return ( EMPTY );
	else if (splitOnSpace.size() < 2){
        kickReaply( client, channel, ERR_PARAMS );
        return ( ERR_PARAMS );
	}
    if ( !channel ) {
        kickReaply( client, channel, NOCHANNEL );
        return ( NOCHANNEL );
    }

    if ( checkRight( *channel, client ) != CONTINUE ) {
        kickReaply( client, channel, NORIGHT );
        return ( NORIGHT );
    }

	enum Err	Flag = findTarget( splitOnSpace[1], *channel, client ); 
    if ( Flag != CONTINUE ) {
        kickReaply( client, channel, Flag );
        return ( Flag );
    }

    return ( NONE );
}

// ########################################################################## //
// #_TOOLS__________________________________________________________________# //

static enum Err
findTarget( std::string& key, Channel& channel, Client& client )
{
    std::map<Client*, bool> target( channel.GetUsers() );
    bool                    found = false;

    for ( std::map<Client*, bool>::iterator itTarget = target.begin();
                  itTarget != target.end(); itTarget++                 )
    {
        if ( itTarget->first->GetNickname() == key ) {
            found = true; break ;
        }
    }

    if ( found == true && key == client.GetNickname() )
        return (HIMSELF);
    return ( found ? CONTINUE : NOTARGET );
}

static enum Err
checkRight( Channel& channel, Client& client )
{
    std::map<Client*, bool> target( channel.GetUsers() );
    if ( target.count( &client ) != 0 && target[&client] == true )
        return ( CONTINUE );
    else
        return ( NORIGHT );
}

// ########################################################################## //
// #_kickReaply_____________________________________________________________# //
static void
kickReaply( Client& client, Channel* channel, int flag )
{
    std::string reply;
    std::string clientName( client.GetNickname() );
    std::string channelName;

    if ( channel )
        channelName = channel->GetName() ;

    if ( flag == NOTARGET ) {
        reply = ": 441 " + clientName + " " + channelName
              + " :Target not on the channel"
              + "\r\n";
    }

    else if ( flag == NORIGHT ) {
        reply = ": 482 " + clientName + " " + channelName
              + " :You're not channel operator"
              + "\r\n";
    }

    else if ( flag == NOCHANNEL ) {
        reply = ": 476 " + clientName +
              + " :No such channel"
              + "\r\n";
    }
	else if (flag == HIMSELF){
        reply = ": 400 " + clientName + " " + channelName
              + " :You can't kick yourself"
              + "\r\n";
	}
	else if (flag == ERR_PARAMS){
		if (channelName.empty() == false)
			reply	= ": 461 " + clientName + " " + channelName + 
					+ " :Not enough parameters\r\n";
		else
			reply	= ": 461 " + clientName + " :Not enough parameters\r\n";

	}
    else 
        reply = "" ;

    client.SetMessageToSend( reply );
}
