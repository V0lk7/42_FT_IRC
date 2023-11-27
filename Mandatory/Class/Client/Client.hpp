#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

/*########################################################################################
	Class Client which is for taking all the information of the potential new client
	connected to the server.
	_Socket => File Descriptor of the socket link to the client.
	_Nickname => Nickname used for the IRC protocol. 9 characters only. ONLY ONE NICKNAME by client is unique
	_Username => Obligation name to be a valid client.
	_Auth => List of the setting conditions.
########################################################################################*/

enum right { SUDO, CLIENT };

enum step{
	OK,
	PASSWD,
	NICK,
	USER
};

class Client{

	private :
		int			_Socket;
		std::string	_Nickname;
		std::string	_Username;
		bool		_Auth[4];

		std::string	_InputBuffer;
		std::string	_MessageToSend;


	public :
		Client();
		~Client();

        // TOOLS DEV
		Client( const std::string name, right right );
		Client(Client const &src);
		Client	&operator=(Client const &rhs);
        // TOOLS DEV

		void            SetNickname(std::string const &);
		void            SetUsername(std::string const &);
		void            SetSocket(int const &);
		void            SetPasswd(void);
		void            SetStatement(int, bool);
		void            SetInputBuffer(std::string const &);
		void            SetMessageToSend(std::string const &);

		std::string		GetNickname(void) const;
		std::string		GetUsername(void) const;
		int				GetSocket(void) const;
		bool			GetStatement(void) const;
		std::string		GetMessage(void) const;

        bool            GetStatementStep( step target ) const;

		std::string		GetInputBuffer(void) const;

		void		    ClearInputBuffer(void);
		void		    ClearMessage(void);
};

std::ostream&	operator<<(std::ostream& print, const Client& other);

#endif
