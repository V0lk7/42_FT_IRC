#include "Join.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Tools.hpp"
#include "Error_code.hpp"

static void	HandleJoinChannel(	Server &server,
								Client &client,
								std::map<std::string, std::string>::iterator &It);

void	Join(Server &server, Client &client, std::string &RawCmd)
{
	std::map<std::string, std::string>				Request;
	std::map<std::string, std::string>::iterator	It;
	std::vector<std::string>						CmdParts;

	CmdParts = split(RawCmd, " ");
	if (OrganiseRequest(Request, CmdParts) == false)
	{
		client.SetMessageToSend(": 461 " + client.GetNickname() +
								" JOIN :Need more parameters\r\n");
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
		if (CreateNewChannel(server, client, It) != true)
			CreateReply(client, *(server.GetChannel(It->first)), BAD_CHANNEL);
		else
			CreateReply(client, *(server.GetChannel(It->first)), NEW_CHANNEL);
	}
	else if (channel->UserInChannel(client) == true)
		CreateReply(client, *channel, ERR_USERONCHANNEL);
	else if (VerifyChannelLimit(*channel) != true)
		CreateReply(client, *channel, ERR_CHANNELISFULL);
	else if (VerifyInvitOnly(*channel, client) != true)
		CreateReply(client, *channel, ERR_INVITEONLYCHAN);
	else if (VerifyPasswordNeed(*channel, It->second) != true)
		CreateReply(client, *channel, ERR_BADCHANNELKEY);
	else
	{
		channel->EraseClientFromWaitingList(client);
		channel->AddClientToChannel(client, false);
		CreateReply(client, *channel, EXISTING_CHANNEL);
	}
	return ;
}
