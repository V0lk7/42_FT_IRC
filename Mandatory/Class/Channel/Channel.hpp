#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>
# include <list>

/*########################################################################################
	Class wich will keep all the informations usefull of the channel.
	_Name => Name of the channel;
	_Password => Password for a new client to enter the channel;
	_Topic => Topic of the channel;
	_LimitUsers => Define the numbers of client the channel can have (0 = no limit);
	_Mode => Define the MODE parameters of the channel:
		-> Mode[0] = "i" Invite only channel 0 = NO, 1 = YES.
		-> Mode[1] = "t" Restriction of the TOPIC command,
					only to channel operator. 0 = NO, 1 = YES.
		-> Mode[2] = "k" Define a password for the channel. 0 = NO, 1 = YES.
	_Users => Define the client list which is in the channel.
		 Map ->
			Key => The client.
			Value => Grade of the client (channel operator or not)
########################################################################################*/

enum mode {
	INVITE_ONLY,
	PASSWORD_SET,
	TOPIC_CHANGE
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


	public :
		Channel(std::string const &);
		~Channel();

        //_TOOL DEV___________________________________________________________//
        Channel( Client& one, Client& two, Client& tree, Client& four );
		Channel	&operator=(Channel const &rhs);
        //_TOOL DEV___________________________________________________________//

		Channel(Channel const &src);
		Channel();

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
		std::map<Client *, bool>	&GetUsers(void);

        std::map<Client *, bool>    GetUser( void ) const;

		void                        AddClientToChannel(Client &, bool);
		void                        EraseClientFromChannel(Client &);
		bool                        UserInChannel(Client &) const;
		bool                        UserInWaitingList(Client &) const;

		void                        PutClientOnWaitingList(Client &);
		void                        EraseClientFromWaitingList(Client &);
		void                        ModifyClientRights(Client &, bool);
		void                        SendMessageToClients(std::string const &, Client const &);
		bool                        IsClientOperator(Client &client);
};

std::ostream&	operator<<(std::ostream& print, const Channel& other);

#endif
