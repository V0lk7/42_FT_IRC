#include "Mode.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

ErrorFlag	ParsingModeCmd(	Server const &server,
							Client const &client,
							std::vector<std::string> Cmd)
{
	Cmd.erase(Cmd.begin());
	if (Cmd.empty() == true)
		return (ERR_NEEDMOREPARAMS);
	Channel	*ptr = server.GetChannel(Cmd[0]);
	if (ptr == NULL)
		return (ERR_NOSUCHCHANNEL);
	if (ptr->IsClientOperator(client) == false)
		return (ERR_CHANOPRIVSNEEDED);
}
