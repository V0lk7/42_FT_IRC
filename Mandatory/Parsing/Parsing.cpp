#include "Tools.hpp"

#include <iostream>
#include <sstream>
#include <vector>

/*
 * Trying to handle parsing
*/

// static void print( std::vector<std::string>& tab );
static int  displaying( const std::string& target );

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

static void userCreation( const std::vector<std::string>& info ) {
    std::vector<std::string> toParse;
    std::vector<std::string> word;
    // TODO create user

    if ( info.empty() )
        return ;
    toParse = info;
    for ( size_t i = 0; i < toParse.size(); i++ ) {
        word = split( toParse[i], " " );
        if ( word[0] == "PASS" )
            
        else if ( word[0] == "NICK" )
        else if ( word[0] == "USER" )
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

static void stashInfo( const std::string& input ) {
    std::string                 str(input);
    std::vector<std::string>    info;

    info = split( str, " " );
}

// static void print( std::vector<std::string>& tab ) {
//     int i = 0;
//     for (std::vector<std::string>::iterator it = tab.begin(); it < tab.end(); it++ )
//         std::cout << "tab[" << i++ << "]: " << *it << std::endl;
// }

/*
 * Parser et creer le client 
*/
