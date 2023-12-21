#include "Server.hpp"
#include "Client.hpp"
#include <string>

void	User(Client &client, std::string &Auth)
{
	if (client.GetStatementStep(PASSWD) == false) {
		client.SetMessageToSend(": 400 : Password needed before\r\n");
		return ;
	}
	if (client.GetStatementStep(USER) == true) {
		client.SetMessageToSend(": 462 : You may not reregister\r\n");
		return ;
	}

	size_t	pos = Auth.find(" ");
	pos = Auth.find_first_not_of(" ", pos);
	if (pos == std::string::npos) {
		client.SetMessageToSend(": 461 USER : Not enough parameters\r\n");
		return ;
	}
	std::string	User(Auth.substr(pos));
	client.SetUsername(User);
	client.SetStatement(USER, true);
//	client.SetStatement(OK, true);
	return ;
}
