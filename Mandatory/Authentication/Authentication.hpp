#ifndef AUTHENTICATION_HPP
# define AUTHENTICATION_HPP

class Server;
class Client;

# include <string>

void	Pass(Server &server, Client &client, std::string &Auth);
void	Nick(Server &server, Client &client, std::string &Auth);
void	User(Client &client, std::string &Auth);

#endif
