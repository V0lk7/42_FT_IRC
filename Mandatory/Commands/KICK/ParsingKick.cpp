#include "Kick.hpp"

// ########################################################################## //
// #_TODO___________________________________________________________________# //
// #-> better handling error msg: 1.                                        # //
// #-> need check if [&#] is present TODO                                   # //
// #-> need to implement checking right of client kickmaker                 # //
// #-> need str is : KICK [&#]CHANNEL target TODO MISSING FORMAT CHECKING   # //
// #               : MISSING MULTIPLE KICKING FORMAT AND HAVE TO CHANNEL    # //
// #               : AFTER [#&]                                             # //
// ########################################################################## //

static enum Err
checkRight( const Channel& channel, Client& client );
static enum Err
findChannel( std::vector<std::string>& data, const Channel& channel );
static enum Err
findTarget( std::vector<std::string>& data, const Channel& channel );

// ########################################################################## //
// #_PARSER_________________________________________________________________# //

enum Err
parseCmd( const std::string& cmd, const Channel& channel, Client& client )
{
    std::vector<std::string> splitOnSpace = split( cmd , " " );

    splitOnSpace.erase( splitOnSpace.begin() );
    if ( !splitOnSpace.size() )
        return ( EMPTY );

    else if ( findChannel( splitOnSpace, channel ) != CONTINUE )
        return ( NOCHANNEL );

    else if ( checkRight( channel, client ) != CONTINUE )
        return ( NORIGHT );

    else if ( findTarget( splitOnSpace, channel ) != CONTINUE )
        return ( NOTARGET );

    return ( NONE );
}

// ########################################################################## //
// #_TOOLS__________________________________________________________________# //

static enum Err
findChannel( std::vector<std::string>& data, const Channel& channel )
{
    bool    found = false;
    size_t  pos = 0;
    for ( std::vector<std::string>::iterator it = data.begin();
                                               it != data.end(); it++, pos++ ) {
        if ( !pos && !(*it).empty() && ( (*it)[0] == '#' ||(*it)[0] == '&' ) &&
                (*it).substr( 1, std::string::npos ) == channel.GetName() ) {
            found = true ; break ;
        }
    }
    return ( found ? CONTINUE : NOCHANNEL );
}

static enum Err
findTarget( std::vector<std::string>& data, const Channel& channel )
{
    std::map<Client*, bool> target( channel.GetUser() );
    bool found = false;
    size_t  pos = 0;

    for ( std::vector<std::string>::iterator itData = data.begin();
                                       itData != data.end(); itData++, pos++ ) {
        for ( std::map<Client*, bool>::iterator itTarget = target.begin();
                                        itTarget != target.end(); itTarget++ ) {
            if ( !itData->empty() && pos == 1 &&
                    itTarget->first->GetNickname() == *itData ) {
                found = true ; break ;
            }
        }
    }
    return ( found ? CONTINUE : NOTARGET );
}

static enum Err
checkRight( const Channel& channel, Client& client )
{
    std::map<Client*, bool> target( channel.GetUser() );
    if ( target.count( &client ) != 0 && target[&client] == true )
        return ( CONTINUE );
    else
        return ( NORIGHT );
}