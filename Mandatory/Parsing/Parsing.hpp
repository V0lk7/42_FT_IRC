#ifndef PARSING_HPP
# define PARSING_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include "Client.hpp"
# include "Server.hpp"

void
handleCommand( const char* buffer, const Server& server, Client& person );
void
test( Client& person );

#endif
