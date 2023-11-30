#include "Server.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include "Mode.hpp"
#include <string>

void	Mode(Server &server, Client &client, std::string &RawCmd)
{
	std::vector<std::string>	CmdParts;
//	std::list<CmdNode>			Modes;

	RawCmd.erase(RawCmd.find_first_of("\r\n"), std::string::npos);
	CmdParts = split(RawCmd, " ");
	int const flag = ParsingModeCmd(server, client, CmdParts, Modes);
}
