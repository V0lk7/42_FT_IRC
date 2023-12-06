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
#include "Core.hpp"

static int	SetListSd(	std::list<Client *> &ClientList,
						fd_set &ListSd, int MasterSocket);

static void	ProcessMasterSocket(Server &ServerData, fd_set &ListSd, int MasterSocket);

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
		ProcessClientsSocket(ServerData, ListSd, MasterSocket);
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
		Sd = It->GetSocket();
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

	SockAddr	Addr		= static_cast<SockAddr>(Server.GetTcpConfig());
	socklen_t	AddrSize	= static_cast<socklen_t>(sizeof(Addr));
	int	NewSocket			= accept(MasterSocket, &Addr, &AddrSize);

	if (NewSocket == -1)
		throw std::runtime_error("Error on accept system call");

	Client	*NewClient = new Client();
	NewClient->SetSocket(NewSocket);
	ServerData.AddClient(NewClient);
	return ;
}

static void	ProcessClientsSocket(Server &ServerData, fd_set &ListSd)
{
	std::list<Client *>				&ClientList = ServerData.getCllist();
	std::list<Client *>::iterator	It = ClientList.begin();
	std::list<Client *>::iterator	ItEnd = ClientList.end();
	std::string						Buffer;
	int								Sd;

	while (It != ItEnd){
		Sd = It->GetSocket();
		if (FD_ISSET(Sd, &ListSd) != 0){
			buffer = ReadData(Sd, BUFFER_SIZE);
			It->SetInputBuffer(buffer);
		}
		It++;
	}
}
