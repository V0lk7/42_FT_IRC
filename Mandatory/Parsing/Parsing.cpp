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
#define NC      7

/*
 * Trying to handle parsing
*/

static void
print( std::string in, std::vector<std::string>& tab );
static void
dispatch( const std::vector<std::string>& info, const int& way, Client* person );
static void
printClient( const Client* person );
static void
userCreation( const std::vector<std::string>& info, Client* person );
static int
wayChooser( const std::string& target );
static void
loadUserData( Client* person, const std::string& data, int word );
// static void
// stashInfo( Client* person, const std::vector<std::string>& input );

void
handleCommand( const char* buffer ) {
    std::string                 work;
    std::vector<std::string>    tab;
    std::vector<std::string>    line;
    int                         way = -1;
    Client*                     person = NULL;

                                                                                 // TODO need a APPEND with previous cmd
                                                                                 // TODO std::cout << "Handle" << std::endl;
    if ( !buffer )
        return ;
    work = buffer;
    tab = split(  work, "\r\n" );
    print( "handleCommand", tab );                                               // TODO testing
    for ( size_t i = 0; tab.size() != 0  && i < tab.size(); i++ ) {
        line = split(  tab[i], " " );
        if ( line.size() != 0 )
            way = wayChooser( line[0] );
        if ( way != -1 )
            break;
        line.clear();
    }
    // TODO wayChoosering cmd;
    dispatch( tab, way, person );
    if ( person )
        printClient( person );
}

static void
dispatch( const std::vector<std::string>& info, const int& way, Client* person ) {
    std::vector<std::string> cpy(info);
    print( "dispatch", cpy );                                                    // TODO testing
    switch ( way ) {
        // case NC :
        // case TOPIC :
        // case INVITE :
        // case PRIVMSG :
        // case KICK :
        // case JOIN :
        // case MODE :
        case CAP :
            std::cout << " ICI bo goss " << std::endl;
            userCreation( info , person );
            break ;
        default :
            return ;
    }
}

static void
userCreation( const std::vector<std::string>& info, Client* person ) {
    std::vector<std::string>    toParse;
    std::vector<std::string>    word;

    if ( info.empty() )
        return ;

    person = new Client();
    toParse = info;

    print( "userCreation", toParse );                                            // TODO DEBUG
    for ( size_t i = 1; i < toParse.size(); i++ ) {
        if ( toParse[i].find( "PASS" ) != std::string::npos )
            loadUserData( person, toParse[i], PASS );
        else if ( toParse[i].find( "NICK" ) != std::string::npos )               // TODO need test
            loadUserData( person, toParse[i], NICK );
        else if ( toParse[i].find( "USER" ) != std::string::npos )
            loadUserData( person, toParse[i], USER );
        // else
                                                                                 // TODO error;
    }
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
        person->SetUsername( tab[1] );
    }
}

static int
wayChooser( const std::string& target ) {
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
                                // TODO CREATION USER NC NEEDING
    return ( NC );
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
print( std::string in, std::vector<std::string>& tab ) {
    int i = 0;
    std::cout << in << ":" << std::endl;
    for ( std::vector<std::string>::iterator it = tab.begin(); it < tab.end(); it++, i++ )
        std::cout << "tab[" << i << "]: " << *it << std::endl;
    std::cout << std::endl;
}

static void
printClient( const Client* person ) {
    std::cout << "NickName: " << person->GetNickname() << std::endl;
    std::cout << "User: " << person->GetUsername() << std::endl;
}

/*
 * Parser et creer le client 
*/
