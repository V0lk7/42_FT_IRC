#include "Privmsg.hpp"
#include "Error_code.hpp"
#include "Tools.hpp"

void
privateMessage( Server& server, Client& client, std::string& rawCommand )
{
    // check message
    if( rawCommand.find ( ":" ) == std::string::npos )
        return client.SetMessageToSend( "ERROR : no message" );

    // grab target
    std::string target = rawCommand.substr( 0, rawCommand.find( ":" ) );

    std::vector<std::string> splitCmd = split( target , " " );

    if( splitCmd.size() < 2 )
        return client.SetMessageToSend( "ERROR : no recipients" );

    else if( splitCmd.size() > 2 )
        return client.SetMessageToSend( "ERROR : too many recipients" );

    target = splitCmd[1];

    // grab message
    std::string message = rawCommand.substr( rawCommand.find( ":" ) + 1 );

    // check target
    if( target.find ( "#" ) == 0 || target.find( "&" ) == 0 )
    {
        Channel* targetChannel = server.GetChannel( target );
        if( targetChannel )
            targetChannel->SendMessageToClients( message, client );
        else
            return ( client.SetMessageToSend( "ERROR : unknown channel" ));
    }
    else
    {
        Client* targetClient = server.GetClient( target );
        if( targetClient )
            targetClient->SetMessageToSend( message );
        else
            return ( client.SetMessageToSend( "ERROR : unknown user" ));
    }
}
