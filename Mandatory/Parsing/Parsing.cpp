#include "Tools.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"

#include <cctype>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

typedef enum e_value {
     TOPIC,
     INVITE,
     PRIVMSG,
     KICK,
     JOIN,
     MODE,
	 WHO,
     CAP,
     sPASS,
     sNICK,
     sUSER,
     NC,
     sCLIENT, 
     NOLOGIN,
     MOREPARAMS
} e_value;


// ########################################################################## //
// #~TODO MANAGER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# //
// Still to make :
// -> Management of NickName
// Error found :
//      SEGFAULT on empty data after all keyword
// ########################################################################## //

// ########################################################################## //
// #~TESTING TOOL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# //
// static void
// printClient( const Client* person );
// static void
// print( std::string in, std::vector<std::string>& tab );

// TEST :
// PASS password
// NICK jimmy
// USER tdc
// ########################################################################## //

static void
dispatch( std::string& info, int& way, Client& person, Server& server );
static void
ncCreation( const std::vector<std::string>& info, Client& person, Server& server );
static void
userCreation( const std::vector<std::string>& info, Client& person, Server& server );
static int
wayChooser( const std::string& target );
static void
loadUserData( Client& person, Server& server, const std::string& data, int word );
static bool
isValidNickName( Server& server, std::string key );
static void
parsingReaply( Client& person, int flag );

// ########################################################################## //
// #_PARSER COMMAND_________________________________________________________# //
int
handleCommand(Server& server, Client& person ) {
    std::string                 work(person.GetInputBuffer());
    std::vector<std::string>    tab;
    std::vector<std::string>    line;
    int                         way = -1;

    // TODO need a APPEND with previous cmd maybe manage here
	//
    tab = split( work, "\r\n" );
	std::cout << "Cmd *-" << work << "-*" << std::endl;
    if ( tab.empty() )
        return ( way ) ;
    for ( size_t i = 0; tab.size() != 0  && i < tab.size(); i++ ) {
        line = split(  tab[i], " " );
        if ( line.size() != 0 )
            way = wayChooser( line[0] );
        if ( way == sCLIENT )
            break ;
        else if ( way != -1 && line.size() > 1 ) {
             dispatch( tab[i], way, person, server );
             person.ClearInputBuffer();
             break ;
        }
        else
            parsingReaply( person, MOREPARAMS );
        line.clear();                                                    // i need to split cmd one by one
    }
    if ( tab.size() == 1 && tab[0].find( "CAP" ) == std::string::npos ) {
        ncCreation( tab, person, server );
        person.ClearInputBuffer();
    }
    else {
        userCreation( tab, person, server );
        person.ClearInputBuffer();
    }
	std::cout << "Reply\n*-" << person.GetMessage() << "-*" << std::endl;
    return ( way );
}

static void
dispatch( std::string& info, int& way, Client& person, Server& server ) {
    if ( !person.GetStatement() )
		way = -1 ;
    switch ( way ) {
        case TOPIC :
            topic( server, person, info );
            break ;
        case INVITE :
            break ;
        case PRIVMSG :
            privateMessage( server, person, info );
            break ;
        case KICK :
            kick ( server, person, info );
            break ;
        case JOIN :
            Join( server, person, info );
            break ;
        case MODE :
            Mode( server, person, info );
            break ;
		case WHO :
            Who( server, person, info );
            break ;
        default :
            parsingReaply( person, NOLOGIN );
            return ;
    }
}

