#include "Bot.hpp"

int	main(int ac, char **av)
{
	if ( ac != 3 || atoi(av[1]) <= 0 )
	{
		std::cout << "Usage: ./Bot <port> <password>" << std::endl;
		return (1);
	}
	Bot	bot( atoi(av[1]), av[2]);
	bot.run();
	return (0);
}