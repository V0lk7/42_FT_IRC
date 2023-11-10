#include "Tools.hpp"
#include "Client.hpp"

#include <iostream>
#include <sstream>
#include <vector>

/*
 * Trying to handle parsing
*/

// static void print( std::vector<std::string>& tab );
static int  displaying( const std::string& target );
static void stashInfo( Client* person, const std::vector<std::string>& input );

void    handleCommand( const char* buffer ) {
    std::vector<std::string>    tab;
    std::vector<std::string>    line;
    int                         way = -1;

    // TODO need a APPEND with previous cmd
    // std::cout << "Handle" << std::endl;
    if ( !buffer )
        return ;
    std::string work(buffer);
    tab = split(  work, "\r\n" );
    // std::cout << "TEST" << std::endl;
    // print( tab );
    for ( size_t i = 0; tab.size() != 0  && i < tab.size(); i++ ) {
        line = split(  tab[i], " " );
        std::cout << "\nWord" << std::endl;
        // TODO displaying cmd;
        if ( line.size() != 0 )
            way = displaying( line[0] );
        if ( way != -1 )
            break;
        line.clear();
    }
}

static Client* userCreation( const std::vector<std::string>& info ) {
    std::vector<std::string> toParse;
    std::vector<std::string> word;
    Client* person = new Client();

    if ( info.empty() )
        return ;
    toParse = info;
    for ( size_t i = 1; i < toParse.size(); i++ ) {
        if ( toParse[i].find( "PASS" ) )

        else if ( toParse[i].find( "NICK" ) ) // TODO need test
        else if ( toParse[i].find( "USER" ) )
        else
            // TODO error;
    }
    // TODO checkUSER;
}

static int  displaying( const std::string& target ) {
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

static void stashInfo( Client* person, const std::vector<std::string>& input ) {
    std::vector<std::string>    info;

    if ( input.empty() )
        return ;
    info = input;
    // if ( info[0] == "PASS" ) {
    //     // NEED MORE CHECK
    //     person->SetPasswd();
    // }
    if ( info[0] == "NICK" ) {
        if ( )
    }
}

static void print( std::vector<std::string>& tab ) {
    int i = 0;
    for (std:::x
            vector<std::string>::iterator it = tab.begin(); it < tab.end(); it++ )
        std::cout << "tab[" << i++ << "]: " << *it << std::endl;
}

/*
 * Parser et creer le client 
*/
