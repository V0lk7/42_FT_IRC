#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
// ########################################################################## //
// #_TODO_TOOLS____________________________________________________________ # //
#include <iostream>
// ########################################################################## //

typedef enum IErr {
    NEXT, NOTARGETINSERVER, TARGETALREADYINCHANNEL,
    CLIENTISTARGET, BADRIGHT, BADCHANNEL, TARGETINWAITLIST,
    BADPARAMS
} IErr;

static IErr
targetAlreadyInChannel( Channel& channel, const std::string& target, Client& client );
static IErr
isValidRight( Client& client, Channel& channel, std::string& target );
static bool
inviteParsing( std::vector<std::string>& key, Server& server,
               Client& client, Channel* channel );
static void
inviteReaply( Client& client, Client* target, Channel* channel, int flag );

void
Invite( Server& server, Client& client, const std::string& cmd )
{
    Channel*                    channel;
    std::vector<std::string>    cuttingCmd = split( cmd, " " );
    Client*                     target = NULL;

    cuttingCmd.erase( cuttingCmd.begin() );
    if ( cuttingCmd.size() != 2 ) {
        inviteReaply( client, NULL, NULL, BADPARAMS );
        return ;
    }

    channel = server.GetChannel( cuttingCmd[1] );

    if ( !inviteParsing( cuttingCmd, server, client, channel ) )
        return ;

    target = server.GetClient( cuttingCmd[0] );
    channel->PutClientOnWaitingList( *target );
    inviteReaply( client, target, channel, NEXT );
}

static bool
inviteParsing( std::vector<std::string>& key, Server& server,
               Client& client, Channel* channel )
{
    if ( !channel ) {
        inviteReaply( client, NULL, NULL, BADPARAMS );
        return ( false );
    }

    if ( !server.GetClient( key[0] ) ) {
        inviteReaply( client, NULL, channel, NOTARGETINSERVER );
        return ( false );
    }

    if ( isValidRight( client, *channel, key[0] ) != NEXT )
        return ( false );

    else
        return ( true );
}

static IErr
isValidRight( Client& client, Channel& channel, std::string& target )
{
    if ( client.GetNickname() == target ) {
        inviteReaply( client, NULL, &channel, CLIENTISTARGET );
        return ( CLIENTISTARGET );
    }

    if ( targetAlreadyInChannel( channel, target, client ) != NEXT )
        return ( TARGETALREADYINCHANNEL );

    if ( channel.GetMode( INVITE_ONLY_SET )
        && !channel.IsClientOperator( client ) )
    {
        inviteReaply( client, NULL, &channel, BADRIGHT );
        return ( BADRIGHT );
    }

    else
        return ( NEXT );
}

static IErr
targetAlreadyInChannel( Channel& channel, const std::string& target, Client& client )
{
    std::map<Client*, bool>             clientInChannel( channel.GetUsers() );
    std::list<Client*>                  clientInWaitingList( channel.GetWaitingList() );
    std::map<Client*, bool>::iterator   check = clientInChannel.begin();
    std::list<Client*>::iterator        checkWL = clientInWaitingList.begin();

    while ( check != clientInChannel.end() ) {
        if ( check->first->GetNickname() == target ) {
            inviteReaply( client, NULL, &channel, TARGETALREADYINCHANNEL );
            return ( TARGETALREADYINCHANNEL );
        }
        check++;
    }
    while ( checkWL != clientInWaitingList.end() ) {
        if ( (**checkWL).GetNickname() == target ) {
            inviteReaply( client, NULL, &channel, TARGETINWAITLIST );
            return ( TARGETINWAITLIST );
        }
        checkWL++;
    }
    return ( NEXT );
}

// ########################################################################## //
// #_inviteReaply___________________________________________________________# //
static void
inviteReaply( Client& client, Client* target, Channel* channel, int flag )
{
    std::string reaply;
    std::string clientName( client.GetNickname() );
    std::string clientTarget;
    std::string channelName;

    if ( channel )
        channelName = channel->GetName();
    if ( target )
        clientTarget = target->GetNickname();

    if ( flag == NOTARGETINSERVER ) {
        reaply = ": 401 :" + clientName + " " + channelName
               + " cannot access to the target mentioned in server"
               + "\r\n";
    }

    else if ( flag == BADRIGHT ) {
        reaply = ": 473 :" + clientName + " " + channelName
               + " You are not a channel operator"
               + "\r\n";
    }

    else if ( flag == BADCHANNEL ) {
        reaply = ": 442 :" + clientName +
               + " command is invalid or improperly formatted"
               + "\r\n";
    }

    else if ( flag == TARGETALREADYINCHANNEL ) {
        reaply = ": 443 :" + clientName + " "  + channelName
               + " target is already in channel"
               + "\r\n";
    }

    else if ( flag == TARGETINWAITLIST ) {
        reaply = ": 443 :" + clientName + " "  + channelName
               + " target is already invited"
               + "\r\n";
    }

    else if ( flag == CLIENTISTARGET ) {
        reaply = ": 401 :" + clientName + " "  + channelName
               + " you can't invite yourself in a channel"
               + "\r\n";
    }

    else if ( flag == BADPARAMS ) {
        reaply = ": 461 :" + clientName +
               + " command is invalid or improperly formatted"
               + "\r\n";
    }

    else if ( flag == NEXT ) {
        reaply = "INVITE " + clientName + " " + channelName
               + "\r\n";
    }

    else
        reaply = "" ;

    if ( target )
        target->SetMessageToSend( reaply );
    else
        client.SetMessageToSend( reaply );
}
// ########################################################################## //
