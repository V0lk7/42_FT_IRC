#include "Privmsg.hpp"

#define ERR_NORECIPIENT		441
#define ERR_NOTEXTTOSEND	412
#define ERR_NOSUCHNICK		401
#define ERR_NOSUCHCHANNEL	403

static void
privMsgError( int code, std::string message, Client& client)
{
    std::ostringstream strCode;
    strCode << ": " << code;
    std::string fullMessage = strCode.str();
    fullMessage += " PRIVMSG : " + message + ".\r\n";
    client.SetMessageToSend( fullMessage );
}

void
privateMessage( Server& server, Client& client, std::string& rawCommand )
{
    // check message
    if( rawCommand.find ( ":" ) == std::string::npos )
        return privMsgError( ERR_NOTEXTTOSEND, "No message", client );

    // grab target
    std::string target = rawCommand.substr( 0, rawCommand.find( ":" ) );

    std::vector<std::string> splitCmd = split( target , " " );

    if( splitCmd.size() < 2 )
        return privMsgError( ERR_NORECIPIENT, "No recipient", client);

    else if( splitCmd.size() > 2 )
        return privMsgError( ERR_NORECIPIENT, "Too many recipients", client);

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
