/*============================================================================*/
/*            _       _                     _          _            _         */
/*           /\ \    /\ \                  /\ \       /\ \        /\ \        */
/*          /  \ \   \_\ \                 \ \ \     /  \ \      /  \ \       */
/*         / /\ \ \  /\__ \                /\ \_\   / /\ \ \    / /\ \ \      */
/*        / / /\ \_\/ /_ \ \     ____     / /\/_/  / / /\ \_\  / / /\ \ \     */
/*       / /_/_ \/_/ / /\ \ \  /\____/\  / / /    / / /_/ / / / / /  \ \_\    */
/*      / /____/\ / / /  \/_/  \/____\/ / / /    / / /__\/ / / / /    \/_/    */
/*     / /\____\// / /                 / / /    / / /_____/ / / /             */
/*    / / /     / / /              ___/ / /__  / / /\ \ \  / / /________      */
/*   / / /     /_/ /              /\__\/_/___\/ / /  \ \ \/ / /_________\     */
/*   \/_/      \_\/               \/_________/\/_/    \_\/\/____________/     */
/*																			  */
/*============================================================================*/

#include "Server.hpp"
#include "Client.hpp"
#include "Core.hpp"
#include "Parsing.hpp"
#include <cstring>

static int	SetListSd(	std::list<Client *> &ClientList,
						fd_set &ListSd, int MasterSocket);

static void	ProcessMasterSocket(Server &ServerData, fd_set &ListSd, int MasterSocket);
static void	ProcessClientSocket(Server &ServerData, fd_set &ListSd);

void	ProcessServer(Server &ServerData)
{
	fd_set	ListSd;
	int		MaxSd; //Sd = Socket descriptor
	int		MasterSocket = ServerData.GetMasterSocket();
	
	while (true){
		MaxSd = SetListSd(ServerData.getCllist(), ListSd, MasterSocket);
		if (select(MaxSd + 1, &ListSd, NULL, NULL, NULL) == -1)
			throw std::runtime_error("Error on select function");
		ProcessMasterSocket(ServerData, ListSd, MasterSocket);
		ProcessClientSocket(ServerData, ListSd);
		if (ServerData.SendReply() != true)
			throw std::runtime_error("Error send system call");
	}
}

static int	SetListSd(	std::list<Client *> &ClientList,
						fd_set &ListSd, int MasterSocket)
{
	std::list<Client *>::iterator	It = ClientList.begin();
	std::list<Client *>::iterator	ItEnd = ClientList.end();
	int	Sd;
	int	MaxSd = MasterSocket;

	FD_ZERO(&ListSd);
	FD_SET(MasterSocket, &ListSd);
	while (It != ItEnd){
		Sd = (*It)->GetSocket();
		if (Sd > 0)
			FD_SET(Sd, &ListSd);
		if (Sd > MasterSocket)
			MaxSd = Sd;
		It++;
	}
	return (MaxSd);
}

static void	ProcessMasterSocket(Server &ServerData, fd_set &ListSd, int MasterSocket)
{
	if (FD_ISSET(MasterSocket, &ListSd) == 0)
		return ;

	SockAddrIn	Addr		= ServerData.GetTcpConfig();
	socklen_t	AddrSize	= static_cast<socklen_t>(sizeof(Addr));
	int			NewSocket	= accept(MasterSocket, (sockaddr *)&Addr, &AddrSize);

	if (NewSocket == -1)
		throw std::runtime_error("Error on accept system call");
	Client	*NewClient = new Client();
	NewClient->SetSocket(NewSocket);
	ServerData.AddClient(NewClient);
	return ;
}

static void	ProcessClientSocket(Server &ServerData, fd_set &ListSd)
{
	std::list<Client *>				&ClientList = ServerData.getCllist();
	std::list<Client *>::iterator	It = ClientList.begin();
	char							Buffer[BUFFER_SIZE];
	int								Sd, flag;

	while (It != ClientList.end()){
		Sd = (*It)->GetSocket();
		if (FD_ISSET(Sd, &ListSd) != 0){
			bzero(Buffer, BUFFER_SIZE);
			flag = recv(Sd, Buffer, BUFFER_SIZE, 0);
			if (flag == -1)
				throw std::runtime_error("Error on read system call");
			else if (flag == 0)
				ServerData.DisconnectClient(**It);
			else {
				(*It)->SetInputBuffer(Buffer);
				handleCommand(ServerData, *(*It));
			}
		}
		It++;
	}
	ServerData.EraseClientDisconnected();
}
