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

# define BACKLOG 4096		//see /proc/sys/net/core/somaxconn
# define BUFFER_SIZE 512	//RFC 1459 section 8.2 

typedef struct sockaddr_in SockAddrIn;
typedef struct sockaddr SockAddr;

typedef enum ErrArgs {
	NONE,
	WRONG_PARAMETERS,
	WRONG_PORT,
	WRONG_PASSWORD
} ErrArgs;

class Server;

ErrArgs	ParseArguments(int ac, char **av);
void	ErrorArguments(ErrArgs flag);

bool	InitializeMasterSocket(Server &ServerData, char const *Port);

void	ProcessServer(Server &ServerData);

#endif
