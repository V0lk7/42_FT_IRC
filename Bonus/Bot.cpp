#include "Bot.hpp"

Bot::Bot() {}
Bot::~Bot() {}

Bot::Bot( int port ) : _port(port) {}

Bot::Bot( Bot const &src )
{
	*this = src;
}

Bot	&Bot::operator=( Bot const &rhs )
{
	if (this != &rhs)
	{
		this->_port = rhs._port;
	}
	return *this;
}

void	Bot::on()
{
	std::string msg = "PRIVMSG #test :Hello World\r\n";
	std::cout << "Bot on" << std::endl;
	while (1)
	{
		send(this->_socket, msg.c_str(), msg.size(), 0);
		sleep(1);
	}
}

void	Bot::run()
{
    struct sockaddr_in addr;

    // création socket du bot
    this->_socket = socket(PF_INET, SOCK_STREAM, 0);
    // parametrage de l'adresse à laquelle le bot va se lier
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->_port);
    // on cast parce que connect attend un sockaddr et pas un addr
    if (connect(this->_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        std::cerr << "bot connect fail" << std::endl;
        return;
    }
    std::cout << "Bot start" << std::endl;
    this->on();
}
