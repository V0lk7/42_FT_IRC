#include "Bot.hpp"

Bot::Bot() {}
Bot::~Bot() {}

Bot::Bot( int port, std::string const &pass) : _port(port), _password(pass){}

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
	std::string pass = "PASS " + _password + "\r\n";

	std::string nick = "NICK Sarah\r\n";

	std::string user = "USER StupidBot\r\n";

	std::string channel = "JOIN #Bot\r\n";

    send(this->_socket, pass.c_str(), pass.size(), 0);
    send(this->_socket, nick.c_str(), nick.size(), 0);
    send(this->_socket, user.c_str(), user.size(), 0);
    usleep(100000);
    send(this->_socket, channel.c_str(), channel.size(), 0);
	puts( "Bot on" );

	std::string fishing = "PRIVMSG #Bot :Hello, I'm Sarah and I'm here to help you. Ask me any question and I'll answer it.\r\n";

    while (run)
    {
        send(this->_socket, fishing.c_str(), fishing.size(), 0);
        usleep(10000000);
        // char buffer[1024];
        // int ret = recv(this->_socket, buffer, 1023, 0);
        // if (ret <= 0)
        //     break;
        // buffer[ret] = 0;
        // std::cout << buffer;
        // if (std::string(buffer).find("PING") == 0)
        // {
        //     std::string pong = "PONG " + std::string(buffer).substr(5) + "\r\n";
        //     send(this->_socket, pong.c_str(), pong.size(), 0);
        // }
    }
	// std::string nop = "PRIVMSG #CutestBot :I didn't understand your question, can you rephrase it?\r\n";
}

void	Bot::start()
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
