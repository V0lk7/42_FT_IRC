#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>
# include <ostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>

class Channel;
class Client;

class Server {

	private:

		std::string				_Password;
		int						_MasterSocket;
		struct sockaddr_in		_TcpConfig;
		std::list<Channel *>	_ChannelList;
		std::list<Client *>		_ClientList;


	public:
		Server(std::string const &);
		~Server();

		Server	&operator=(Server const &rhs);
		Server(Server const &src);
		Server();

		void	SetPassword(std::string const &);
		void	SetMasterSocket(int const &);
		void	SetTcpConfig(uint16_t const &);
		void	AddChannel(Channel *);
		void	AddClient(Client *);

		std::string	GetChannelList(void) const;
		std::string	GetClientList(void) const;

        std::list<Client *>&
        getCllist( void ) ;

		std::string			GetPassword(void) const;
		int					GetMasterSocket(void) const;
		struct sockaddr_in	GetTcpConfig(void) const;
		Channel				*GetChannel(std::string const &) const;
		Client				*GetClient(std::string const &) const;

		bool				SendReply(void);
		void				DisconnectClient(Client &);	
		void				EraseClientDisconnected(void);
};

std::ostream&	operator<<(std::ostream& print, const Server& other);

#endif
