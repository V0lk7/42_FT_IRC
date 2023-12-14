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
    std::string connexions = "PASS " + _password + "\r\nNICK bot\r\nUSER bot\r\nJOIN #bot\r\n";
    send(this->_socket, connexions.c_str(), connexions.size(), 0);


while (run) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(this->_socket, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 400000;

    int ready = select(this->_socket + 1, &read_fds, NULL, NULL, &timeout);

    if (ready > 0 && FD_ISSET(this->_socket, &read_fds)) {
        char buffer[1024];
        int ret = recv(this->_socket, buffer, sizeof(buffer) - 1, 0);
        if (ret <= 0) {
            break;
        }
        buffer[ret] = '\0';
        std::cout << buffer << std::endl;
        
        std::string received(buffer);

        if (received.find("ping") != std::string::npos) {
            usleep(100000);
            puts("pong");
            send(this->_socket, "PRIVMSG #bot :pong\r\n", 20, 0);
        }
        if (received.find("pong") != std::string::npos) {
            usleep(100000);
            puts("ping");
            send(this->_socket, "PRIVMSG #bot :ping\r\n", 20, 0);
        }
    } 
    // else if (ready == 0) {
    //     // Aucune donnée disponible dans le délai spécifié, vous pouvez effectuer d'autres actions ici si nécessaire
    // } else {
    //     // Gérer l'erreur de select() ici
    // }
}

send(this->_socket, "QUIT\r\n", 6, 0);




    // while (run)
    // {
    //     char buffer[1024];
    //     int ret = recv(this->_socket, buffer, 1023, 0);
    //     if (ret <= 0)
    //         break;
    //     buffer[ret] = 0;
    //     std::cout << buffer << std::endl;
    //     if (std::string(buffer).find("ping") != std::string::npos)
    //     {
    //         usleep(400000);
    //         puts( "pong" );
    //         std::string pong = "PRIVMSG #bot :pong\r\n";
    //         std::cout << pong.size() << std::endl;
    //         send(this->_socket, pong.c_str(), pong.size(), 0);
    //     }
    //     if (std::string(buffer).find("pong") != std::string::npos)
    //     {
    //         usleep(400000);
    //         puts( "ping" );
    //         std::string ping = "PRIVMSG #bot :ping\r\n";
    //         send(this->_socket, ping.c_str(), ping.size(), 0);
    //     }
    // }
    // send( this->_socket, "QUIT\r\n", 6, 0 );
}

void	Bot::start()
{
    struct sockaddr_in addr;

    this->_socket = socket(PF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->_port);
    if (connect(this->_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        std::cerr << "Bot connect fail" << std::endl;
        return;
    }
    std::cout << "Bot connected" << std::endl;
    this->on();
}
