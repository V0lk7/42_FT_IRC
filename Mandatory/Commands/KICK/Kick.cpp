#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "Tools.hpp"
#include "Parsing.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include "Kick.hpp"

// ########################################################################## //
// #_TODO___________________________________________________________________# //
// #-> better handling error msg: 1.                                        # //
// ########################################################################## //

static std::string
msgMaker( Client& client, Channel& channel, std::vector<std::string>& data );
static void
rmClientOfChannel( Channel& channel, const std::string& key,
                                                    const std::string& reason );

void
kick( const Server& server, Client& client, Channel& channel,
                                                        const std::string& cmd )
{
    (void)server;
    (void)client;

    std::string reason;
    std::vector<std::string> data = split( cmd, "" );
    data.erase( data.begin() );


    if ( parseCmd( cmd , channel, client ) != NONE ) {
        std::cerr << "\t*NEED MSG HANDLING*" << std::endl;                       // TODO 1.
        return ;
    }
    reason = msgMaker( client, channel, data );
    rmClientOfChannel( channel, data[2], reason );
    return ;
}

static void
rmClientOfChannel( Channel& channel, const std::string& key,
                                                     const std::string& reason )
{
    Client* kicked = NULL;
    std::map<Client*, bool> target( channel.GetUser() );

    for ( std::map<Client*, bool>::iterator it = target.begin();
                                                      it != target.end(); it++ )
    {

        if ( it->first->GetNickname() == key )
            *kicked = *it->first;

        send( it->first->GetSocket(),
                                  reason.c_str(), strlen( reason.c_str() ), 0 );
    }
    if ( kicked )
        channel.EraseClientFromChannel( *kicked );
}

static std::string
msgMaker( Client& client, Channel& channel, std::vector<std::string>& data )
{
    std::string msg;
    if ( data.size() < 2 ) {
        msg = client.GetNickname() + " KICK " + data[2] + " to #"
            + channel.GetName() + "\r\n";
    }
    else
    {
        msg = client.GetNickname() + " KICK " + data[2] + " to #"
            + channel.GetName() + " ";

        for ( std::vector<std::string>::iterator it = data.begin() + 3;
                                                        it != data.end(); it++ )
            msg += *it;

        msg += "\r\n";
    }
    return ( msg );
}
