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
#include <string>
#include <iostream>
#include <cstdlib>
#include <cerrno>

ErrArgs	ParseArguments(int ac, char **av)
{
	if (ac != 3)
		return (WRONG_PARAMETERS);

	std::string	Ports(av[1]);
	std::string	Password(av[2]);

	if (Password.empty() == true)
		return (WRONG_PASSWORD);
	if (Ports.find_first_not_of("0123456789") != std::string::npos)
		return (WRONG_PORT);
	double	Port = strtod(av[1], NULL);
	if (errno == ERANGE || !(Port >= 1024.0 && Port <= 655335)) //Range port available
		return (WRONG_PORT);									//See RFC 1340, page 23.
	return (NONE);
}

void	ErrorArguments(ErrArgs flag)
{
	if (flag == WRONG_PARAMETERS)
		std::cerr << "Usage: ircserv \"Port\" \"Password\"";
	else if (flag == WRONG_PORT)
		std::cerr << "Error: Port number invalid";
	else
		std::cerr << "Error: Need a non empty password";
	std::cerr << std::endl;
	return ;
}
