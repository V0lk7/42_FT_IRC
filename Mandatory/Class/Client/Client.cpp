#include "Client.hpp"
#include <iostream>
#include <unistd.h>

Client::Client() : _Socket(-1), _Nickname(""), _Username("")
{
	for (int i = 0; i < 4; i++)
		this->_Auth[i] = false;
}

Client::Client(Client const &src) {*this = src;}

Client& Client::operator=( const Client& rhs )
{
    if ( this == &rhs )
		return ( *this );
    _Socket         = rhs._Socket;
    _Nickname       = rhs._Nickname;
    _Username       = rhs._Username;
    _Auth[ OK ]     = rhs._Auth[ OK ];
    _Auth[ PASSWD ] = rhs._Auth[ PASSWD ];
    _Auth[ NICK ]   = rhs._Auth[ NICK ];
    _Auth[ USER ]   = rhs._Auth[ USER ];
    _InputBuffer    = rhs._InputBuffer;
    _MessageToSend  = rhs._MessageToSend;
    return ( *this );
}

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
}

void	Client::SetUsername(std::string const &NewUsername)
{
	this->_Username = NewUsername;
	this->_Auth[USER] = true;
}

void	Client::SetSocket(int const &NewSocket)
{
	this->_Socket = NewSocket;
}

void	Client::SetPasswd(void)
{
	_Auth[ PASSWD ] = true;
}

bool
Client::GetStatementStep( step target ) const {
    return ( _Auth[ target ] );
}

std::string	Client::GetMessage(void) const
{
	return (this->_MessageToSend);
}

void	Client::SetStatement(int Index, bool state)
{
	this->_Auth[Index] = state;
}

void	Client::SetInputBuffer(std::string const &Cmd)
{
	this->_InputBuffer += Cmd;
}

void	Client::SetMessageToSend(std::string const &Msg)
{
	this->_MessageToSend += Msg;
}

void	Client::ClearInputBuffer(void)
{
	this->_InputBuffer.clear();
}

void	Client::ClearMessage(void)
{
	this->_MessageToSend.clear();
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

bool	Client::IsAuthenticate(void) const
{
	for (int i = 1; i < 4; i++){
		if (this->_Auth[i] == false)
			return (false);
	}
	return (true);
}

std::ostream&	operator<<(std::ostream& print, const Client& other)
{
	print << "Nickname : " << other.GetNickname() << std::endl;
	print << "Username : " << other.GetUsername() << std::endl;
	print << "Socket : " << other.GetSocket() << std::endl;
	print << "Statement ok : " << other.GetStatement() << std::endl;
    print << "PASSWD : " << other.GetStatementStep( PASSWD ) << std::endl;
    print << "NICK : " << other.GetStatementStep( NICK ) << std::endl;
    print << "USER : " << other.GetStatementStep( USER ) << std::endl;
	// print << "InputBuffer : " << other.GetInputBuffer() << std::endl;
	return (print);
}
