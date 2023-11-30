#include "doctest.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

#include "../../../Mandatory/Commands/PRIVMSG/Privmsg.cpp"

TEST_CASE ( "privateMessage" )
{
	Server		server;
	Client*		Jean = server.GetClient( "Jean" );
	Client*		Charles = server.GetClient( "Charles" );
	Channel*	channel = server.GetChannel( "#Test" );
	
	SUBCASE ( "PRIVMSG Charles : Unmot" )
	{
		std::string	str( "PRIVMSG Charles : Unmot" );
		privateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == " Unmot");
	}

	SUBCASE ( "PRIVMSG Charles : Deux mots" )
	{
		std::string	str( "PRIVMSG Charles : Deux mots" );
		privateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == " Deux mots");
	}

	SUBCASE ( "PRIVMSG Charles : Beaucoup                 d'espaces" )
	{
		std::string	str( "PRIVMSG Charles : Beaucoup                 d'espaces" );
		privateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == " Beaucoup                 d'espaces");
	}

	SUBCASE ( "PRIVMSG Charles :Ch@r@ct3r&s @ |a c0n !<>,./$%^*()_+*:" )
	{
		std::string	str( "PRIVMSG Charles :Ch@r@ct3r&s @ |a c0n !<>,./$%^*()_+*:" );
		privateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == "Ch@r@ct3r&s @ |a c0n !<>,./$%^*()_+*:");
	}

	SUBCASE ( "PRIVMSGCharles :Commande colle au user" )
	{
		std::string	str( "PRIVMSGCharles :Commande colle au user" );
		privateMessage( server, *Jean, str );
		std::string test = (*Jean).GetMessage();
		CHECK( (*Jean).GetMessage() == "ERROR : no recipients" );
	}

	SUBCASE ( "PRIVMSG Charles:Deux points colle au user" )
	{
		std::string	str( "PRIVMSG Charles:Deux points colle au user" );
		privateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == "Deux points colle au user");
	}

	SUBCASE ( "PRIVMSG pacome : Utilisateur inconnu" )
	{
		std::string	str( "PRIVMSG pacome : Utilisateur inconnu" );
		privateMessage( server, *Jean, str );
		CHECK(  (*Jean).GetMessage() == "ERROR : unknown user" );
	}

	SUBCASE ( "PRIVMSG jean charles : 2 user" )
	{
		std::string	str( "PRIVMSG jean charles : 2 user" );
		privateMessage( server, *Jean, str );
		CHECK(  (*Jean).GetMessage() == "ERROR : too many recipients" );
	}

}