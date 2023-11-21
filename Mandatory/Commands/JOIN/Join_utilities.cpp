#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Join.hpp"
#include <map>
#include <string>

void	CreateNewChannel(	Server &server, Client &client,
							std::map<std::string, std::string>::iterator &ChanParams)
{
	Channel	*NewChannel = new Channel(ChanParams->first);

	if (ChanParams->second.empty() != true)
	{
		NewChannel->SetPassword(ChanParams->second);
		NewChannel->SetMode(PASSWORD_SET, true);
	}
	NewChannel->AddClientToChannel(client, true);
	server.AddChannel(NewChannel);
}

bool	VerifyChannelLimit(Channel &chan)
{
	size_t	size;

	if (chan.GetLimitUsers() == 0)
		return (true);
	size = chan.GetUsers().size();
	if (size + 1 > chan.GetLimitUsers())
		return (false);
	else
		return (true);
}

bool	VerifyInvitOnly(Channel const &chan, Client &client)
{
	if (chan.GetMode(INVITE_ONLY) != true)
		return (true);
	if (chan.UserInWaitingList(client) == true)
		return (true);
	else
		return (false);
}

bool	VerifyPasswordNeed(Channel const &chan, std::string const &Passwd)
{
	if (chan.GetMode(PASSWORD_SET) != true)
		return (true);
	if (chan.GetPassword() == Passwd)
		return (true);
	else
		return (false);
}

//void	ErrorHandling(Client &client, ErrorsFlag Error)
//{
//
//}

void	CreateReply(Client &client, Channel &channel, int flag)
{
	std::string	Reply;
	std::string	ClientName(client.GetNickname());
	std::string	ChannelName(channel.GetName());

	if (flag == NEW_CHANNEL){
		Reply	= ":" + ClientName + " JOIN #" + ChannelName + "\r\n";
		Reply	+= ": 353 " + ClientName + " = #"
				+ ChannelName + " :@" + ClientName + "\r\n";
		Reply	+= ": 366 " + ClientName + " #"
				+ ChannelName + " :End of /NAMES list.\r\n";
	}
	else if (flag == EXISTING_CHANNEL){
		Reply	= ":" + ClientName + " JOIN #" + ChannelName + "\r\n";
//		channel.SendToClientList(Reply, ClientName);
		Reply	+= ": 332 " + ClientName + " #" + ChannelName + " :" + channel.GetTopic() + "\r\n";
		Reply	+= ": 353 " + ClientName + " = #"
				+ ChannelName + " :" + channel.GetListClientIn() + "\r\n";
		Reply	+= ": 366 " + ClientName + " #"
				+ ChannelName + " :End of /NAMES list.\r\n";
	}
	else if (flag == BAD_KEY){
		Reply	= ": 475 " + ClientName + " #" + ChannelName
				+ " :Cannot join channel (+k) - bad key\r\n";
	}
	else if (flag == TOO_MANY_CLIENT){
		Reply	= ": 471 " + ClientName + " #" + ChannelName
				+ " :Cannot join channel (+l) - channel full\r\n";
	}
	else if (flag == NOT_INVITED){
		Reply	= ": 473 " + ClientName + " #" + ChannelName
				+ " :Cannot join channel (+i) - not invited\r\n";
	}
	else if (flag == ALREADY_IN){
		Reply	= ": 338 " + ClientName + " #" + ChannelName
				+ " :Cannot join channel, you're already in\r\n";
	}
	else
		Reply = "";
	client.SetMessageToSend(Reply);
}

//void	ErrorHandling(Client &client, ErrorsFlag flag)
//{
//	
//}
