#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() {}

Channel::Channel(std::string const &NewName) : _Name(NewName), _Password(""),
												_Topic(""), _LimitUsers(0)
{
	_Mode[I] = false;
	_Mode[T] = false;
	_Mode[K] = false;
}

Channel::~Channel(){}

Channel::Channel(Channel const &src) {*this = src;}

Channel	&Channel::operator=(Channel const &rhs) {(void)rhs;}

//----------------Set/Get-------------------//

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

void	Channel::SetLimitUsers(std::string const &NewLimit)
{
	this->_LimitUsers = NewLimit;
}

void	Channel::SetMode(int Index, bool state)
{
	this->Mode[Index] = state;
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

void	Channel::AddClientToChannel(Client const &src, int Type)
{
	if (this->_Users.find(src) != map::end)
		throw std::logic_error("ALREADY_IN_CHANNEL");
	this->_Users[src] = Type;
}

void	Channel::EraseClientFromChannel(Client const &src)
{
	std::map<Client &, int>:: iterator	It = this->_Users.find(src);
	if (It == map::end)
		throw std::logic_error("NOT_IN_CHANNEL");
	this->_Users.erase(It);
}
