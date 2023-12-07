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

int	main(int ac, char **av)
{
	//Set singnal handler
	//PARSE USER ARGUMENTS
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
	catch (std::exception &e){
		std::cerr << "IrcServ: Error: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
