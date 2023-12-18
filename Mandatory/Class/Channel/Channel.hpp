#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <list>

enum mode {
	INVITE_ONLY_SET,
	PASSWORD_SET,
	TOPIC_CHANGE_SET
};

class Client;

class Channel{

	private :
		std::string				    _Name;
		std::string				    _Password;
		std::string				    _Topic;
		size_t					    _LimitUsers;
		bool					    _Mode[3];

		std::map<Client *, bool>	_Users;
		std::list<Client *>			_WaitingList;


		Channel();
		Channel	&operator=(Channel const &rhs);
		Channel(Channel const &src);

	public :

		Channel(std::string const &);
		~Channel();

		void                        SetName(std::string const &);
		void                        SetPassword(std::string const &);
		void                        SetTopic(std::string const &);
		void                        SetLimitUsers(size_t const &);
		void                        SetMode(int, bool);

		std::string					GetName(void) const;
		std::string					GetPassword(void) const;
		std::string					GetTopic(void) const;
		size_t						GetLimitUsers(void) const;
		bool						GetMode(int) const;
		std::string					GetListClientIn(void);
		std::map<Client *, bool>&	GetUsers(void) ;
		std::list<Client*>&	        GetWaitingList(void) ;
		size_t						GetSizeUsers(void) const;

		void                        AddClientToChannel(Client &, bool);
		void                        EraseClientFromChannel(Client &);
		bool                        UserInChannel(Client &) const;
		bool                        UserInWaitingList(Client &) const;

		void                        PutClientOnWaitingList(Client &);
		void                        EraseClientFromWaitingList(Client &);
		void                        ModifyClientRights(Client &, bool);
		void                        SendMessageToClients(std::string const &, Client const &);
		bool                        IsClientOperator(Client &client);

        void                        displayUsers( void );
};

std::ostream&	operator<<(std::ostream& print, const Channel& other);

#endif
