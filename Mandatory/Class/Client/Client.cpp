/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:05:18 by jduval            #+#    #+#             */
/*   Updated: 2023/11/03 14:15:21 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _Socket(-1), _Nickname(""), _Username("")
{
	for (int i = 0; i < 4; i++)
		this->_Auth[i] = false;
}

Client::Client(Client const &src) {*this = src;}

Client	&Client::operator=(Client const &rhs) {(void)rhs;}

Client::~Client()
{
	if (this->_Socket >= 0)
		close (this->_Socket);
}

//----------------Set/Get-------------------//

void	Client::SetNickname(std::string const &NewNickname)
{
	this->_Nickname = NewNickname;
	this->_Auth[NICK] = true;
	if (this->Auth[PASSWD] == true && this->_Auth[USER] == true)
		this->_Auth[OK] = true;
}

void	Client::SetUsername(std::string const &NewUsername)
{
	this->_Username = NewUsername;
	this->_Auth[USER] = true;
	if (this->Auth[PASSWD] == true && this->_Auth[NICK] == true)
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

int const	&Client::GetSocket(void) const
{
	return (this->_Socket);
}

std::string const	&Client::GetNickname(void) const
{
	return (this->_Nickname);
}

std::string const	&Client::GetUsername(void) const
{
	return (this->_Username);
}

bool const	&Client::GetStatement(void) const
{
	return (this->_Auth[OK]);
}
