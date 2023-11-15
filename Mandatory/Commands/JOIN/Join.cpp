#include "Server.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include <map>

void	Join(Server &server, Client &client, std::string &RawCmd)
{
	std::map<std::string, std::string>	Request;
	std::vector<std::string>			CmdParts;
	int									ErrorFlag = 0;

	csplit(CmdParts, RawCmd, " ");
	ErrorFlag = OrganiseRequest(Request, CmdParts);
//	if (ErrorFlag != 0){
//		ErrorHandler(client, ErrorFlag);
//		return ;
//	}
}
