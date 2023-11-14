#include "Tools.hpp"
#include "Client.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>

#define TOPIC   0 
#define INVITE  1
#define PRIVMSG 2
#define KICK    3
#define JOIN    4
#define MODE    5
#define CAP     6
#define PASS    7
#define NICK    8
#define USER    9
#define NC      10
#define CLIENT  11

/*
 * Trying to handle parsing
*/

// static void
// print( std::string in, std::vector<std::string>& tab );
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

Client*
handleCommand( const char* buffer ) {
    std::string                 work;
    std::vector<std::string>    tab;
    std::vector<std::string>    line;
    int                         way = - 1;
    Client*                     person = new Client();

    // TODO need a APPEND with previous cmd
    // TODO std::cout << "Handle" << std::endl;
    if ( !buffer )
        return ( NULL ) ;
    work = buffer;
    tab = split(  work, "\r\n" );
    // print( "handleCommand", tab );                                            // TODO testing
    for ( size_t i = 0; tab.size() != 0  && i < tab.size(); i++ ) {
        line = split(  tab[i], " " );
        if ( line.size() != 0 )
            way = wayChooser( line[0] );
        if ( way != -1 )
            break ;
        line.clear();
    }
    if ( way != -1 )
        dispatch( tab, way, person );
    if ( person->GetStatement() == true ) {
        printClient( person );
        return ( person );
    }
    // else
    //     delete ( person );
    return ( NULL );
}

static void
dispatch( const std::vector<std::string>& info, const int& way, Client* person ) {
    // std::vector<std::string> cpy(info);
    // print( "dispatch", cpy );                                                    // TODO testing
    switch ( way ) {
        // case NC :
        // case TOPIC :
        // case INVITE :
        // case PRIVMSG :
        // case KICK :
        // case JOIN :
        // case MODE :
        case CLIENT :
            userCreation( info , person );
            break;
        default :
            return ;
    }
}

static void
userCreation( const std::vector<std::string>& info, Client* person ) {
    std::vector<std::string>    toParse;
    std::vector<std::string>    word;

    if ( info.empty() ) {
        delete ( person );
        return ;
    }

    toParse = info;
    // print( "userCreation", toParse );                                            // TODO DEBUG
    // std::cout << "HERE MARK" << std::endl;
    for ( size_t i = 0; i < toParse.size(); i++ ) {
        if ( toParse[i].find( "PASS" ) != std::string::npos )
            loadUserData( person, toParse[i], PASS );
        else if ( toParse[i].find( "NICK" ) != std::string::npos )               // TODO need test
            loadUserData( person, toParse[i], NICK );
        else if ( toParse[i].find( "USER" ) != std::string::npos )
            loadUserData( person, toParse[i], USER );
        else
            delete ( person );
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
        else
            person->SetNickname( "ERROR" ); // TODO ERROR gestion need better gestion
        buffer.clear();
    }
    else if ( word == PASS ) {
        it = toWork.find( "PASS" );
        buffer = toWork.substr( it + 4, std::string::npos );
        // verfied PASSWORD in server and buffer                                 // TODO
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
    std::string client[4] = { "CAP", "PASS", "NICK", "USER" };
    std::string cmd[6] = { "TOPIC", "INVITE", "PRIVMSG", "KICK", "JOIN", "MODE" };

    for ( int i = 0; i < 6; i++ ) {
        if ( cmd[i] == target ) {
            return ( i );
        }
    }
    for ( int i = 0; i < 4; i++ ) {
        if ( client[i] == target ) {
            return ( CLIENT );
        }
    }
    return ( NC );
}

// ########################################################################## //
// #~TESTING TOOL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# //

// static void
// print( std::string in, std::vector<std::string>& tab ) {
//     int i = 0;
//     std::cout << in << ":" << std::endl;
//     for ( std::vector<std::string>::iterator it = tab.begin(); it < tab.end(); it++, i++ )
//         std::cout << "tab[" << i << "]: " << *it << std::endl;
//     std::cout << std::endl;
// }

static void
printClient( const Client* person ) {
    std::cout << "NickName: " << person->GetNickname() << std::endl;
    std::cout << "User: " << person->GetUsername() << std::endl;
}

/*
 * Parser et creer le client 
*/
