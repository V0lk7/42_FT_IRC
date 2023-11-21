#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "parsing.hpp"

void	privateMessage(Server &server, Client &client, serverstd::string &rawCommand){

    std::vector<std::string> splitCommand = Parsing::split(rawCommand, ' ');
    std::erase(splitCommand.begin());
    std::string::iterator it = splitCommand.begin();
    std::cout << it << std::endl;
    if(it == '#'){
        //je suis perdu...
    }
}