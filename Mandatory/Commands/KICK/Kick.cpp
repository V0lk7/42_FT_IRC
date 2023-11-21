#include <iostream>
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
// #-> need hard testing                                                    # //
// #-> need to implement checking right of client kickmaker                 # //
// #-> need str is : KICK [&#]CHANNEL target                                # //
// ########################################################################## //

void
kick( const Server& server, const Client& client, Channel& channel,
                                                        const std::string& cmd )
{
    (void)server;
    (void)client;
    Client kicker( client );

    if ( parseCmd( cmd , channel, kicker ) != NONE ) {
        std::cerr << "\t*NEED MSG HANDLING*" << std::endl;                       // TODO 1.
        return ;
    }

}
