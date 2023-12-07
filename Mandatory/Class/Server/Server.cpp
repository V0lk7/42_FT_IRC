#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <unistd.h>
#include <algorithm>
#include <iostream>

Server::Server() : _Password( "password" ),
                   _MasterSocket( -1 )
{
    int i = 0;
    Client* one;
    Client* two;
    Client* three;
    Client* four;

    _ClientList.push_back( new Client ( "sudo",      SUDO ) );
    _ClientList.push_back( new Client ( "Jean",    CLIENT ) );
    _ClientList.push_back( new Client ( "Charles", CLIENT ) );
    _ClientList.push_back( new Client ( "Jimmy",   CLIENT ) );

	std::list<Client *>::iterator	ItClientBegin = this->_ClientList.begin();
	std::list<Client *>::iterator	ItEnd = this->_ClientList.end();
	while ( ItClientBegin != ItEnd )
	{
        switch (i) {
            case 0:
                one = *ItClientBegin;
                break;
            case 1:
                two = *ItClientBegin;
                break;
            case 2:
                three= *ItClientBegin;
                break;
            default:
                four = *ItClientBegin;
                break;
        }
		ItClientBegin++;
        i++;
	}

    _ChannelList.push_back( new Channel ( *one, *two, *three, *four ) );

}

Server::Server(std::string const &Passwd) : _Password(Passwd), _MasterSocket(-1) {}

Server::~Server()
{
	if (this->_MasterSocket > 0)
		close(this->_MasterSocket);
	std::list<Channel *>::iterator	ItChannelBegin = this->_ChannelList.begin();
	std::list<Channel *>::iterator	ItChannelEnd = this->_ChannelList.end();
	while (ItChannelBegin != ItChannelEnd)
	{
		delete *ItChannelBegin;
		ItChannelBegin++;
	}

	std::list<Client *>::iterator	ItClientBegin = this->_ClientList.begin();
	std::list<Client *>::iterator	ItEnd = this->_ClientList.end();
	while (ItClientBegin != ItEnd)
	{
		delete *ItClientBegin;
		ItClientBegin++;
	}
}

Server::Server(Server const &src) {*this = src;}

Server	&Server::operator=(Server const &rhs) {(void)rhs; return (*this);}

//----------------Set/Get-------------------//

std::list<Client *>&
Server::getCllist( void ) {
    return ( _ClientList );
}

void	Server::SetPassword(std::string const &NewPassword)
{
	this->_Password = NewPassword;
}

void	Server::SetTcpConfig(uint16_t const &Port)
{
	_TcpConfig.sin_family = AF_INET; 
	_TcpConfig.sin_addr.s_addr = INADDR_ANY; 
	_TcpConfig.sin_port = htons(Port);
}

void	Server::SetMasterSocket(int const &NewMasterSocket)
{
	this->_MasterSocket = NewMasterSocket;
}

std::string	Server::GetChannelList(void) const
{
	std::string	ChannelList;
	std::list<Channel *>::const_iterator	ItBegin = this->_ChannelList.begin();
	std::list<Channel *>::const_iterator	ItEnd = this->_ChannelList.end();
	for(int i = 0; ItBegin != ItEnd; i++, ItBegin++)
	{
		ChannelList += i;
		ChannelList += " ";
		ChannelList += (*ItBegin)->GetName();
		ChannelList += "\n";
	}
	return (ChannelList);
}

std::string	Server::GetClientList(void) const
{
	std::string	ClientList;
	std::list<Client *>::const_iterator	ItBegin = this->_ClientList.begin();
	std::list<Client *>::const_iterator	ItEnd = this->_ClientList.end();
	for(int i = 0; ItBegin != ItEnd; i++, ItBegin++)
	{
		ClientList += i;
		ClientList += " ";
		ClientList += (*ItBegin)->GetNickname();
		ClientList += "\n";
	}
	return (ClientList);
}

std::string	Server::GetPassword(void) const
{
	return (this->_Password);
}

int	Server::GetMasterSocket(void) const
{
	return (this->_MasterSocket);
}

struct sockaddr_in	Server::GetTcpConfig(void) const
{
	return (this->_TcpConfig);
}

Channel	*Server::GetChannel(std::string const &Name) const
{
	std::list<Channel *>::const_iterator	ItBegin = this->_ChannelList.begin();
	std::list<Channel *>::const_iterator	ItEnd = this->_ChannelList.end();
	while (ItBegin != ItEnd)
	{
		if ((*ItBegin)->GetName() == Name)
			return (*ItBegin);
		ItBegin++;
	}
	return (NULL);
}

Client	*Server::GetClient(std::string const &Nickname) const
{
	std::list<Client *>::const_iterator	ItBegin = this->_ClientList.begin();
	std::list<Client *>::const_iterator	ItEnd = this->_ClientList.end();
	while (ItBegin != ItEnd)
	{
		if ((*ItBegin)->GetNickname() == Nickname)
			return (*ItBegin);
		ItBegin++;
	}
	return (NULL);
}

/*----------------------SpecificMethods----------------------------*/

void	Server::AddChannel(Channel *NewChannel)
{
	this->_ChannelList.push_back(NewChannel);
}

void	Server::AddClient(Client *NewClient)
{
	this->_ClientList.push_back(NewClient);
}

bool	Server::SendReply(void)
{
	std::list<Client *>::iterator	It = _ClientList.begin();
	std::string						Msg;

	while (It != _ClientList.end()){
		Msg = (*It)->GetMessage();
		if (Msg.empty() != true){
			if (send((*It)->GetSocket(), Msg.c_str(), Msg.size(), 0) == -1)
				return (false);
			(*It)->ClearMessage();
		}
		It++;
	}
	return (true);
}

void	Server::DisconnectClient(Client &client)
{
	std::list<Channel *>::iterator	It = _ChannelList.begin();
	std::string						Reply;

	Reply = ":" + client.GetNickname() + " QUIT : Disconnected.\r\n";
	while (It != _ChannelList.end()){
		if ((*It)->UserInChannel(client) == true){
			(*It)->SendMessageToClients(Reply, client);
			if ((*It)->UserInWaitingList(client) == true){
				(*It)->EraseClientFromWaitingList(client);	
			}
			(*It)->EraseClientFromChannel(client);
		}
		It++;
	}
	close(client.GetSocket());
	client.SetSocket(-1);
}

void	Server::ClearClients(void)
{
	std::list<Client *>::iterator	It;

	while (true){
		It = _ClientList.begin();
		if ((*It)->GetSocket() == -1){
			delete *It;
			_ClientList.erase(It);
			continue ;
		}
		It++;
		if (It == _ClientList.end())
			return ;
	}
}

std::ostream&	operator<<(std::ostream& print, const Server& other)
{
	print << "Server password : " << other.GetPassword() << std::endl;
	print << "Master socket : " << other.GetMasterSocket() << std::endl;
	print << "Channel list : " << other.GetChannelList() << std::endl;
	print << "Client list : " << other.GetClientList() << std::endl;
	return (print);
}
