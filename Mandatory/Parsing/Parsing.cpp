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
} e_value;

static const	std::string Cmd[10] = 
{
	"TOPIC", "INVITE", "PRIVMSG", "KICK",
	"JOIN", "MODE", "WHO", "PASS", "NICK", "USER"
};

// ########################################################################## //
// #~TODO MANAGER~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# //
// Still to make :
// -> Management of NickName
// Error found :
//      SEGFAULT on empty data after all keyword
// ########################################################################## //

// ########################################################################## //
// #~TESTING TOOL~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# //
// static void
// printClient( const Client* person );
// static void
// print( std::string in, std::vector<std::string>& tab );

// TEST :
// PASS password
// NICK jimmy
// USER tdc
// ########################################################################## //

static void
dispatch( std::string& info, int& way, Client& person, Server& server );

static int
wayChooser( const std::string& target );

// ########################################################################## //
// #_PARSER COMMAND_________________________________________________________# //
int
handleCommand(Server& server, Client& person ) {
    std::string                 work(person.GetInputBuffer());
    std::vector<std::string>    tab;
    int                         way = -1;

    // TODO need a APPEND with previous cmd maybe manage here
	//
    tab = split( work, "\r\n" );
//	std::cout << "Cmd *-" << work << "-*" << std::endl;
    if ( tab.empty() )
        return ( way ) ;
    for ( size_t i = 0; i < tab.size(); i++ ) {
		way = wayChooser( tab[i] );
		if (way == -1) {
			person.SetMessageToSend(": 461 :Unknow command\r\n");
			continue ;
		}
		dispatch( tab[i], way, person, server );
    }
	person.ClearInputBuffer();
//	std::cout << "Reply\n*-" << person.GetMessage() << "-*" << std::endl;
    return ( way );
}

static void
dispatch( std::string& info, int& way, Client& person, Server& server ) {
    if ( !person.GetStatement() && !(way > 6 &&  way < 10) ){
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

    for ( int i = 0; i < 10; i++ ) {
        if ( Cmd[i].compare(0, Cmd[i].size(), tmp) == 0 ) {
            return ( i );
        }
    }
	return (-1);
}
