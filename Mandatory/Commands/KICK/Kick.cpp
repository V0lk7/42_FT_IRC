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
// #-> check idx 4 not sur i guess it would be 3                            # //
// #-> care about commment code to test: 2.                                 # //
// #-> Send msg handling need to be modify put msg in client buffer         # //
// #        ClientMsgToSend                                                 # //
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
    std::vector<std::string> data = split( cmd, " " );
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
    std::map<Client*, bool> target( channel.GetUsers() );

    for ( std::map<Client*, bool>::iterator it = target.begin();
                                                      it != target.end(); it++ )
    {

        (void)reason;
        // send( it->first->GetSocket(),                                          // TODO 2.
        //                           reason.c_str(), strlen( reason.c_str() ), 0 );

        if ( it->first->GetNickname() == key )
            channel.EraseClientFromChannel( *it->first );
    }
}

static std::string
msgMaker( Client& client, Channel& channel, std::vector<std::string>& data )
{
    std::string msg;
    if ( data.size() <= 2 ) {
        msg = client.GetNickname() + " KICK " + data[1] + " to #"
            + channel.GetName() + "\r\n";
    }
    else
    {
        msg = client.GetNickname() + " KICK " + data[1] + " to #"
            + channel.GetName();

        for ( size_t idx = 4; idx < data.size(); idx++ )                         // TODO not sur about
            msg += " " + data[idx];                                              // idx = 4
        msg += "\r\n";
    }
    return ( msg );
}
