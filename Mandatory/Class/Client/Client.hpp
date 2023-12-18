#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

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

		Client(Client const &src);
		Client	&operator=(Client const &rhs);

	public :
		Client();
		~Client();

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
