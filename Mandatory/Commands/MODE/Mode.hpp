#ifndef MODE_HPP
# define MODE_HPP

# include <string>
# include <vector>
# include <list>

# define INVITONLY_CHANGED 0
# define TOPIC_CHANGED 1
# define PASSWORD_CHANGED 2
# define CLIENTRIGHT_CHANGED 3
# define LIMIT_CHANGED 4 
# define LIST_MODE 5 

class Server;
class Channel;
class Client;

typedef struct CmdNode
{
	bool		Op;
	char		Mode;
	std::string	Param;
} CmdNode;

int	ParsingModeCmd(	Server const &server,
					Client &client,
					std::vector<std::string> &Cmd,
					std::list<CmdNode> &Modes);

int	ChangeInvitOnlyMode(Channel *ChanPtr, CmdNode const &Mode);
int ChangeTopicMode(Channel *ChanPtr, CmdNode const &Mode);
int	ChangePasswordMode(Channel *ChanPtr, CmdNode const &Mode);
int	ChangeOperatorPrivilege(Server &server, Channel *ChanPtr, CmdNode const &Mode);
int ChangeLimitMode(Channel *ChanPtr, CmdNode const &Mode);

#endif
