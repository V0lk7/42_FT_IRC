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
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cerrno>
#include <iostream>

static bool SetupTcpSocket(Server &ServerData, int MasterSocket, char const *Port);

bool	InitializeMasterSocket(Server &ServerData, char const *Port)
{
	std::cout << "Create the master socket" << std::endl;
	int MasterSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (MasterSocket == -1){
		std::cerr << "Error: " << errno << " : Socket creation" << std::endl;
		return (-1);
	}
    ServerData.SetMasterSocket(MasterSocket);
//	Usefull ?
//	if (setsockopt(server->GetMasterSocket(), SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
//		sizeof(opt)) < 0)
//
	if (SetupTcpSocket(ServerData, MasterSocket, Port) == false)
		return (false);
	return (true);
}

static bool SetupTcpSocket(Server &ServerData, int MasterSocket, char const *Port)
{
	ServerData.SetTcpConfig(static_cast<uint16_t>(atoi(Port)));

	SockAddrIn	Address = ServerData.GetTcpConfig();

	std::cout << "Binding the socket" << std::endl;
	if (bind(MasterSocket, (SockAddr *)&Address, sizeof(Address)) == -1){
		std::cerr << "Error: " << errno << " : bind function" << std::endl;
		return (false);
	}
	std::cout << "Put the socket in listen mode" << std::endl;
	if (listen(MasterSocket, BACKLOG) == -1){
		std::cerr << "Error: " << errno << " : listen function" << std::endl;
		return (false);
	}
	return (true);
}
