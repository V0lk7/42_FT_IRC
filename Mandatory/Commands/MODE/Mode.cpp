#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include "Mode.hpp"
#include "Error_code.hpp"
#include <string>

static int	ChangeMode(Server &server, Channel *ChanPtr, CmdNode const &Mode);
static void	ModeReply(Client &client, Channel *ChanPtr, int Flag, CmdNode *Mode);

void	Mode(Server &server, Client &client, std::string &RawCmd)
{
	std::vector<std::string>	CmdParts;
	std::list<CmdNode>			Modes;
	Channel						*ChanPtr;
	int							flag;

	CmdParts = split(RawCmd, " ");
	flag = ParsingModeCmd(server, client, CmdParts, Modes);
	ChanPtr = server.GetChannel(CmdParts[0]);
	if (flag != 0)
		ModeReply(client, ChanPtr, flag, NULL);
	else
	{
		for (	std::list<CmdNode>::iterator It = Modes.begin();
				It != Modes.end(); It++)
		{
			flag = ChangeMode(server, ChanPtr, *It);
			ModeReply(client, ChanPtr, flag, &(*It));
		}
	}
	return ;
}

static int	ChangeMode(Server &server, Channel *ChanPtr, CmdNode const &Mode)
{
	int	flag = 0;

	switch (Mode.Mode){
		case 'i':
			flag = ChangeInvitOnlyMode(ChanPtr, Mode);
			break ;
		case 't':
			flag = ChangeTopicMode(ChanPtr, Mode);
			break ;
		case 'k':
			flag = ChangePasswordMode(ChanPtr, Mode);
			break ;
		case 'o':
			flag = ChangeOperatorPrivilege(server, ChanPtr, Mode);
			break ;
		case 'l':
			flag = ChangeLimitMode(ChanPtr, Mode);
			break ;
	}
	return (flag);
}

static void	ModeReply(Client &client, Channel *ChanPtr, int Flag, CmdNode *Mode)
{
	std::string	Reply;
	std::string	Add;
	std::string	ClientName(client.GetNickname());
	std::string	ChannelName;

	if (ChanPtr != NULL)
		ChannelName = ChanPtr->GetName();
	
	if (Mode != NULL && Mode->Op == true)
		Add = "set";
	else
		Add = "unset";

	if (Flag == INVITONLY_CHANGED){
		Reply = ": 324 " + ClientName + " MODE " + ChannelName + ": " + Add + " invit only\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
		Reply = ": 324 MODE " + ChannelName + ": " + Add + " invit only\r\n";
	}
	else if (Flag == TOPIC_CHANGED){
		Reply = ": 324 " + ClientName + " MODE " + ChannelName + ": " + Add + " topic\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
		Reply = ": 324 MODE " + ChannelName + ": " + Add + " topic\r\n";
	}
	else if (Flag == LIMIT_CHANGED){
		Reply = ": 324 " + ClientName + " MODE " + ChannelName + ": " + Add + " limit user\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
		Reply = ": 324 MODE " + ChannelName + ": " + Add + " limit user\r\n";
	}
	else if (Flag == PASSWORD_CHANGED){
		Reply	= ": 324 " + ClientName + " MODE " + ChannelName
				+ ": " + Add + " channel password\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
		Reply = ": 324 MODE " + ChannelName + ": " + Add + " channel password\r\n";
	}
	else if (Flag == CLIENTRIGHT_CHANGED){
		Reply	= ": 324 " + ClientName + " MODE " + ChannelName + ": " + Add + " "
				+ Mode->Param + " operator\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
		Reply = ": 324 MODE " + ChannelName + ": " + Add + " " + Mode->Param + " operator\r\n";
	}
	else if (Flag == ERR_NEEDMOREPARAMS){
		Reply = ": 461 MODE " + ChannelName + ": parameter error\r\n";
	}
	else if (Flag == ERR_KEYSET){
		Reply = ": 467 MODE " + ChannelName + ": mode already " + Add + "\r\n";
	}
	else if (Flag == ERR_NOSUCHNICK){
		Reply = ": 401 MODE " + ChannelName + ": no such nickname\r\n";
	}
	else if (Flag == ERR_NOSUCHCHANNEL){
		Reply = ": 403 MODE : no such channel\r\n";
	}
	else if (Flag == ERR_NOTONCHANNEL){
		Reply = ": 442 MODE : not on channel\r\n";
	}
	else if (Flag == ERR_UNKNOWNMODE){
		Reply = ": 472 MODE : unknow mode\r\n";
	}
	client.SetMessageToSend(Reply);
}
