#include "Bot.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Usage: ./Bot <port>" << std::endl;
		return (1);
	}
	Bot	bot(atoi(av[1]));
	bot.run();
	return (0);
}