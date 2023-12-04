#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include <exception>
#include <stdexcept>

/* Paramètres: <pseudonyme> <canal> */

typedef enum IErr {
    NEXT, NOTARGETINSERVER, TARGETALREADYINCHANNEL,
    CLIENTISTARGET, BADRIGHT, BADCHANNEL, TARGETINWAITLIST
} IErr;

static Client&
extractTarget( const std::string& key, Server& server );
static IErr
findTargetInServer( const std::string& target, Server& server );
static IErr
targetAlreadyInChannel( Channel& channel, const std::string& target );
static IErr
hostRight( Channel& channel, Client& client );
static IErr
isValidRight( Client& client, Channel& channel, std::string& target );
static bool
inviteParsing( std::vector<std::string>& key, Server& server,
               Client& client, Channel& channel );
static void
inviteReaply( Client& client, Client* target, Channel* channel, int flag );

void
invite( Server& server, Client& client, const std::string& cmd )
{
    (void)server;
    (void)client;
    Channel*                    channel;
    std::vector<std::string>    cuttingCmd = split( cmd, " " );
    Client*                     target = NULL;

    cuttingCmd.erase( cuttingCmd.begin() );
    if ( cuttingCmd.size() != 2 )
        return ;

    channel = server.GetChannel( cuttingCmd[0] );

    if ( !inviteParsing( cuttingCmd, server, client, *channel ) )
        return ;

    try {
        target = &extractTarget( cuttingCmd[1] , server );
        inviteReaply( client, target, channel, NEXT );
    }
    catch ( std::exception& e) {}
}

static Client&
extractTarget( const std::string& key, Server& server )
{
    std::list<Client*>              clientList( server.getCllist() );
    std::list<Client*>::iterator    check = clientList.begin();
    while ( check != clientList.end() ) {
        if ( (**check).GetNickname() == key )
            return ( **check );
        check++;
    }

    throw std::logic_error( "UNREACHABLE" );
}

static bool
inviteParsing( std::vector<std::string>& key, Server& server,
               Client& client, Channel& channel )
{
    if ( findTargetInServer( key[0], server ) != NEXT )
        return ( false );

    if ( isValidRight( client, channel, key[0] ) != NEXT )
        return ( false );

    else
        return ( true );
}

static IErr
isValidRight( Client& client, Channel& channel, std::string& target )
{
    if ( client.GetNickname() == target ) 
        return ( CLIENTISTARGET );

    if ( targetAlreadyInChannel( channel, target ) != NEXT )
        return ( TARGETALREADYINCHANNEL );

    if ( channel.GetMode( INVITE_ONLY_SET ) || !hostRight( channel, client ) )
        return ( BADRIGHT );

    else
        return ( NEXT );
}

static IErr
hostRight( Channel& channel, Client& client )
{
    std::map<Client*, bool> clientList( channel.GetUsers() );
    if ( clientList.count( &client ) && clientList[ &client ] )
        return ( NEXT );
    else
        return ( BADRIGHT );
}

static IErr
targetAlreadyInChannel( Channel& channel, const std::string& target )
{
    std::map<Client*, bool>             clientInChannel( channel.GetUsers() );
    std::list<Client*>                  clientInWaitingList( channel.GetWaitingList() );
    std::map<Client*, bool>::iterator   check = clientInChannel.begin();
    std::list<Client*>::iterator        checkWL = clientInWaitingList.begin();

    while ( check != clientInChannel.end() ) {
        if ( check->first->GetNickname() == target )
            return ( TARGETALREADYINCHANNEL );
        check++;
    }
    while ( checkWL != clientInWaitingList.end() ) {
        if ( (**checkWL).GetNickname() == target )
            return ( TARGETINWAITLIST );
        checkWL++;
    }
    return ( NEXT );
}

static IErr
findTargetInServer( const std::string& target, Server& server )
{
    std::list<Client*>              clientList( server.getCllist() );
    std::list<Client*>::iterator    check = clientList.begin();

    while ( check != clientList.end() ) {
        if ( (**check).GetNickname() == target )
            return ( NEXT );
        check++;
    }

    return ( NOTARGETINSERVER );
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
        channelName = (*channel).GetName();
    if ( target )
        clientTarget = (*target).GetNickname();

    if ( flag == NOTARGETINSERVER ) {
        reaply = ": 401 " + clientName + " " + channelName
               + ":INVITE cannot access to the target mentioned in server."
               + "\r\n";
    }

    else if ( flag == BADRIGHT ) {
        reaply = ": 473 " + clientName + " " + channelName
               + ":INVITE You are not a channel operator."
               + "\r\n";
    }

    else if ( flag == BADCHANNEL ) {
        reaply = ": 442 " + clientName +
               + ":INVITE command is invalid or improperly formatted."
               + "\r\n";
    }

    else if ( flag == TARGETALREADYINCHANNEL ) {
        reaply = ": 443 " + clientName + " "  + channelName
               + ":INVITE target is already in channel."
               + "\r\n";
    }

    else if ( flag == TARGETINWAITLIST ) {
        reaply = ": 443 " + clientName + " "  + channelName
               + ":INVITE target is already invited."
               + "\r\n";
    }

    else if ( flag == CLIENTISTARGET ) {
        reaply = ": 401 " + clientName + " "  + channelName
               + ":INVITE you can't invite yourself in a channel."
               + "\r\n";
    }

    else if ( flag == NEXT ) {
        reaply = ": 301 " + clientName + " " + channelName
               + ":INVITE you've been invited in channel."
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