// ########################################################################## //
// #_CLIENT GESTION_________________________________________________________# //
static void
ncCreation( const std::vector<std::string>& info, Client& person, Server& server ) {
    std::vector<std::string>    toParse;
    std::vector<std::string>    word;
    size_t                      check = 0;

    // if ( info.empty() ) {
    //     return ;
    // }

    toParse = info;
    if ( !person.GetStatementStep( PASSWD ) ) {
        for ( size_t i = 0; i < toParse.size(); i++, check++ ) {
            if ( toParse[i].find( "PASS" ) != std::string::npos ) {
                loadUserData( person, server, toParse[i], sPASS );
                return ;
            }
        }
        if ( check == toParse.size() )
            parsingReaply( person, sPASS );
        return ;
    }
    for ( size_t i = 0; i < toParse.size() && person.GetStatementStep( PASSWD ); i++ ) {
        if ( toParse[i].find( "NICK" ) != std::string::npos ) {
            loadUserData( person, server, toParse[i], sNICK );
            return ;
        }                                                                         // TODO need test
        else if ( toParse[i].find( "USER" ) != std::string::npos && \
                person.GetStatementStep( NICK ) ) {
            loadUserData( person, server, toParse[i], sUSER );
            return ;
        }
        else if ( !person.GetStatementStep( NICK ) ) {
            parsingReaply( person, sNICK );
            return ;
        }
    }

    if ( person.GetStatementStep( PASSWD ) && person.GetStatementStep( NICK ) \
        && !person.GetStatementStep( USER ) )
        parsingReaply( person, sUSER );
}

static void
userCreation( const std::vector<std::string>& info, Client& person, Server& server ) {
    std::vector<std::string>    toParse;
    std::vector<std::string>    word;

    if ( info.empty() ) {
        return ;
    }

    toParse = info;
    for ( size_t i = 0; i < toParse.size(); i++ )
    {
        if ( toParse[i].find( "PASS" ) != std::string::npos )
            loadUserData( person, server, toParse[i], sPASS );

        else if ( toParse[i].find( "NICK" ) != std::string::npos )
            loadUserData( person, server, toParse[i], sNICK );

        else if ( toParse[i].find( "USER" ) != std::string::npos )
            loadUserData( person, server, toParse[i], sUSER );
    }
}

static void
loadUserData( Client& person, Server& server, const std::string& data, int word ) {
    std::string::size_type      it;
    std::string                 toWork( data );
    std::string                 buffer;
    std::vector<std::string>    tab;

    if ( word == sNICK ) {
        it = toWork.find( "NICK" );
        buffer = toWork.substr( it + 5, std::string::npos );
        if ( isValidNickName(server, buffer) )
            person.SetNickname( buffer );
        else if ( person.GetNickname().empty() )
            parsingReaply( person, sNICK );
        buffer.clear();
    }
    else if ( word == sPASS ) {
        it = toWork.find( "PASS" );
        buffer = toWork.substr( it + 5, std::string::npos );
        if ( server.GetPassword() == buffer )
            person.SetPasswd();
        else
            parsingReaply( person, sPASS );
        buffer.clear();
    }
    else if ( word == sUSER ) {
        tab = split( data, " " );
        if ( tab.size() > 1 ) 
            person.SetUsername( tab[1] );
    }
}

static bool
isValidNickName( Server& server, std::string key )
{
    if ( key.length() > 8 )
        return ( false );

    for( size_t i = 0; i < key.length(); i++) {
        if ( !std::isalnum( key[i] ) )
            return ( false );
    }

    if ( server.GetClient( key ) )
        return ( false );

    return ( true );
}

static int
wayChooser( const std::string& target ) {
    std::string cmd[7] = { "TOPIC", "INVITE", "PRIVMSG", "KICK", "JOIN", "MODE", "WHO" };
    std::string client[4] = { "CAP", "PASS", "NICK", "USER" };

    for ( int i = 0; i < 7; i++ ) {
        if ( cmd[i] == target ) {
            return ( i );
        }
    }
    for ( int i = 0; i < 4; i++ ) {
        if ( client[i] == target ) {
            return ( sCLIENT );
        }
    }
    return ( sCLIENT );
}

static void
parsingReaply( Client& person, int flag )
{
	std::string	reply;

    if ( flag  == sPASS )
        reply = "Require password.\r\n";

    else if ( flag == sUSER )
        reply = "Require username.\r\n";

    else if ( flag == sNICK )
        reply = "Require nickname.\r\n";

    else if ( flag == NOLOGIN )
        reply = ": 444 :LOGIN error no login.\r\n";

    else if ( flag == MOREPARAMS )
        reply = "More params requiert\r\n";

    else
        reply = "";

    person.SetMessageToSend( reply );
}
