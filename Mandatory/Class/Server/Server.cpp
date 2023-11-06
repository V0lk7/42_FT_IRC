/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 14:20:18 by jduval            #+#    #+#             */
/*   Updated: 2023/11/06 15:22:37 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Server::Server(){} 

Server::Server(std::string const &Passwd) : _Password(Passwd) 

Server::~Server()
{
	std::close(this->_MasterSocket);
	std::list<Channel *>::iterator	ItBegin = this->_ChannelList.begin();
	std::list<Channel *>::iterator	ItEnd = this->_ChannelList.end();
	while (ItBegin != ItEnd)
	{
		delete *ItBegin;
		ItBegin++;
	}

	std::list<Client *>::iterator	ItBegin = this->_ClientList.begin();
	std::list<Client *>::iterator	ItTmp;
	std::list<Client *>::iterator	ItEnd = this->_ClientList.end();
	while (ItBegin != ItEnd)
	{
		delete *ItBegin;
		ItBegin++;
	}
}

Server::Server(Server const &src) {*this = src;}

Server	&Server::operator=(Server const &rhs) {(void)rhs;}

//----------------Set/Get-------------------//


