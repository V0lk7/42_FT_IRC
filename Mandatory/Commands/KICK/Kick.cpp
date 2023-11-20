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
// ########################################################################## //

enum Err { NONE, EMPTY, NOTARGET, CONTINUE, NOCHANNEL };

static enum Err
findChannel( std::vector<std::string> data, const Channel& channel );
static enum Err
findTarget( std::vector<std::string> data, const Channel& channel );
static enum Err
parseCmd( const std::string& cmd, const Channel& channel );

void
kick( const Server& server, const Client& client, Channel& channel,
                                                        const std::string& cmd )
{
    (void)server;
    (void)client;
    std::string str(cmd);
    std::string msg;
    std::vector<std::string> words;


    if ( parseCmd( cmd , channel ) != NONE ) {
        std::cerr << "\t*NEED MSG HANDLING*" << std::endl;                       // TODO 1.
        return ;
    }

}

// ########################################################################## //
// #_PARSER_________________________________________________________________# //

static enum Err
parseCmd( const std::string& cmd, const Channel& channel )
{
    std::vector<std::string> splitOnSpace;

    splitOnSpace = split( cmd , " " );
    splitOnSpace.erase( splitOnSpace.begin() );
    if ( !splitOnSpace.size() )
        return ( EMPTY );
    if ( findChannel( splitOnSpace, channel ) == NOCHANNEL )
        return ( NOCHANNEL );
    if ( findTarget( splitOnSpace, channel ) == NOTARGET )
        return ( NOTARGET );

    return ( NONE );
}

static enum Err
findChannel( std::vector<std::string> data, const Channel& channel )
{
    bool found = false;
    for ( std::vector<std::string>::iterator it = data.begin();
                                                      it != data.end(); it++ ) {
        if ( *(++it) == channel.GetName() ) {
            found = true ; break ;
        }
    }
    if ( found ) return ( CONTINUE );
    else         return ( NOCHANNEL );
}

static enum Err
findTarget( std::vector<std::string> data, const Channel& channel )
{
    std::map<Client*, bool> target( channel.GetUser() );
    bool found = false;

    for ( std::vector<std::string>::iterator itData = data.begin();
                                              itData != data.end(); itData++ ) {
        for ( std::map<Client*, bool>::iterator itTarget = target.begin();
                                        itTarget != target.end(); itTarget++ ) {
            if ( itTarget->first->GetUsername() == *itData ) {
                found = true ; break ;
            }
        }
    }
    if ( found ) return ( CONTINUE );
    else         return ( NOTARGET );
}
