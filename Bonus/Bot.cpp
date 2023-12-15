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

void	Bot::start()
{
    struct sockaddr_in addr;

    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if (setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        close(this->_socket);
        return;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->_port);

    if (connect(this->_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(this->_socket);
        std::cerr << "Bot connect fail" << std::endl;
        return;
    }
    std::cout << "Bot connected" << std::endl;
    this->on();
}

void	Bot::on()
{
    char buffer[1024];

    std::string connexions = "PASS " + _password + "\r\nNICK bot\r\nUSER bot\r\nJOIN #bot\r\nTOPIC #bot :PING PONG\r\n";
    send(this->_socket, connexions.c_str(), connexions.size(), 0);

    int ret = recv(this->_socket, buffer, sizeof( buffer ) - 1, 0);
    buffer[ret] = '\0';

    std::string check( buffer );
    std::cout << "Check:" << check << "^" << std::endl;
    if (check.find(":bot JOIN #bot") == std::string::npos)
    {
        send(this->_socket, "QUIT\r\n", 6, 0);
        close(this->_socket);
        std::cerr << "Error from server" << std::endl;
        return;
    }

    while (run)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(this->_socket, &read_fds);

        int ready = select( this->_socket + 1, &read_fds, NULL, NULL, NULL );

        if ( ready > 0 && FD_ISSET( this->_socket, &read_fds ) )
        {
            int ret = recv(this->_socket, buffer, sizeof( buffer ) - 1, 0);

            if ( ret <= 0 )
                break;

            buffer[ret] = '\0';
            
            std::string received( buffer );
            std::cout << received << std::endl;

            if ( received.find( "ping" ) != std::string::npos )
                send( this->_socket, "PRIVMSG #bot :pong\r\n", 20, 0 );
            if ( received.find( "pong" ) != std::string::npos )
                send( this->_socket, "PRIVMSG #bot :ping\r\n", 20, 0 );
        }
        else if (ready < 0 && run) {
            std::cerr << "select error" << std::endl;
            break;
        }
    }
    send(this->_socket, "QUIT\r\n", 6, 0);
    close(this->_socket);
	puts( "\nStay here, I'll be back." );

}
