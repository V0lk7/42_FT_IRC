#include "Server.hpp"
#include "Client.hpp"
#include <string>

void	Pass(Server &server, Client &client, std::string &Auth)
{
	if (client.GetStatementStep(PASSWD) == true){
		client.SetMessageToSend(": 462 : You may not reregister\r\n");
		return ;
	}
	if (server.GetPassword().empty() == true){
		client.SetStatement(PASSWD, true);
		return ;
	}
	size_t	pos = Auth.find(" ");
	if (pos == std::string::npos){
		client.SetMessageToSend(": 461 PASS :Not enough parameters\r\n");
		return ;
	}
	pos = Auth.find_first_not_of(" ", pos);
	if (pos == std::string::npos){
		client.SetMessageToSend(": 461 PASS :Not enough parameters\r\n");
		return ;
	}
	std::string	pass(Auth.substr(pos));
	if (pass != server.GetPassword()){
		client.SetMessageToSend(": 464 : Password incorrect\r\n");
		return ;
	}
	else {
		client.SetPasswd();
	}
	return ;
}
