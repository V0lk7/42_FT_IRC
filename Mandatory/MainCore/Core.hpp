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

#ifndef CORE_HPP
# define CORE_HPP

# define BACKLOG 4096
# define BUFFER_SIZE 

typedef struct sockaddr_in SockAdrrIn;
typedef struct sockaddr SockAdrr;

typedef enum ErrArgs {
	NONE,
	WRONG_PARAMETERS,
	WRONG_PORT
} ErrArgs;

class Server;

ErrArgs	ParseArguments(int ac, char **av);
void	ErrorArguments(ErrArgs flag);
bool	InitializeMasterSocket(Server &ServerData);

#endif
