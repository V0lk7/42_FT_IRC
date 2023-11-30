#include "Kick.hpp"

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
checkRight( const Channel& channel, Client& client );
static enum Err
findTarget( std::vector<std::string>& data, const Channel& channel, Client& client );
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

    if ( findTarget( splitOnSpace, *channel, client ) != CONTINUE ) {
        kickReaply( client, channel, NOTARGET );
        return ( NOTARGET );
    }

    return ( NONE );
}

// ########################################################################## //
// #_TOOLS__________________________________________________________________# //

static enum Err
findTarget( std::vector<std::string>& data, const Channel& channel, Client& client )
{
    std::map<Client*, bool> target( channel.GetUser() );
    bool                    found = false;
    size_t                  pos = 0;
    std::string             himself;

    for ( std::vector<std::string>::iterator itData = data.begin();
                                       itData != data.end(); itData++, pos++ ) {
        for ( std::map<Client*, bool>::iterator itTarget = target.begin();
                                        itTarget != target.end(); itTarget++ ) {
            if ( !itData->empty() && pos == 1 &&
                                   itTarget->first->GetNickname() == *itData ) {
                himself = *itData;
                found = true;
                break ;
            }
        }
    }
    if ( himself == client.GetNickname() )
        found = false;                                                           // TODO maybe specify it's
    return ( found ? CONTINUE : NOTARGET );                                      // hiself
}

static enum Err
checkRight( const Channel& channel, Client& client )
{
    std::map<Client*, bool> target( channel.GetUser() );
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
        std::string channelName = (*channel).GetName() ;

    if ( flag == NOTARGET ) {
        reply = ": 442 " + clientName + " " + channelName
              + ":KICK cannot access to the target mentioned and has kicked it."
              + "\r\n";
    }

    else if ( flag == NORIGHT ) {
        reply = ": 482 " + clientName + " " + channelName
              + ":KICK You are not an operator"
              + "\r\n";
    }

    else if ( flag == NOCHANNEL ) {
        reply = ": 476 " + clientName +
              + ":KICK command is invalid or improperly formatted."
              + "\r\n";
    }
    else 
        reply = "" ;

    client.SetMessageToSend( reply );
}
