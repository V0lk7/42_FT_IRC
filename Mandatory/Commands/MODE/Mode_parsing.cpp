#include "Mode.hpp"
#include "Error_code.hpp"
#include "Tools.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

static bool		CheckModeFormat(std::string const &Modes);
static void		CreateModes(std::vector<std::string> &Cmd,
							std::list<CmdNode> &Modes);
static CmdNode	CreateNode(bool const Op, char const Mode);
static int		InsertParameter(CmdNode &Node, std::string Parameter);
	
int	ParsingModeCmd(	Server const &server,
					Client &client,
					std::vector<std::string> &Cmd,
					std::list<CmdNode> &Modes)
{
	Cmd.erase(Cmd.begin());
	if (Cmd.empty() == true)
		return (ERR_NEEDMOREPARAMS);
	Channel	*Chanptr = server.GetChannel(Cmd[0]);
	if (Chanptr == NULL)
		return (ERR_NOSUCHCHANNEL);
	else if (Chanptr->UserInChannel(client) == false)
		return (ERR_NOTONCHANNEL);
	else if (Chanptr->IsClientOperator(client) == false)
		return (ERR_CHANOPRIVSNEEDED);
	else if (Cmd.size() == 1)
		return (RPL_CHANNELMODEIS);
	else if (CheckModeFormat(Cmd[1]) != true)
		return (ERR_UNKNOWNMODE);
	else
		CreateModes(Cmd, Modes);
	return (0);
}

static bool	CheckModeFormat(std::string const &Modes)
{
	if (Modes.empty() != false || (Modes[0] != '+' && Modes[0] != '-'))
		return (false);
	else if (Modes.size() < 2)
		return (false);
	else if (Modes.find_first_not_of("itkol", 1) != std::string::npos)
		return (false);
	else
		return (true);
}

static void	CreateModes(	std::vector<std::string> &Cmd,
							std::list<CmdNode> &Modes)
{
	std::vector<std::string>	Parameters;
	std::string const			RawModes(Cmd[1]);
	bool						Op = (RawModes[0] == '+' ? true : false);
	size_t						j = 0;

	if (Cmd.size() > 2)
		Parameters = split(Cmd[2], ",");
	for (size_t i = 1; i < RawModes.size(); i++)
	{
		CmdNode	Node = CreateNode(Op, RawModes[i]);
		if (Parameters.empty() != true && j < Parameters.size())
			j += InsertParameter(Node, Parameters[j]);
		Modes.push_back(Node);
	}
}

static CmdNode	CreateNode(bool const Op, char const Mode)
{
	CmdNode	Node;

	Node.Op = Op;
	Node.Mode = Mode;
	return (Node);
}

static int InsertParameter(CmdNode &Node, std::string Parameter)
{
	if (Node.Mode == 'i' || Node.Mode == 't')
		return (0);
	else
		Node.Param = Parameter;
	return (1);
}
