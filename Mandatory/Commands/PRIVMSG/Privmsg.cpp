#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "parsing.hpp"



void
privateMessage( Server& server, Client& client, serverstd::string& rawCommand )
{
    if( !rawCommand.find ( ":" ) || rawCommand.find ( ":" ) == std::string::npos )
        return ;

    // grab message
    std::string message = rawCommand.substr( rawCommand.find( ":" ) + 1, std::string::npos );

    // grab target
    std::vector<std::string> splitCmd = split( rawCommand , " " );
    std::string target = splitcmd[1];

    // check target
    if( target.find ( "#" ) == 0 )
    {
        target = target.substr( 1 );
        std::vector<std::string> channels = split( server.GetChannelList(), " " );
        for( int i = 0; i < channels.size(); i++ )
        {
            if( channels[i] == target )
                //send message with Bob function
            else
                return ; // error message channel not found
        }
    }
    else
    {
        std::vector<std::string> clients = split( server.GetClientList(), " " );
        for( int i = 0; i < clients.size(); i++ )
        {
            if( clients[i] == target )
                //send message with Jiji function
            else
                return ; // error message client not found
        }
    }
}

//bon... en gros on cherche si il y a un channel ou un client
// 1 je vire le premier mot qui est PRIVMSG
// 2 je vais chercher le message a envoyer il est apres le ":" SINON ERREUR ?????
// 3 je split a la "," pour avoir les channels et les clients
// 4 je test si c'est un channel ou un client
// 5 je met ca dans un vecteur <string> sans doublon (sinon erreur?????)
// 6 j'envoie le message a tout le monde
// je sais pas comment ce passe l'envoie a chercher
//entre les channels et les client il y a des ","
//je garde ca dans un vecteur <string>, pas de doublon
//mais je fais comment garder mes espaces ??
//je continue jusqu'au ":" ca c'est le debut du message.
//je join les node du vecteur pour avoir le message
//j'envoie le message a tout le monde


// du coup V2 eazy => blc je fais mon proto
// PRIVMSG target :message
// simple et efficace, pas de flexibiliter mais on s'en fou
// je refais ,
// 1 je vire le premier mot qui est PRIVMSG
// 2 je choppe le target qui est le prochain mot
// 3 je garde le message apres le ":"
// 4 j'envoi le message

///////////////////////////////////////////////////////////////////////////////
