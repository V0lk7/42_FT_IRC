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

// enum Err { NONE, EMPTY, NOTARGET, CONTINUE, NOCHANNEL, NORIGHT };

// static enum Err
// checkRight( const Channel& channel, Client& client );
// static enum Err
// findChannel( std::vector<std::string> data, const Channel& channel );
// static enum Err
// findTarget( std::vector<std::string> data, const Channel& channel );
// static enum Err
// parseCmd( const std::string& cmd, const Channel& channel, Client& client );

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

// ########################################################################## //
// #_PARSER_________________________________________________________________# //

// static enum Err
// parseCmd( const std::string& cmd, const Channel& channel, Client& client )
// {
//     std::vector<std::string> splitOnSpace;
//
//     splitOnSpace = split( cmd , " " );
//     splitOnSpace.erase( splitOnSpace.begin() );
//     if ( !splitOnSpace.size() )
//         return ( EMPTY );
//     if ( findChannel( splitOnSpace, channel ) != CONTINUE )
//         return ( NOCHANNEL );
//     if ( checkRight( channel, client ) != CONTINUE )
//         return ( NORIGHT );
//     if ( findTarget( splitOnSpace, channel ) != CONTINUE )
//         return ( NOTARGET );
//
//     return ( NONE );
// }
//
// static enum Err
// findChannel( std::vector<std::string> data, const Channel& channel )
// {
//     bool found = false;
//     for ( std::vector<std::string>::iterator it = data.begin();
//                                                       it != data.end(); it++ ) {
//         if ( !(*it).empty() &&
//                 (*it).substr( 1, std::string::npos ) == channel.GetName() ) {
//             found = true ; break ;
//         }
//     }
//     if ( found ) return ( CONTINUE );
//     else         return ( NOCHANNEL );
// }
//
// static enum Err
// findTarget( std::vector<std::string> data, const Channel& channel )
// {
//     std::map<Client*, bool> target( channel.GetUser() );
//     bool found = false;
//
//     for ( std::vector<std::string>::iterator itData = data.begin();
//                                               itData != data.end(); itData++ ) {
//         for ( std::map<Client*, bool>::iterator itTarget = target.begin();
//                                         itTarget != target.end(); itTarget++ ) {
//             if ( !itData->empty() &&
//                     itTarget->first->GetNickname() == *itData ) {
//                 found = true ; break ;
//             }
//         }
//     }
//     if ( found ) return ( CONTINUE );
//     else         return ( NOTARGET );
// }
//
// static enum Err
// checkRight( const Channel& channel, Client& client )
// {
//     std::map<Client*, bool> target( channel.GetUser() );
//     if ( target.count( &client ) != 0 && target[&client] == true )
//         return ( CONTINUE );
//     else
//         return ( NORIGHT );
// }
