#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>

class Channel;
class Client;

class Server {

	private:
		
		std::string						_Password;
		int								_MasterSocket;
		std::list<Channel *>			_ChannelList;
		std::list<Client *>				_ClientList;

		Server	&operator=(Server const &rhs);
		Server(Server const &src);
		Server();

	public:
		Server(std::string const &);
		~Server();

}

#endif
