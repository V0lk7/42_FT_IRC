#include "Tools.hpp"

#include <iostream>
#include <sstream>
#include <vector>

/*
 * Trying to handle parsing
*/

// static void print( std::vector<std::string>& tab );
static int  displaying( const std::string& target );

void    handleCommand( char* buffer ) {

    if ( !buffer )
        return ;
    std::string                 work(buffer);
    std::vector<std::string>    tab;
    std::vector<std::string>    line;

    // TODO need a APPEND with previous cmd
    // std::cout << "Handle" << std::endl;
    tab = split(  work, "\r\n" );
    // print( tab );
    for ( size_t i = 0; tab.size() != 0  && i < tab.size(); i++ ) {
        line = split(  tab[i], " " );
        std::cout << "\nWord" << std::endl;
        // TODO displaying cmd;
        if ( line.size() != 0 )
            std::cout << displaying( line[0] ) << std::endl;
        line.clear();
    }
}

static int  displaying( const std::string& target ) {

    std::string comp[9] = {
        "TOPIC", "INVITE", "PRIVMSG",
        "KICK", "JOIN", "MODE",
        "USER", "NICK", "PASS"
    };

    for ( int i = 0; i < 9; i++ ) {
        if ( comp[i] == target ) {
            std::cout << comp[i] << std::endl;
            return ( i );
        }
    }
    return ( -1 );
}

// static void print( std::vector<std::string>& tab ) {
//     int i = 0;
//     for (std::vector<std::string>::iterator it = tab.begin(); it < tab.end(); it++ )
//         std::cout << "tab[" << i++ << "]: " << *it << std::endl;
// }

/*
 * Parser et creer le client 
*/
