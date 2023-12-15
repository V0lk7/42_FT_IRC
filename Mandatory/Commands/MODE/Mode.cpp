#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include "Mode.hpp"
#include "Error_code.hpp"

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
		Add = "+";
	else
		Add = "-";

	if (Flag == INVITONLY_CHANGED){
		Reply = ":" + ClientName + " MODE " + ChannelName + " " + Add + Mode->Mode + "\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
	}
	else if (Flag == TOPIC_CHANGED){
		Reply = ":" + ClientName + " MODE " + ChannelName + " " + Add + Mode->Mode + "\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
	}
	else if (Flag == LIMIT_CHANGED){
		Reply = ":" + ClientName + " MODE " + ChannelName + " " + Add + Mode->Mode + " ";
		if (Add == "+")
			Reply += Mode->Param;
		Reply += "\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
	}
	else if (Flag == PASSWORD_CHANGED){
		Reply	= ":" + ClientName + " MODE " + ChannelName + " " + Add + Mode->Mode + " ";
		if (Add == "+")
			Reply += Mode->Param;
		Reply += "\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
	}
	else if (Flag == CLIENTRIGHT_CHANGED){
		Reply	= ":" + ClientName + " MODE " + ChannelName + " " + Add + Mode->Mode + " "
				+ Mode->Param + "\r\n";
		ChanPtr->SendMessageToClients(Reply, client);
	}
	else if (Flag == ERR_NEEDMOREPARAMS){
		Reply	= ": 467 " + ClientName + " " + ChannelName + " " + Mode->Mode 
				+ " *: You must specify a parameter with " + Add + Mode->Mode + "\r\n";
	}
	else if (Flag == ERR_NOSUCHNICK){
		Reply = ": 401 MODE " + ChannelName + " Mode: Unknow Nickname: " + Mode->Param + "\r\n";
	}
	else if (Flag == ERR_NOSUCHCHANNEL){
		Reply = ": 403 MODE :No such channel\r\n";
	}
	else if (Flag == ERR_NOTONCHANNEL){
		Reply = ": 442 MODE :Not on channel\r\n";
	}
	else if (Flag == ERR_UNKNOWNMODE){
		Reply = ": 472 MODE " + ChannelName + " Mode: Unknow mode\r\n";
	}
	else if (Flag == ERR_KEYSET)
		Reply = "";
	client.SetMessageToSend(Reply);
}
