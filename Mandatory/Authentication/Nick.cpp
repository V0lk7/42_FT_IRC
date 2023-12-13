#include "Server.hpp"
#include "Client.hpp"
#include <string>

static int	NickIsValid(Server &server, std::string Nick);

void	Nick(Server &server, Client &client, std::string &Auth)
{
	if (client.GetStatementStep(PASSWD) == false) {
		client.SetMessageToSend(": 461 NICK :Password needed before\r\n");
		return ;
	}
	if (client.GetStatementStep(NICK) == true) {
		client.SetMessageToSend(": 461 NICK :Nick already set\r\n");
		return ;
	}

	size_t	pos = Auth.find(" ");
	pos = Auth.find_first_not_of(" ", pos);
	if (pos == std::string::npos) {
		client.SetMessageToSend(": 431 NICK :Need more parameters\r\n");
		return ;
	}

	std::string	Nick(Auth.substr(pos));
	int	flag = NickIsValid(server, Nick);
	if (flag == 1)
		client.SetMessageToSend(": 432 NICK :Wrong format nickname\r\n");
	else if (flag == 2)
		client.SetMessageToSend(": 433 NICK :Nickname already in use\r\n");
	else {
		client.SetNickname(Nick);
		client.SetStatement(NICK, true);
		client.SetMessageToSend(": 461 NICK :Nickname set\r\n"); //maybe useless
	}
	return ;
}

static int	NickIsValid(Server &server, std::string Nick)
{
	if (Nick.size() > 9 || Nick.find_first_of(":#& ") != std::string::npos)
		return (1);
	if (server.GetClient(Nick) != NULL)
		return (2);
	return (0);
}
