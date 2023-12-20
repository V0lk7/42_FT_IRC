#include "Server.hpp"
#include "Client.hpp"
#include <string>

static int	NickIsValid(Server &server, Client &client, std::string Nick);

void	Nick(Server &server, Client &client, std::string &Auth)
{
	std::string	reply;

	if (client.GetStatementStep(PASSWD) == false) {
		reply = ": 400 : Password needed before\r\n";
		client.SetMessageToSend(reply);
		return ;
	}

	size_t	pos = Auth.find(" ");
	pos = Auth.find_first_not_of(" ", pos);
	if (pos == std::string::npos) {
		client.SetMessageToSend(": 431 NICK : No nickname given\r\n");
		return ;
	}

	std::string	Nick(Auth.substr(pos));
	int	flag = NickIsValid(server, client, Nick);
	if (flag == 1)
		reply = ": 432 : " + client.GetNickname() + " :Erroneus nickname\r\n";
	else if (flag == 2){
		if (client.GetNickname().empty() == true)
			client.SetNickname(Nick);
		reply = ": 433 " + Nick + " :Nickname is already in use\r\n";
	}
	else {
		if (client.GetNickname().empty() == true)
			reply = ":" + Nick + " NICK :" + Nick + "\r\n";
		else
			reply = ":" + client.GetNickname() + " NICK :" + Nick + "\r\n";
		client.SetNickname(Nick);
		client.SetStatement(NICK, true);
	}
	client.SetMessageToSend(reply);
	return ;
}

static int	NickIsValid(Server &server, Client &client, std::string Nick)
{
	if (Nick.size() > 9 || Nick.find_first_of(":#& ") != std::string::npos)
		return (1);
	Client	*ptr = server.GetClient(Nick);
	if (ptr != NULL && ptr != &client)
		return (2);
	return (0);
}
