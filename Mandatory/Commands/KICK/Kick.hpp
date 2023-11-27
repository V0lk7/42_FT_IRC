#ifndef KICK_HPP
# define KICK_HPP

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

// ########################################################################## //
// #_TODO___________________________________________________________________# //
// #-> better handling error msg: 1.                                        # //
// #-> need hard testing                                                    # //
// #-> need to implement checking right of client kickmaker                 # //
// #-> need str is : KICK [&#]CHANNEL target                                # //
// ########################################################################## //

typedef enum Err { NONE, EMPTY, NOTARGET, CONTINUE, NOCHANNEL, NORIGHT } Err;

enum Err
parseCmd( const std::string& cmd, const Channel& channel, Client& client );

#endif 
