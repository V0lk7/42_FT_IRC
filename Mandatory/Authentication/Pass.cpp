#include "Server.hpp"
#include "Client.hpp"
#include <string>

void	Pass(Server &server, Client &client, std::string &Auth)
{
	if (client.GetStatementStep(PASSWD) == true){
		if (server.GetPassword().empty() == true)
			client.SetMessageToSend(": 461 PASS :Password not needed\r\n");
		else
			client.SetMessageToSend(": 462 PASS :Password already accepted\r\n");
		return ;
	}
	if (server.GetPassword().empty() == true){
		client.SetStatement(PASSWD, true);
		client.SetMessageToSend(": 461 PASS :Password accepted\r\n"); //maybe useless
		return ;
	}
	size_t	pos = Auth.find(" ");
	if (pos == std::string::npos){
		client.SetMessageToSend(": 461 PASS :Need more parameters\r\n");
		return ;
	}
	pos = Auth.find_first_not_of(" ", pos);
	if (pos == std::string::npos){
		client.SetMessageToSend(": 461 PASS :Need more parameters\r\n");
		return ;
	}
	std::string	pass(Auth.substr(pos));
	if (pass != server.GetPassword()){
		client.SetMessageToSend(": 461 PASS :Invalid password\r\n");
		return ;
	}
	else {
		client.SetPasswd();
		client.SetMessageToSend(": 461 PASS :Password accepted\r\n"); //maybe useless
	}
	return ;
}