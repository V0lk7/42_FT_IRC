#ifndef MODE_HPP
# define MODE_HPP

# include <string>
# include <vector>
# include <list>

class Server;
class Client;

typedef enum Operand {
	SET = 43,
	UNSET = 45
} Operand;

typedef struct CmdNode
{
	Operand		Op;
	std::string	Mode;
	std::string	Param;
} CmdNode;

int	ParsingModeCmd(	Server const &server,
					Client &client,
					std::vector<std::string> &Cmd,
					std::list<CmdNode> &Modes);

#endif

//ERR_NEEDMOREPARAMS - 461: Returned by the server when a command requires more parameters
//							than have been provided by the client. 1
//
//RPL_CHANNELMODEIS - 324:	Sent by the server in response to a MODE command for a channel
//							to display the existing channel modes. 1
//
//ERR_CHANOPRIVSNEEDED - 482:	Returned by the server when a client attempts to perform a channel
//								operation but does not have the necessary privileges. 1
//
//ERR_NOSUCHNICK - 401: Returned by the server when a nickname parameter supplied for
//						a command is currently unused. 1
//
//ERR_NOTONCHANNEL - 442:	Sent by the server to a user who is either (a) not on the channel
//							which is the target of the command or (b) not a channel operator on
//							the channel which is the target of the command and the command can
//							only be performed by a channel operator. 1
//
//ERR_KEYSET - 467: Returned by the server when a client attempts to change the key for a
//					channel that already has a key set. 1
//
//ERR_UNKNOWNMODE - 472:	Returned by the server when a client sends a mode flag that
//							the server does not recognize. 1
//
//ERR_NOSUCHCHANNEL - 403: Returned by the server when the given channel name is invalid. 1
