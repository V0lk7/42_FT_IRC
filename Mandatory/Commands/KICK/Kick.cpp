#include "Tools.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#include "Kick.hpp"

// ########################################################################## //
// #_TODO___________________________________________________________________# //
// #-> better handling error msg: 1.                                        # //
// #-> check idx 4 not sur i guess it would be 3                            # //
// #-> i need a precise explanation of string storage 'Channel'             # //
// #   Does it store '#' or '&' inside itself.                               # //
// ########################################################################## //

static std::string
msgMaker( Client& client, Channel& channel, std::vector<std::string>& data );
static void
rmClientOfChannel( Channel& channel, const std::string& key,
                                                    const std::string& reason );
void
Kick( const Server& server, Client& client, const std::string& cmd )
{
    std::string                 reason;
    Channel*                    channel;

    std::vector<std::string>    data = split( cmd, " " );
    data.erase( data.begin() );

    channel = server.GetChannel( data[0] );

    if ( parseCmd( cmd , channel, client ) != NONE )
        return ;

    reason = msgMaker( client, *channel, data );
    rmClientOfChannel( *channel, data[1], reason );
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

        it->first->SetMessageToSend( reason );
        if ( it->first->GetNickname() == key )
            channel.EraseClientFromChannel( *it->first );
    }
}
// TODO TESTING here
static std::string
msgMaker( Client& client, Channel& channel, std::vector<std::string>& data )
{
//    std::cout << "\tTEST ICI PUTAIN\n"; // TODO 
    std::string msg;
    if ( data.size() <= 2 ) {
        msg = ":" + client.GetNickname() + " KICK "
            + channel.GetName() + " " + data[1] + "\r\n";
    }
    else
    {
//        std::cout << "\tTEST\n"; // TODO 
        msg = ":" + client.GetNickname() + " KICK "
            + channel.GetName() + " " + data[1] + " :";

        for ( size_t idx = 4; idx < data.size(); idx++ )                         // TODO not sur about
            msg += " " + data[idx];                                              // idx = 4

  //      std::cout << "msg: " << msg;
        msg += "\r\n";
    }
    return ( msg );
}
