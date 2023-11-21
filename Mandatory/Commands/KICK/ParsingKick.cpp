#include "Kick.hpp"

static enum Err
checkRight( const Channel& channel, Client& client );
static enum Err
findChannel( std::vector<std::string> data, const Channel& channel );
static enum Err
findTarget( std::vector<std::string> data, const Channel& channel );


// ########################################################################## //
// #_PARSER_________________________________________________________________# //

enum Err
parseCmd( const std::string& cmd, const Channel& channel, Client& client )
{
    std::vector<std::string> splitOnSpace;

    splitOnSpace = split( cmd , " " );
    splitOnSpace.erase( splitOnSpace.begin() );
    if ( !splitOnSpace.size() )
        return ( EMPTY );
    if ( findChannel( splitOnSpace, channel ) != CONTINUE )
        return ( NOCHANNEL );
    if ( checkRight( channel, client ) != CONTINUE )
        return ( NORIGHT );
    if ( findTarget( splitOnSpace, channel ) != CONTINUE )
        return ( NOTARGET );

    return ( NONE );
}

// ########################################################################## //
// #_TOOLS__________________________________________________________________# //

static enum Err
findChannel( std::vector<std::string> data, const Channel& channel )
{
    bool found = false;
    for ( std::vector<std::string>::iterator it = data.begin();
                                                      it != data.end(); it++ ) {
        if ( !(*it).empty() &&
                (*it).substr( 1, std::string::npos ) == channel.GetName() ) {
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
checkRight( const Channel& channel, Client& client )
{
    std::map<Client*, bool> target( channel.GetUser() );
    if ( target.count( &client ) != 0 && target[&client] == true )
        return ( CONTINUE );
    else
        return ( NORIGHT );
}
