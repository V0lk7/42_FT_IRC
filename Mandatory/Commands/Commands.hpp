#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <string>

class Server;
class Client;

void	Join(Server &server, Client &client, std::string &RawCmd);

#endif
