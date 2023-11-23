#include "Mode.hpp"

ErrorFlag	ParsingModeCmd(	Server const &server,
							Client const &client,
							std::vector<std::string> Cmd)
{
	Cmd.erase(Cmd.begin());
	if (Cmd.empty() == true)
		return (ERR_NEEDMOREPARAMS);
	Channel	*ptr = server.Get	
}
