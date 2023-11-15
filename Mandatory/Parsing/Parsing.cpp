#include "Tools.hpp"
#include "Client.hpp"
#include "Server.hpp"

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
// printClient( const Client* person );
// static void
// print( std::string in, std::vector<std::string>& tab );

// static void
// dispatch( const std::string& info, const int& way, Client& person, const Server& server );
static void
userCreation( const std::vector<std::string>& info, Client& person, const Server& server );
static int
wayChooser( const std::string& target );
static void
loadUserData( Client& person, const Server& server, const std::string& data, int word );

void
handleCommand( const char* buffer, const Server& server, Client& person ) {
    std::string                 work;
    std::vector<std::string>    tab;
    std::vector<std::string>    line;
    int                         way = -1;


    if ( !buffer || !*buffer )
        return ;
    // TODO need a APPEND with previous cmd maybe manage here
    person.SetInputBuffer( buffer );
    work = person.GetInputBuffer();
    tab = split( work, "\r\n" );
    if ( tab.empty() )
        return ;
    for ( size_t i = 0; tab.size() != 0  && i < tab.size(); i++ ) {
        line = split(  tab[i], " " );
        if ( line.size() != 0 )
            way = wayChooser( line[0] );
        if ( way != CLIENT )
            break ;
        // else if ( way != -1 )                                                    // TODO
        //     dispatch( tab[i], way, person, server );                     // need more accuratie
        line.clear();                                                    // i need to split cmd one by one
    }
    if ( way == CLIENT )
        userCreation( tab, person, server );
    return ;
}

// static void
// dispatch( const std::string& info, const int& way, Client& person, const Server& server ) {
//     switch ( way ) {
//         // case NC :
//         // case TOPIC :
//         // case INVITE :
//         // case PRIVMSG :
//         // case KICK :
//         // case JOIN :
//         // case MODE :
//         default :
//             return ;
//     }
// }

static void
userCreation( const std::vector<std::string>& info, Client& person, const Server& server ) {
    std::vector<std::string>    toParse;
    std::vector<std::string>    word;

    if ( info.empty() ) {
        return ;
    }

    toParse = info;
    // print( "userCreation", toParse );                                         // TODO DEBUG
    // std::cout << "HERE MARK" << std::endl;
    for ( size_t i = 0; i < toParse.size(); i++ ) {
        if ( toParse[i].find( "PASS" ) != std::string::npos )
            loadUserData( person, server, toParse[i], PASS );
        else if ( toParse[i].find( "NICK" ) != std::string::npos )               // TODO need test
            loadUserData( person, server, toParse[i], NICK );
        else if ( toParse[i].find( "USER" ) != std::string::npos )
            loadUserData( person, server, toParse[i], USER );
        // else
                                                                                 // TODO error;
    }
}

static void
loadUserData( Client& person, const Server& server, const std::string& data, int word ) {
    std::string::size_type      it;
    std::string                 toWork( data );
    std::string                 buffer;
    std::vector<std::string>    tab;

    if ( word == NICK ) {
        it = toWork.find( "NICK" );
        buffer = toWork.substr( it + 5, std::string::npos );
        if ( buffer.size() < 9 )
            person.SetNickname( buffer );
        else
            person.SetNickname( "ERROR" ); // TODO ERROR gestion need better gestion
        buffer.clear();
    }
    else if ( word == PASS ) {
        it = toWork.find( "PASS" );
        buffer = toWork.substr( it + 5, std::string::npos );
        if ( server.GetPassword() == buffer )
            person.SetPasswd();
        buffer.clear();
    }
    else if ( word == USER ) {
        tab = split( data, " " );
        person.SetUsername( tab[1] );
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

// static void
// printClient( const Client& person ) {
//     std::cout << "NickName: " << person->GetNickname() << std::endl;
//     std::cout << "User: " << person->GetUsername() << std::endl;
// }

/*
 * Parser et creer le client 
*/
