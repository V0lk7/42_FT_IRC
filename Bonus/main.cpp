#include "Bot.hpp"

bool	run = true;

void	ExitHandlerSignal(int Signal)
{
	if( Signal == SIGINT)
	{
		run = false;
		puts( "\nStay here, I'll be back." );
	}
}

int	main(int ac, char **av)
{
	if( ac != 3 || atoi(av[1]) <= 0 )
	{
		std::cout << "Usage: ./Bot <port> <password>" << std::endl;
		return (1);
	}
	signal(SIGINT, ExitHandlerSignal);
	Bot	bot( atoi(av[1]), av[2]);
	bot.start();
	return (0);
}
