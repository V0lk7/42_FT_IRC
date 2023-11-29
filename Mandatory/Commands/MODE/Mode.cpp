#include "Server.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include <string>

void	Mode(Server &server, Client &client, std::string &RawCmd)
{
	std::vector<std::string>	CmdParts;
	ErrorFlag					flag;

	RawCmd.erase(RawCmd.find_first_of("\r\n"), std::string::npos);
	CmdParts = split(RawCmd, " ");
	flag = ParsingModeCmd(server, client, CmdParts);
}
