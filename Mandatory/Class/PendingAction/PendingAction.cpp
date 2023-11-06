#include "PendingAction.hpp"
#include <iostream>
#include <sstream>
#include <vector>

/*
 * Trying to handle parsing
*/

static void print( std::vector<std::string>& tab );

void    handleCommand( char* buffer ) {
    std::string                 work = buffer;
    std::vector<std::string>    tab;
    std::istringstream          InputStream(work);

    while (InputStream)
    {
        std::getline(InputStream, work, '\n');
        if (InputStream.good() == false || InputStream.eof() == true)
          break ;
        tab.push_back(work);
    }
    print( tab );
}

static void print( std::vector<std::string>& tab ) {
    int i = 0;
    for (std::vector<std::string>::iterator it = tab.begin(); it < tab.end(); it++ )
        std::cout << "tab[" << i++ << "]: " << *it << std::endl;
}

/*
 * Parser et creer le client 
*/
