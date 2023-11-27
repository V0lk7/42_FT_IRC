#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>

class Server;

std::string	IntToString(int n);
Client *SetupCli(std::string Name);
void	Setup(Server &serv, int ChanNbr, int CliNbr);

#endif
