#include "Server.hpp"
#include "Client.hpp"
#include <string>

void	User(Client &client, std::string &Auth)
{
	if (client.GetStatementStep(PASSWD) == false) {
		client.SetMessageToSend(": 461 USER :Password needed before\r\n");
		return ;
	}
	if (client.GetStatementStep(NICK) == false) {
		client.SetMessageToSend(": 461 USER :Nick needed before\r\n");
		return ;
	}
	if (client.GetStatementStep(USER) == true) {
		client.SetMessageToSend(": 461 USER :User already set\r\n");
		return ;
	}

	size_t	pos = Auth.find(" ");
	pos = Auth.find_first_not_of(" ", pos);
	if (pos == std::string::npos) {
		client.SetMessageToSend(": 431 USER :Need more parameters\r\n");
		return ;
	}
	std::string	User(Auth.substr(pos));
	client.SetUsername(User);
	client.SetStatement(USER, true);
	client.SetStatement(OK, true);
	client.SetMessageToSend(": 461 USER :Username set\r\n"); //maybe useless
//	client.SetMessageToSend(": Welcome and enjoy ! \r\n"); //maybe useless
	return ;
}
