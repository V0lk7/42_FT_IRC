#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include <exception>

/* Paramètres: <pseudonyme> <canal> */

static Client&
extractTarget( const std::string& key, Server& server );
static bool
findTargetInServer( const std::string& target, Server& server );
static Channel*
findChannel( const std::string& target, Server& server );
static bool
targetAlreadyInChannel( Channel& channel, const std::string& target );
static bool
hostRight( Channel& channel, Client& client );
static bool
isValidRight( Client& client, Channel& channel, std::string& target );
static bool
inviteParsing( std::vector<std::string>& key, Server& server,
                                             Client& client, Channel& channel );

static void
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

    channel = findChannel( cuttingCmd[1], server );

    if ( !channel || !inviteParsing( cuttingCmd, server, client, *channel ) ) {
        // TODO MANAGING ERROR
        return ;
    }

    target = &extractTarget( cuttingCmd[1] , server );
    target->SetMessageToSend( "GPT" );

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

    throw std::runtime_error( "UNREACHABLE" );
}

static bool
inviteParsing( std::vector<std::string>& key, Server& server,
                                              Client& client, Channel& channel )
{
    if ( !findTargetInServer( key[0], server ) )
        return ( false );

    if ( !isValidRight( client, channel, key[0] ) )
        return ( false );

    else
        return ( true );
}

static bool
isValidRight( Client& client, Channel& channel, std::string& target )
{
    if ( client.GetNickname() == target ||
                                     targetAlreadyInChannel( channel, target ) )
        return ( false );

    if ( channel.GetMode( INVITE_ONLY ) || !hostRight( channel, client ) )
        return ( false );

    else
        return ( true );
}

static bool
hostRight( Channel& channel, Client& client )
{
    std::map<Client*, bool> clientList( channel.GetUsers() );
    if ( clientList.count( &client ) && clientList[ &client ] )
        return ( true );
    else
        return ( false );
}

static bool
targetAlreadyInChannel( Channel& channel, const std::string& target )
{
    std::map<Client*, bool>             clientInChannel( channel.GetUsers() );
    std::map<Client*, bool>::iterator   check = clientInChannel.begin();

    while ( check != clientInChannel.end() ) {
        if ( check->first->GetNickname() == target )
            return ( true );
        check++;
    }
    return ( false );
}

static bool
findTargetInServer( const std::string& target, Server& server )
{
    std::list<Client*>              clientList( server.getCllist() );
    std::list<Client*>::iterator    check = clientList.begin();
    bool                            found = false;
    while ( check != clientList.end() ) {
        if ( (**check).GetNickname() == target )
            found = true;
        check++;
    }
    return ( found );
}

static Channel*
findChannel( const std::string& target, Server& server ) {
    return ( server.GetChannel( target ) );
}
