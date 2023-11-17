#include "Server.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include "Join.hpp"

void	Join(Server &server, Client &client, std::string &RawCmd)
{
	std::map<std::string, std::string>				Request;
	std::map<std::string, std::string>::iterator	It;
	std::vector<std::string>						CmdParts;
	ErrorsFlag										Error;

	(void)server;
	(void)client;
	csplit(CmdParts, RawCmd, " ");
	Error = OrganiseRequest(Request, CmdParts);
	//if (Error != NONE)
	//{
	//	ErrorHandling(client, Error);
	//	return ;
	//}
//	for (std::map<std::string, std::string>::iterator	It = Request.begin();
//			It != Request.end(); It++);
//		HandleJoinChannel(server, client, It);
	return ;
}

static void	HandleJoinChannel(	Server &server,
								Client &client,
								std::map<std::string, std::string>::iterator &Chan)
{
		
}
