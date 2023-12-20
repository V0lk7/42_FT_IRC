#include "Error_code.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Tools.hpp"

void
PrivateMessage( Server& server, Client& client, std::string& rawCommand )
{
    if( rawCommand.find ( ":" ) == std::string::npos || rawCommand.find ( ":" ) == rawCommand.size() - 1 )
		return client.SetMessageToSend(": 412 : No text to send (PRIVMSG)\r\n");

    std::string target = rawCommand.substr( 0, rawCommand.find( ":" ) );

    std::vector<std::string> splitCmd = split( target , " " );

    if( splitCmd.size() < 2 )
		return client.SetMessageToSend(": 411 : No recipient given (PRIVMSG)\r\n");

    else if( splitCmd.size() > 2 )
		return client.SetMessageToSend(": 407 : Duplicate recipients. No message delivered\r\n");

    target = splitCmd[1];

	std::string message(":" + client.GetNickname() + " PRIVMSG ");

    if( target.find ( "#" ) == 0 || target.find( "&" ) == 0 )
    {
        Channel* targetChannel = server.GetChannel( target );
        if( targetChannel ){
			message += targetChannel->GetName() + " " + rawCommand.substr(rawCommand.find(":"));
			message += "\r\n";
            targetChannel->SendMessageToClients( message, client );
		}
        else
			return client.SetMessageToSend(": 401 : " + target + " :No such nick/channel\r\n");
    }
    else
    {
        Client* targetClient = server.GetClient( target );
        if( targetClient && targetClient->GetStatement() == true){
			message += targetClient->GetNickname() + " " + rawCommand.substr(rawCommand.find(":"));
			message += "\r\n";
            targetClient->SetMessageToSend( message );
		}
        else
			return client.SetMessageToSend(": 401 : " + target + " :No such nick/channel\r\n");
    }
}
