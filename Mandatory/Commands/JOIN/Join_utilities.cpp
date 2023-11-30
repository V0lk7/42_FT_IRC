#include "Join.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Error_code.hpp"

bool	CreateNewChannel(	Server &server, Client &client,
							std::map<std::string, std::string>::iterator &ChanParams)
{
	std::string	ChanName(ChanParams->first);
	std::string	ChanKey(ChanParams->second);
	Channel		*NewChannel;

	if ((ChanName[0] != '#' && ChanName[0] != '&')
		|| ChanName.size() > static_cast<size_t>(CHAN_NAMESIZE_MAX)
		|| ChanName.find('\a') != std::string::npos)
		return (false);

	NewChannel = new Channel(ChanParams->first);
	if (ChanParams->second.empty() != true)
	{
		NewChannel->SetPassword(ChanParams->second);
		NewChannel->SetMode(PASSWORD_SET, true);
	}
	NewChannel->AddClientToChannel(client, true);
	server.AddChannel(NewChannel);
	return (true);
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
	if (chan.GetMode(INVITE_ONLY_SET) != true)
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

void	CreateReply(Client &client, Channel &channel, int flag)
{
	std::string	Reply;
	std::string	ClientName(client.GetNickname());
	std::string	ChannelName;

	if (flag != BAD_CHANNEL)
		ChannelName = channel.GetName();

	if (flag == NEW_CHANNEL){
		Reply	= ":" + ClientName + " JOIN " + ChannelName + ".\r\n";
		Reply	+= ": 353 " + ClientName + " = "
				+ ChannelName + " :@" + ClientName + ".\r\n";
		Reply	+= ": 366 " + ClientName + " "
				+ ChannelName + " :End of /NAMES list.\r\n";
	}
	else if (flag == BAD_CHANNEL){
		Reply = ": 461 " + ClientName + " JOIN :Bad Channel name.\r\n";
	}
	else if (flag == EXISTING_CHANNEL){
		Reply	= ":" + ClientName + " JOIN " + ChannelName + ".\r\n";
		channel.SendMessageToClients(Reply, client);
		if (channel.GetTopic().empty() == false)
			Reply	+= ": 332 " + ClientName + " " + ChannelName
					+ " :" + channel.GetTopic() + ".\r\n";
		else
			Reply	+= ": 331 " + ClientName + " " + ChannelName + " :No Topic set.\r\n";
		Reply	+= ": 353 " + ClientName + " = " + ChannelName
				+ " :" + channel.GetListClientIn() + ".\r\n";
		Reply	+= ": 366 " + ClientName + " "
				+ ChannelName + " :End of /NAMES list.\r\n";
	}
	else if (flag == ERR_BADCHANNELKEY){
		Reply	= ": 475 " + ClientName + " " + ChannelName
				+ " :Cannot join channel (+k) - bad key.\r\n";
	}
	else if (flag == ERR_CHANNELISFULL){
		Reply	= ": 471 " + ClientName + " " + ChannelName
				+ " :Cannot join channel (+l) - channel full.\r\n";
	}
	else if (flag == ERR_INVITEONLYCHAN){
		Reply	= ": 473 " + ClientName + " " + ChannelName
				+ " :Cannot join channel (+i) - not invited.\r\n";
	}
	else if (flag == ERR_USERONCHANNEL){
		Reply	= ": 443 " + ClientName + " " + ChannelName
				+ " :Cannot join channel, you're already in.\r\n";
	}
	else
		Reply = "";
	client.SetMessageToSend(Reply);
}
