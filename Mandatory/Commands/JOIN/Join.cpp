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
	if (Error != NONE)
	{
//		ErrorHandling(client, Error);
		return ;
	}
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

	if (channel == NULL){
		CreateNewChannel(server, client, It);
		CreateReply(client, *(server.GetChannel(It->first)), NEW_CHANNEL);
	}
	else if (channel->UserInChannel(client) == true)
		CreateReply(client, *channel, ALREADY_IN);
	else if (VerifyChannelLimit(*channel) != true)
		CreateReply(client, *channel, TOO_MANY_CLIENT);
	else if (VerifyInvitOnly(*channel, client) != true)
		CreateReply(client, *channel, NOT_INVITED);
	else if (VerifyPasswordNeed(*channel, It->second) != true)
		CreateReply(client, *channel, BAD_KEY);
	else
	{
		channel->EraseClientFromWaitingList(client);
		channel->AddClientToChannel(client, false);
		CreateReply(client, *channel, EXISTING_CHANNEL);
	}
	return ;
}
