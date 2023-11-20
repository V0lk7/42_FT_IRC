#ifndef JOIN_HPP
# define JOIN_HPP

# include <map>
# include <vector>
# include <string>

class Server;
class Client;
class Channel;

typedef enum ErrorsFlag {
	NONE,
	NO_PARAMETERS,
	TOO_MANY_PARAMETERS,
	WRONG_FORMAT
} ErrorsFlag;

ErrorsFlag	OrganiseRequest(	std::map<std::string, std::string> &Request,
								std::vector<std::string> &CmdParts);

void	CreateNewChannel(	Server &server, Client &client,
							std::map<std::string, std::string>::iterator &chanParams);

bool	VerifyChannelLimit(Channel &chan);
bool	VerifyInvitOnly(Channel const &chan, Client &client);
bool	VerifyPasswordNeed(Channel const &chan, std::string const &Passwd);

#endif
