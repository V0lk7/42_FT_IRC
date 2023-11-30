#include "Mode.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

int const	ParsingModeCmd(	Server const &server,
							Client const &client,
							std::vector<std::string> Cmd)
{
	Cmd.erase(Cmd.begin());
	if (Cmd.empty() == true)
		return (ERR_NEEDMOREPARAMS);
	Channel	*Chanptr = server.GetChannel(Cmd[0]);
	if (ptr == NULL)
		return (ERR_NOSUCHCHANNEL);
	else if (Chanptr->UserInChannel(client) == false)
		return (ERR_NOTONCHANNEL);
	else if (Chanptr->IsClientOperator(client) == false)
		return (ERR_CHANOPRIVSNEEDED);
	else if (Cmd.size() == 1)
		return (RPL_CHANNELMODEIS);
}
