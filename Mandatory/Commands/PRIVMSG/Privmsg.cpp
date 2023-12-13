#include "Error_code.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include <sstream>

static void
privMsgError( int code, std::string message, Client& client)
{
    std::ostringstream strCode;
    strCode << ": " << code;
    std::string fullMessage = strCode.str();
    fullMessage += " PRIVMSG : " + message + "\r\n";
    client.SetMessageToSend( fullMessage );
}

void
PrivateMessage( Server& server, Client& client, std::string& rawCommand )
{
    if( rawCommand.find ( ":" ) == std::string::npos || rawCommand.find ( ":" ) == rawCommand.size() - 1 )
        return privMsgError( ERR_NOTEXTTOSEND, "No message", client );

    std::string target = rawCommand.substr( 0, rawCommand.find( ":" ) );

    std::vector<std::string> splitCmd = split( target , " " );

    if( splitCmd.size() < 2 )
        return privMsgError( ERR_NORECIPIENT, "No recipient", client);

    else if( splitCmd.size() > 2 )
        return privMsgError( ERR_NORECIPIENT, "Too many recipients", client);

    target = splitCmd[1];

    std::string message = client.GetNickname();
    message += " : ";
    message += rawCommand.substr( rawCommand.find( ":" ) + 1 );
    message += "\r\n";

    if( target.find ( "#" ) == 0 || target.find( "&" ) == 0 )
    {
        Channel* targetChannel = server.GetChannel( target );
        if( targetChannel )
            targetChannel->SendMessageToClients( message, client );
        else
            return privMsgError( ERR_NOSUCHCHANNEL, "Unknown channel", client);
    }
    else
    {
        Client* targetClient = server.GetClient( target );
        if( targetClient )
            targetClient->SetMessageToSend( message );
        else
            return privMsgError( ERR_NOSUCHNICK, "Unknown user", client);
    }
}
