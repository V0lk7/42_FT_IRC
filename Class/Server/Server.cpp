/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 14:20:18 by jduval            #+#    #+#             */
/*   Updated: 2023/11/03 14:33:30 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

Channel::Channel(){} 

Channel::Channel(std::string const &Passwd) : _Password(Passwd) 

Channel::~Channel()
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

Channel::Channel(Channel const &src) {*this = src;}

Channel	&Channel::operator=(Channel const &rhs) {(void)rhs;}

//----------------Set/Get-------------------//
