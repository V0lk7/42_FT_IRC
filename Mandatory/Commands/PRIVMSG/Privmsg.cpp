#include "privmsg.hpp"
#include "Error_code.hpp"

void
privateMessage( Server& server, Client& client, serverstd::string& rawCommand )
{
    // check target
    if( rawCommand.find ( ":" ) == 0 )
        return client.SetMessageToSend( ERR_NORECIPIENT );

    // check message
    if( rawCommand.find ( ":" ) == std::string::npos )
        return client.SetMessageToSend( ERR_NOTEXTTOSEND );
    
    // grab message
    std::string message = rawCommand.substr( rawCommand.find( ":" ) + 1, std::string::npos );

    // grab target
    std::vector<std::string> splitCmd = split( rawCommand , " " );
    std::string target = splitcmd[1];

    // check target
    if( target.find ( "#" ) == 0 || target.find( "&" ) == 0 )
    {
        std::vector<std::string> channels = split( server.GetChannelList(), " " );
        for( int i = 0; i < channels.size(); i++ )
        {
            if( channels[i] == target )
                channel[i].SendMessageToClients( message, client );
            else
                return ( client.SetMessageToSend( ERR_NOSUCHCHANNEL ));
        }
    }
    else
    {
        std::vector<std::string> clients = split( server.GetClientList(), " " );
        for( int i = 0; i < clients.size(); i++ )
        {
            if( clients[i] == target )
                client[i].SetMessageToSend( message );
            else
                return ( client.SetMessageToSend( ERR_NOSUCHNICK ));
        }
    }
}

//TEST_A_FAIRE :
// PRIVMSG jean : Unmot
// PRIVMSG jean : Deux mots
// PRIVMSG jean : Beaucoup                 d'espace
// PRIVMSG jean : Ch@r@ct3r&s @ |a c0n !<>,./$%^*()_+*:
// PRIVMSGjean : Commande colle au user
// PRIVMSG jean: Deux points colle au user
// PRIVMSG jean :Deux points colle au message
// PRIVMSG pacome : Utilisateur inconnu
// PRIVMSG jean charles : 2 user
// PRIVMSG &jean : E commercial devant jean
// PRIVMSG #Test : Test est un channel qui existe
// PRIVMSG #wrongchannel : wrongchannel est un channel qui existe pas
