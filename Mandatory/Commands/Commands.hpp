#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <string>

class Server;
class Client;

void    Join(Server &server, Client &client, std::string &RawCmd);

void
kick( const Server& server, Client& client, const std::string& cmd );

void
topic( const Server& server, Client& client, const std::string& cmd );

void
invite( Server& server, Client& client, const std::string& cmd );

#endif
