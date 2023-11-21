#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>
#include <ostream>

Channel::Channel( void ) {}

Channel::Channel( Client& one, Client& two, Client& three ) :
                           _Name( "Test" ),
                           _Password( "password" ),
                           _Topic( "Test" ),
                           _LimitUsers( 5 )
{
    _Mode[ INVITE_ONLY  ] = false;
    _Mode[ TOPIC_CHANGE ] = false;
    _Mode[ PASSWORD_SET ] = false;

    _Users[ &one ]   = true;
    _Users[ &two ]   = false;
    _Users[ &three ] = false;

    _WaitingList.push_back( new Client( "Jimmy", CLIENT ) );
}

Channel::Channel(std::string const &NewName) : _Name(NewName), _Password(""),
												_Topic(""), _LimitUsers(0)
{
	_Mode[INVITE_ONLY] = false;
	_Mode[TOPIC_CHANGE] = false;
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

    _Mode[ INVITE_ONLY  ] = rhs._Mode[ INVITE_ONLY  ];
    _Mode[ TOPIC_CHANGE ] = rhs._Mode[ TOPIC_CHANGE ];
    _Mode[ PASSWORD_SET ] = rhs._Mode[ PASSWORD_SET ];

    _Users                = rhs._Users;
    _WaitingList          = rhs._WaitingList;

    return ( *this );
}

//----------------Set/Get-------------------//

std::map<Client*, bool> Channel::GetUser( void ) const { return ( _Users ); }

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
	this->_Password = NewTopic;
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

/*----------------------SpecificMethods----------------------------*/

void	Channel::AddClientToChannel(Client &src, int Admin)
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

std::ostream&	operator<<(std::ostream& print, const Channel& other)
{
	print << "CHANNEL NAME : " << other.GetName() << std::endl;
	print << "PASSWORD : " << other.GetPassword() << std::endl;
	print << "TOPIC : " << other.GetTopic() << std::endl;
	print << "LIMIT USERS : " << other.GetLimitUsers() << std::endl;
	print << "INVITE ONLY : " << other.GetMode(INVITE_ONLY) << std::endl;
	print << "TOPIC CHANGE : " << other.GetMode(TOPIC_CHANGE) << std::endl;
	print << "PASSWORD SET : " << other.GetMode(PASSWORD_SET) << std::endl;
	return (print);
}
