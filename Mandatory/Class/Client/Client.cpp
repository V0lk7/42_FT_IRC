#include "Client.hpp"
#include <ostream>
#include <unistd.h>

Client::Client() : _Socket(-1), _Nickname(""), _Username("")
{
	for (int i = 0; i < 4; i++)
		this->_Auth[i] = false;
}

Client::Client(Client const &src) {*this = src;}

Client	&Client::operator=(Client const &rhs) {(void)rhs; return (*this);}

Client::~Client()
{
	if (this->_Socket >= 0)
		close(this->_Socket);
}

//----------------Set/Get-------------------//

void	Client::SetNickname(std::string const &NewNickname)
{
	this->_Nickname = NewNickname;
	this->_Auth[NICK] = true;
	if (this->_Auth[PASSWD] == true && this->_Auth[USER] == true)
		this->_Auth[OK] = true;
}

void	Client::SetUsername(std::string const &NewUsername)
{
	this->_Username = NewUsername;
	this->_Auth[USER] = true;
	if (this->_Auth[PASSWD] == true && this->_Auth[NICK] == true)
		this->_Auth[OK] = true;
}

void	Client::SetSocket(int const &NewSocket)
{
	this->_Socket = NewSocket;
}

void	Client::SetPasswd(void)
{
	this->_Auth[PASSWD] = true;
}

void	Client::SetStatement(int Index, bool state)
{
	this->_Auth[Index] = state;
}

void	Client::SetInputBuffer(std::string const &Cmd)
{
	this->_InputBuffer += Cmd;
}

void	Client::ClearInputBuffer(void)
{
	this->_InputBuffer.clear();
}

int 	Client::GetSocket(void) const
{
	return (this->_Socket);
}

std::string 	Client::GetNickname(void) const
{
	return (this->_Nickname);
}

std::string 	Client::GetUsername(void) const
{
	return (this->_Username);
}

bool 	Client::GetStatement(void) const
{
	return (this->_Auth[OK]);
}

std::string	Client::GetInputBuffer(void) const
{
	return (this->_InputBuffer);
}

std::ostream&	operator<<(std::ostream& print, const Client& other)
{
	print << "Nickname : " << other.GetNickname() << std::endl;
	print << "Username : " << other.GetUsername() << std::endl;
	print << "Socket : " << other.GetSocket() << std::endl;
	print << "Statement : " << other.GetStatement() << std::endl;
	// print << "InputBuffer : " << other.GetInputBuffer() << std::endl;
	return (print);
}
