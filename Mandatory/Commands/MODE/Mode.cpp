#include "Server.hpp"
#include "Client.hpp"
#include "Tools.hpp"
#include "Mode.hpp"
#include <string>

void	Mode(Server &server, Client &client, std::string &RawCmd)
{
	std::vector<std::string>	CmdParts;
	std::list<CmdNode>			Modes;
	Channel						*ChanPtr;
	int							flag;

	RawCmd.erase(RawCmd.find_first_of("\r\n"), std::string::npos);
	CmdParts = split(RawCmd, " ");
	flag = ParsingModeCmd(server, client, CmdParts, Modes);
	ChanPtr = server.GetChannel(CmdParts[0]);
	if (flag != 0)
		ModeReply(client, ChanPtr, flag);
	else
	{
		for (	std::list<CmdNode>::iterator It = Modes.begin();
				It != Modes.end(); It++)
		{
			flag = ChangeMode(server, ChanPtr, *It);
//			ModeReply(client, ChanPtr, flag);
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
