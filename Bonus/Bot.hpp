#pragma once

# include <string>
# include <list>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <signal.h>
# include <cstdio>
# include <cstdlib>

extern bool run;

class Bot
{
	private :

		int			_port;
		int			_socket;
		std::string	_password;

	public :

		Bot();
		Bot( int port, std::string const &password);
		~Bot();
		Bot( Bot const &src );

		Bot	&operator=( Bot const &rhs );

		void	start();
		void	on();
};
