#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include "Join.hpp"

static void	HandleJoinChannel(	Server &server,
								Client &client,
								std::map<std::string, std::string>::iterator &It);

void	Join(Server &server, Client &client, std::string &RawCmd)
{
	std::map<std::string, std::string>				Request;
	std::map<std::string, std::string>::iterator	It;
	std::vector<std::string>						CmdParts;
	ErrorsFlag										Error;

	if (client.GetStatement() != true)
	//Maybe sending a message to the client is needed ?
		return ;
	CmdParts = split(RawCmd, " ");
	Error = OrganiseRequest(Request, CmdParts);
	//if (Error != NONE)
	//{
	//	ErrorHandling(client, Error);
	//	return ;
	//}
	for (std::map<std::string, std::string>::iterator It = Request.begin(); 
			It != Request.end(); It++)
		HandleJoinChannel(server, client, It);
	return ;
}

static void	HandleJoinChannel(	Server &server,
								Client &client,
								std::map<std::string, std::string>::iterator &It)
{
	Channel	*channel = server.GetChannel(It->first);

	if (channel == NULL)
		CreateNewChannel(server, client, It);
		//maybe send a message to the client
	else if (channel->UserInChannel(client) == true){
		//Send an error message to the client
		return ;
	}
	else if (VerifyChannelLimit(*channel) != true){
		//Send an error message to the client
		return ;
	}
	else if (VerifyInvitOnly(*channel, client) != true){
		//Send an error message to the client
		return ;
	}
	else if (VerifyPasswordNeed(*channel, It->second) != true){
		//Send an error message to the client
		return ;
	}
	else
	{
		channel->EraseClientFromWaitingList(client);
		channel->AddClientToChannel(client, false);
	}
	return ;
}
