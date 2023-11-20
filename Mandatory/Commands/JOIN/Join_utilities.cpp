#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
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
