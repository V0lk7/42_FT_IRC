#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include <vector>
#include <string>
#include "Tools.hpp"

void	Who(Server &server, Client &client, std::string &RawCmd)
{
	std::vector<std::string>	Cmd = split(RawCmd, " ");

	Cmd.erase(Cmd.begin());
	if (Cmd.size() != 1){
		client.SetMessageToSend(": 461 " + client.GetNickname() +
								" JOIN :Need more parameters\r\n");
		return ;
	}
	Channel	*ChanPtr = server.GetChannel(Cmd[0]);
	if (ChanPtr == NULL)
		client.SetMessageToSend(": 403 WHO : no such channel\r\n");
	else if (ChanPtr->UserInChannel(client) != true){
		client.SetMessageToSend(": 442 WHO : not on channel\r\n");
	}
	else {
		client.SetMessageToSend(": 353 " + client.GetNickname() + " = " + ChanPtr->GetName() +  " :" + ChanPtr->GetListClientIn() + "\r\n");
		client.SetMessageToSend(": 366" + client.GetNickname() + ChanPtr->GetName() +  " :End\r\n");
	}
}
