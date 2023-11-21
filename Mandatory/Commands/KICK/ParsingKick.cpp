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
checkRight( const Channel& channel, const Client& client );
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
    bool found = false;
    for ( std::vector<std::string>::iterator it = data.begin();
                                                      it != data.end(); it++ ) {
        if ( !(*it).empty() && ( (*it)[0] == '#' ||(*it)[0] == '&' ) &&
                (*it).substr( 1, std::string::npos ) == channel.GetName() ) {
            found = true ; break ;
        }
    }
    if ( found ) return ( CONTINUE );
    else         return ( NOCHANNEL );
}

static enum Err
findTarget( std::vector<std::string>& data, const Channel& channel )
{
    std::map<Client*, bool> target( channel.GetUser() );
    bool found = false;

    for ( std::vector<std::string>::iterator itData = data.begin();
                                              itData != data.end(); itData++ ) {
        for ( std::map<Client*, bool>::iterator itTarget = target.begin();
                                        itTarget != target.end(); itTarget++ ) {
            if ( !itData->empty() &&
                    itTarget->first->GetNickname() == *itData ) {
                found = true ; break ;
            }
        }
    }
    if ( found ) return ( CONTINUE );
    else         return ( NOTARGET );
}

static enum Err
checkRight( const Channel& channel, const Client& client )
{
    Client asRight( client );

    std::map<Client*, bool> target( channel.GetUser() );
    if ( target.count( &asRight ) != 0 && target[&asRight] == true )
        return ( CONTINUE );
    else
        return ( NORIGHT );
}
