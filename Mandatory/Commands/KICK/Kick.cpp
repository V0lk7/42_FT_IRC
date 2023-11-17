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

void
kick( const Server& server, Client& client, Channel& channel, const std::string& cmd ) {
    std::string str(cmd);
    std::string msg;
    std::vector<std::string> words;

    if ( !client.getRight() ) {
        msg = client.GetUsername() + " haven't right to kick\n";
        send( client.GetSocket(), msg.c_str(), strlen( msg.c_str() ) , 0 );
        return ;
    }
    words = split( str, "," );
}
