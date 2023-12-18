#include "Kick.hpp"
#include "Tools.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include <vector>

#include <iostream>

// ########################################################################## //
// #_TODO___________________________________________________________________# //
// #-> better handling error msg: 1.                                        # //
// #-> need check if [&#] is present TODO                                   # //
// #-> need to implement checking right of client kickmaker                 # //
// #-> need str is : KICK [&#]CHANNEL target TODO MISSING FORMAT CHECKING   # //
// #               : MISSING MULTIPLE KICKING FORMAT AND HAVE TO CHANNEL    # //
// #               : AFTER [#&]                                             # //
// ########################################################################## //

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
    if ( !splitOnSpace.size() )
        return ( EMPTY );

    if ( !channel ) {
        kickReaply( client, channel, NOCHANNEL );
        return ( NOCHANNEL );
    }

    if ( checkRight( *channel, client ) != CONTINUE ) {
        kickReaply( client, channel, NORIGHT );
        return ( NORIGHT );
    }

    if ( findTarget( splitOnSpace[1], *channel, client ) != CONTINUE ) {
        kickReaply( client, channel, NOTARGET );
        return ( NOTARGET );
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

    if ( key == client.GetNickname() )
        found = false;
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
              + " :They are not on that channel."
              + "\r\n";
    }

    else if ( flag == NORIGHT ) {
        reply = ": 482 " + clientName + " " + channelName
              + " :You must be a channel operator."
              + "\r\n";
    }

    else if ( flag == NOCHANNEL ) {
        reply = ": 476 " + clientName +
              + " :Command is invalid or improperly formatted."
              + "\r\n";
    }
    else 
        reply = "" ;

    client.SetMessageToSend( reply );
}
