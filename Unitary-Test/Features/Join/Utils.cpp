#include <string>
#include <sstream>
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

std::string	IntToString(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

Client *SetupCli(std::string Name)
{
	Client	*ptr = new Client;

	ptr->SetNickname(Name);
	for (int i = 0; i < 4; i++)
		ptr->SetStatement(i, true);
	return (ptr);
}

void	Setup(Server &serv, int ChanNbr, int CliNbr)
{
	std::string	ChanName;
	std::string	CliName;
	Client		*CliPtr;

	for (int i = 0; i < ChanNbr; i++)
	{
		ChanName = "#Chan" + IntToString(i);
		serv.AddChannel(new Channel(ChanName));
	}
	for (int i = 0; i < CliNbr; i++)
	{
		CliName = "Client" + IntToString(i);
		CliPtr = SetupCli(CliName);
		serv.AddClient(CliPtr);
	}
}
