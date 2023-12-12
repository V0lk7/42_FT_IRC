#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <string>

class Server;
class Client;

void
PrivateMessage( Server& server, Client& client, std::string& rawCommand );

void
Join(Server &server, Client &client, std::string &RawCmd);

void
Kick( const Server& server, Client& client, const std::string& cmd );

void
Topic( const Server& server, Client& client, const std::string& cmd );

void
Invite( Server& server, Client& client, const std::string& cmd );

void
Mode(Server &server, Client &client, std::string &rawcmd);

void
Who(Server &server, Client &client, std::string &rawcmd);

#endif
