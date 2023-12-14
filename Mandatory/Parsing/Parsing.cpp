#include "Tools.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"
#include "Authentication.hpp"

#include <iostream>
#include <vector>

typedef enum e_value {
     TOPIC,
     INVITE,
     PRIVMSG,
     KICK,
     JOIN,
     MODE,
	 WHO,
     sPASS,
     sNICK,
     sUSER,
	 QUIT
} e_value;

static const	std::string Cmd[11] = 
{
	"TOPIC", "INVITE", "PRIVMSG", "KICK",
	"JOIN", "MODE", "WHO", "PASS", "NICK", "USER", "QUIT"
};

static void
dispatch( std::string& info, int& way, Client& person, Server& server );

static int
wayChooser( const std::string& target );

static std::string	SaveIncompleteCommand(std::string &Cmd);

// ########################################################################## //
// #_PARSER COMMAND_________________________________________________________# //
void
handleCommand(Server& server, Client& person ) {
    std::string                 work(person.GetInputBuffer());
	std::string					Backup(SaveIncompleteCommand(work));
    std::vector<std::string>    tab;
    int                         way;

    tab = split( work, "\r\n" );

	std::cout << "\tCmd\n*-" << work << "-*" << std::endl;
	std::cout << "Size list before " << server.getCllist().size() << std::endl; //DEBUG
	std::cout << "Backup *-" << Backup << "-*" << std::endl; //DEBUG
	std::cout << "Cmd *-" << work << "-*" << std::endl; //DEBUG

    if ( tab.empty() )
        return ;
    for ( size_t i = 0; i < tab.size(); i++ ) {
		way = wayChooser( tab[i] );
		if (way == -1) {
			person.SetMessageToSend(": 461 : Unknow command\r\n");
			continue ;
		}
		dispatch( tab[i], way, person, server );
    }

	person.ClearInputBuffer();

	std::cout << "\tReply\n*-" << person.GetMessage() << "-*" << std::endl;

	person.SetInputBuffer(Backup);

	std::cout << "Size list after " << server.getCllist().size() << std::endl; //DEBUG

    return ;
}

static std::string	SaveIncompleteCommand(std::string &Cmd)
{
	std::string	Backup;
	size_t	pos = Cmd.rfind("\n");

	if (pos == std::string::npos){
		pos = 0;
		Backup = Cmd;
	}
	else if (pos == Cmd.size() - 1)
		Backup = "";
	else
		Backup = Cmd.substr(pos + 1);
	Cmd.erase(pos);
	return (Backup);
}

static void
dispatch( std::string& info, int& way, Client& person, Server& server ) {
	if (way == QUIT)
		server.DisconnectClient(person);
	else if ( !person.GetStatement() && !(way > 6 &&  way < 10) ){
		person.SetMessageToSend(": 451 :Not registered\r\n");
		return ;
	}
    switch ( way ) {
        case TOPIC :
            Topic( server, person, info );
            break ;
        case INVITE :
			Invite( server, person, info );
            break ;
        case PRIVMSG :
            PrivateMessage( server, person, info );
            break ;
        case KICK :
            Kick ( server, person, info );
            break ;
        case JOIN :
            Join( server, person, info );
            break ;
        case MODE :
            Mode( server, person, info );
            break ;
		case WHO :
            Who( server, person, info );
            break ;
		case sPASS :
            Pass( server, person, info );
            break ;
		case sNICK :
            Nick( server, person, info );
            break ;
		case sUSER :
            User( person, info );
            break ;
        default :
            return ;
    }
}

static int
wayChooser( const std::string& target ) {
	std::string	tmp(target.substr(0, target.find_first_of(" ")));

    for ( int i = 0; i < 11; i++ ) {
        if ( Cmd[i].compare(0, Cmd[i].size(), tmp) == 0 ) {
            return ( i );
        }
    }
	return (-1);
}
