#ifndef JOIN_HPP
# define JOIN_HPP

# include <map>
# include <vector>
# include <string>

class Server;
class Client;
class Channel;

enum Reply {
	NEW_CHANNEL,
	EXISTING_CHANNEL,
	BAD_CHANNEL
};

bool	OrganiseRequest(	std::map<std::string, std::string> &Request,
							std::vector<std::string> &CmdParts);

bool	CreateNewChannel(	Server &server, Client &client,
							std::map<std::string, std::string>::iterator &chanParams);

bool	VerifyChannelLimit(Channel &chan);
bool	VerifyInvitOnly(Channel const &chan, Client &client);
bool	VerifyPasswordNeed(Channel const &chan, std::string const &Passwd);
void	CreateReply(Client &client, Channel &channel, int flag);
//void	ErrorHandling(Client &client, Error const Type);

#endif
