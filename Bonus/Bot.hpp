#pragma once

# include <string>
# include <list>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>

class Bot
{
	private :

		int	_port;
		int	_socket;

	public :

		Bot();
		Bot( int port );
		~Bot();
		Bot( Bot const &src );

		Bot	&operator=( Bot const &rhs );

		void	run();
		void	on();
};
