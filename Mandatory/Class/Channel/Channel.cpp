#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>
#include <iostream>

Channel::Channel( void ) {}

Channel::Channel(std::string const &NewName) : _Name(NewName), _Password(""),
												_Topic(""), _LimitUsers(0)
{
	_Mode[INVITE_ONLY_SET] = false;
	_Mode[TOPIC_CHANGE_SET] = true;
	_Mode[PASSWORD_SET] = false;
}

Channel::~Channel() {}

Channel::Channel(Channel const &src) {*this = src;}

Channel	&Channel::operator=(Channel const &rhs) 
{
    if ( this == &rhs )
        return ( *this );

    _Name                 = rhs._Name;
    _Password             = rhs._Password;
    _Topic                = rhs._Topic;
    _LimitUsers           = rhs._LimitUsers;

    _Mode[ INVITE_ONLY_SET  ] = rhs._Mode[ INVITE_ONLY_SET  ];
    _Mode[ TOPIC_CHANGE_SET ] = rhs._Mode[ TOPIC_CHANGE_SET ];
    _Mode[ PASSWORD_SET ] = rhs._Mode[ PASSWORD_SET ];

    _Users                = rhs._Users;
    _WaitingList          = rhs._WaitingList;

    return ( *this );
}

//----------------Set/Get-------------------//

std::map<Client*, bool>& Channel::GetUsers( void ) { return ( _Users ); }

std::list<Client*>&  Channel::GetWaitingList( void ) { return ( _WaitingList); }

void	Channel::SetName(std::string const &NewName)
{
	this->_Name = NewName;
}

void	Channel::SetPassword(std::string const &NewPassword)
{
	this->_Password = NewPassword;
}

void	Channel::SetTopic(std::string const &NewTopic)
{
	this->_Topic = NewTopic;
}

void	Channel::SetLimitUsers(size_t const &NewLimit)
{
	this->_LimitUsers = NewLimit;
}

void	Channel::SetMode(int Index, bool state)
{
	this->_Mode[Index] = state;
}

std::string	Channel::GetName(void) const
{
	return (this->_Name);
}

std::string	Channel::GetPassword(void) const
{
	return (this->_Password);
}

std::string	Channel::GetTopic(void) const
{
	return (this->_Topic);
}

size_t	Channel::GetLimitUsers(void) const
{
	return (this->_LimitUsers);
}

bool	Channel::GetMode(int Index) const
{
	return (this->_Mode[Index]);
}

size_t	Channel::GetSizeUsers(void) const
{
	return (this->_Users.size());
}

std::string	Channel::GetModeOn(void) const
{
	std::string	ModeOn;

	if (this->_LimitUsers > 0)
		ModeOn = "l";
	if (this->_Mode[INVITE_ONLY_SET] == true)
		ModeOn += "i";
	if (this->_Mode[PASSWORD_SET] == true)
		ModeOn += "k";
	if (this->_Mode[TOPIC_CHANGE_SET] == true)
		ModeOn += "t";
	return (ModeOn);
}

/*----------------------SpecificMethods----------------------------*/

void
Channel::displayUsers( void )
{
    int i = 0;
    std::map<Client*,bool>::iterator it = _Users.begin();
    std::cout << "\tChannel: " << _Name << std::endl;
    while ( it != _Users.end() ) 
    {
        std::cout << "Channel client[" << i <<"]: "
                  << it->first->GetUsername() << std::endl;
        it++; i++;
    }
}

void	Channel::AddClientToChannel(Client &src, bool Admin)
{
	if (this->_Users.find(&src) != this->_Users.end())
		return ;
	this->_Users[&src] = Admin;
}

void	Channel::EraseClientFromChannel(Client &src)
{
	std::map<Client *, bool>::iterator	It = this->_Users.find(&src);

	if (It == this->_Users.end())
		return ;
	this->_Users.erase(It);
}

void	Channel::PutClientOnWaitingList(Client &src)
{
	std::list<Client *>::iterator	ItBegin = this->_WaitingList.begin();
	std::list<Client *>::iterator	ItEnd = this->_WaitingList.end();

	if (std::find(ItBegin, ItEnd, &src) != ItEnd)
		return ;
	this->_WaitingList.push_back(&src);
}

void	Channel::EraseClientFromWaitingList(Client &src)
{
	std::list<Client *>::iterator	ItBegin = this->_WaitingList.begin();
	std::list<Client *>::iterator	ItEnd = this->_WaitingList.end();
	std::list<Client *>::iterator	ItClient = std::find(ItBegin, ItEnd, &src);

	if (ItClient == ItEnd)
		return ;
	this->_WaitingList.erase(ItClient);
}

void	Channel::ModifyClientRights(Client &src, bool Admin)
{
	std::map<Client *, bool>::iterator	It = this->_Users.find(&src);

	if (It == this->_Users.end())
		return ;
	if (Admin == true)
		It->second = true;
	else
		It->second = false;
}

bool	Channel::UserInChannel(Client &client) const
{
	if (this->_Users.find(&client) != this->_Users.end())
		return (true);
	return (false);
}

bool	Channel::UserInWaitingList(Client &client) const
{
	std::list<Client *>::const_iterator	ItBegin = this->_WaitingList.begin();
	std::list<Client *>::const_iterator	ItEnd = this->_WaitingList.end();

	if (std::find(ItBegin, ItEnd, &client) != ItEnd)
		return (true);
	return (false);
}

std::string	Channel::GetListClientIn(void)
{
	std::map<Client *, bool>::iterator	It = this->_Users.begin();
	std::string							ClientList;

	while (It != this->_Users.end())
	{
		if (It->second == true)
			ClientList += "@";
		else
			ClientList += "+";
		ClientList += It->first->GetNickname() + " ";
		It++;
	}
	ClientList.erase(ClientList.end() - 1);
	return (ClientList);
}

void	Channel::SendMessageToClients(std::string const &Message, Client const &client)
{
	std::map<Client *, bool>::iterator	It = this->_Users.begin();

	while (It != this->_Users.end())
	{
		if (It->first != &client)
			It->first->SetMessageToSend(Message);
		It++;
	}
	return ;
}

bool	Channel::IsClientOperator(Client &client)
{
	std::map<Client *, bool>::iterator	It = this->_Users.find(&client);

	if (It != this->_Users.end() && It->second == true)
		return (true);
	return (false);
}

std::ostream&	operator<<(std::ostream& print, const Channel& other)
{
	print << "CHANNEL NAME : " << other.GetName() << std::endl;
	print << "PASSWORD : " << other.GetPassword() << std::endl;
	print << "TOPIC : " << other.GetTopic() << std::endl;
	print << "LIMIT USERS : " << other.GetLimitUsers() << std::endl;
	print << "INVITE ONLY : " << other.GetMode(INVITE_ONLY_SET) << std::endl;
	print << "TOPIC CHANGE : " << other.GetMode(TOPIC_CHANGE_SET) << std::endl;
	print << "PASSWORD SET : " << other.GetMode(PASSWORD_SET) << std::endl;
	return (print);
}
