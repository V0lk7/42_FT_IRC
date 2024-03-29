#include "Tools.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Commands.hpp"
#include "Authentication.hpp"

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

static void	SendGreatingMessage(Client &client);

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

    if ( tab.empty() )
        return ;
    for ( size_t i = 0; i < tab.size(); i++ ) {
		way = wayChooser( tab[i] );
		if (way == -1) {
			person.SetMessageToSend(": 421 : " + tab[i] + " :Unknow command\r\n");
			continue ;
		}
		dispatch( tab[i], way, person, server );
    }
	person.ClearInputBuffer();
	person.SetInputBuffer(Backup);
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
	if (way == QUIT){
		server.DisconnectClient(person);
		return ;
	}
	if (person.GetStatement() != true && !(way > 6 && way < 10)){
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
			if (person.GetStatement() == false && person.IsAuthenticate() == true){
				person.SetStatement(OK, true);
				SendGreatingMessage(person);
			}
            break ;
		case sUSER :
            User( person, info );
			if (person.GetStatement() == false && person.IsAuthenticate() == true){
				person.SetStatement(OK, true);
				SendGreatingMessage(person);
			}
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

static void	SendGreatingMessage(Client &client)
{
	std::string	Reply;

	Reply = ": 001 " + client.GetNickname() + " :Welcome to 42 Irc Network\r\n";
	Reply += ": 005 " + client.GetNickname() + " NETWORK=Irc42_JCJ\r\n";
	client.SetMessageToSend(Reply);
}
