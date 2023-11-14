#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>
# include <ostream>
# include <unistd.h>

class Channel;
class Client;

class Server {

	private:

		std::string				_Password;
		int						_MasterSocket;
		std::list<Channel *>	_ChannelList;
		std::list<Client *>		_ClientList;

		Server	&operator=(Server const &rhs);
		Server(Server const &src);
		Server();

	public:
		Server(std::string const &);
		~Server();

		void	SetPassword(std::string const &);
		void	SetMasterSocket(int const &);
		void	AddChannel(Channel &);
		void	AddClient(Client &);

		std::string	GetChannelList(void) const;
		std::string	GetClientList(void) const;
		std::string	GetPassword(void) const;
		int			GetMasterSocket(void) const;
		Channel		*GetChannel(std::string const &) const;
		Client		*GetClient(std::string const &) const;

};

std::ostream&	operator<<(std::ostream& print, const Server& other);

#endif
