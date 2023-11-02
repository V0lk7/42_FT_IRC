/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:05:18 by jduval            #+#    #+#             */
/*   Updated: 2023/11/02 15:41:02 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : _ValidClient(0), _Socket(-1), _Nickname(""), _Username("") {}

Client::Client(int const &NewSock, std::string const &NewNick, std::string const &NewUser)
				: _Socket(NewSock), _Nickname(NewNick), _Username(NewUser), _ValidClient(3) {}

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
}

void	Client::SetUsername(std::string const &NewUsername)
{
	this->_Username = NewUsername;
}

void	Client::SetSocket(int const &NewSocket)
{
	this->_Socket = NewSocket;
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

int const	&Client::GetValidClient(void) const
{
	return (this->_ValidClient);
}
