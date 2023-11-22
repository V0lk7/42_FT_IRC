#include "Join.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include <iostream>

static void	HandleJoinChannel(	Server &server,
								Client &client,
								std::map<std::string, std::string>::iterator &It);

void	Join(Server &server, Client &client, std::string &RawCmd)
{
	std::map<std::string, std::string>				Request;
	std::map<std::string, std::string>::iterator	It;
	std::vector<std::string>						CmdParts;

	if (client.GetStatement() != true)
	{
		ErrorHandling(client, INVALID_CLIENT);
		return ;
	}
	RawCmd.erase(RawCmd.find_first_of("\r\n"), std::string::npos);
	CmdParts = split(RawCmd, " ");
	if (OrganiseRequest(Request, CmdParts) == false)
	{
		ErrorHandling(client, SYNTAX_ERROR);
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
