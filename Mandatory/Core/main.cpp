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

#include "Core.hpp"
#include "Server.hpp"
#include <iostream>
#include <signal.h>

#define EXIT 1

void	ExitHandlerSignal(int Signal);

int	main(int ac, char **av)
{
	if (signal(SIGINT, ExitHandlerSignal) == SIG_ERR){
		std::cerr << "IrcServer: Error signal function" << std::endl;
		return (1);
	}
	ErrArgs	Error = ParseArguments(ac, av);
	if (Error != NONE){
		ErrorArguments(Error);
		return (1);
	}
	Server	ServerData(av[2]);
	if (InitializeMasterSocket(ServerData, av[1]) != true)
		return (1);
	try {
		ProcessServer(ServerData);
	}
	catch (int exit){
		std::cerr << "IrcServ: Server disconnected." << std::endl;
	}
	catch (std::exception &e){
		std::cerr << "IrcServ: Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}

void	ExitHandlerSignal(int Signal)
{
	if (Signal == SIGINT)
		throw EXIT;
}
