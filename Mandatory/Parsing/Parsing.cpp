#include "Tools.hpp"
#include "Client.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>

#define NICK 10
#define PASS 11
#define USER 21

#define TOPIC   0 
#define INVITE  1
#define PRIVMSG 2
#define KICK    3
#define JOIN    4
#define MODE    5
#define CAP     6

/*
 * Trying to handle parsing
*/

static void
print( std::vector<std::string>& tab );
static void
printClient( const Client* person );
static Client*
userCreation( const std::vector<std::string>& info );
static int
displaying( const std::string& target );
static void
loadUserData( Client* person, const std::string& data, int word );
// static void
// stashInfo( Client* person, const std::vector<std::string>& input );

void
handleCommand( const char* buffer ) {
    std::vector<std::string>    tab;
    std::vector<std::string>    line;
    int                         way = -1;
    Client*                     person;

    // TODO need a APPEND with previous cmd
    // std::cout << "Handle" << std::endl;
    if ( !buffer )
        return ;
    std::string work(buffer);
    tab = split(  work, "\r\n" );
    // std::cout << "TEST" << std::endl;
    print( tab );
    for ( size_t i = 0; tab.size() != 0  && i < tab.size(); i++ ) {
        line = split(  tab[i], " " );
        // TODO displaying cmd;
        if ( line.size() != 0 )
            way = displaying( line[0] );
        if ( way != -1 )
            break;
        line.clear();
    }
    if ( way == CAP ) 
        std::cout << "yes" << std::endl;
    person = userCreation( tab );
    if ( person )
        printClient( person );
}

static Client*
userCreation( const std::vector<std::string>& info ) {
    std::vector<std::string>    toParse;
    std::vector<std::string>    word;
    Client*                     person;

    if ( info.empty() )
        return ( NULL ) ;

    person = new Client();
    toParse = info;

    // print( toParse );                                                         // TODO DEBUG
    for ( size_t i = 1; i < toParse.size(); i++ ) {
        if ( toParse[i].find( "PASS" ) != std::string::npos )
            loadUserData( person, toParse[i], PASS );
        else if ( toParse[i].find( "NICK" ) != std::string::npos ) // TODO need test
            loadUserData( person, toParse[i], NICK );
        else if ( toParse[i].find( "USER" ) != std::string::npos )
            loadUserData( person, toParse[i], USER );
        // else
            // TODO error;
    }
    // TODO checkUSER;
    return ( person );
}

static void
loadUserData( Client* person, const std::string& data, int word ) {
    std::string::size_type      it;
    std::string                 toWork( data );
    std::string                 buffer;
    std::vector<std::string>    tab;

    if ( word == NICK ) {
        it = toWork.find( "NICK" );
        buffer = toWork.substr( it + 4, std::string::npos );
        std::cout << "NICK: " << buffer << std::endl;
        if ( buffer.size() < 9 )
            person->SetNickname( buffer );
        buffer.clear();
    }
    else if ( word == PASS ) {
        it = toWork.find( "PASS" );
        buffer = toWork.substr( it + 4, std::string::npos );
        // TODO verfied PASSWORD in server and buffer
        person->SetPasswd();
        buffer.clear();
    }
    else if ( word == USER ) {
        tab = split( data, " " );
        std::cout << tab[1] << std::endl;
        person->SetUsername( tab[1] );
    }
}

static int
displaying( const std::string& target ) {
    std::string comp[7] = {
        "TOPIC", "INVITE", "PRIVMSG",
        "KICK", "JOIN", "MODE",
        "CAP"
    };
    for ( int i = 0; i < 9; i++ ) {
        if ( comp[i] == target ) {
            std::cout << comp[i] << std::endl;
            return ( i );
        }
    }
    // CREATION USER NC NEEDING
    return ( -1 );
}

// static void
// stashInfo( Client* person, const std::vector<std::string>& input ) {
//     std::vector<std::string>    info;
//
//     if ( input.empty() )
//         return ;
//     info = input;
//     // if ( info[0] == "PASS" ) {
//     //     // NEED MORE CHECK
//     //     person->SetPasswd();
//     // }
//     if ( info[0] == "NICK" ) {
//         if ( )
//     }
// }


// ########################################################################## //
// #~TESTING TOOL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# //

static void
print( std::vector<std::string>& tab ) {
    int i = 0;
    for ( std::vector<std::string>::iterator it = tab.begin(); it < tab.end(); it++ )
        std::cout << "tab[" << i++ << "]: " << *it << std::endl;
}

static void
printClient( const Client* person ) {
    std::cout << "NickName: " << person->GetNickname() << std::endl;
    std::cout << "User: " << person->GetUsername() << std::endl;
}

/*
 * Parser et creer le client 
*/
