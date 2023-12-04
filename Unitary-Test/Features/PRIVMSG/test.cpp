#include "doctest.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

#include "../../../Mandatory/Commands/PRIVMSG/Privmsg.cpp"

TEST_CASE ( "privateMessage" )
{
	Server		server;

	server.AddClient( new Client( "Hanri", CLIENT ) );
	server.AddClient( new Client( "Enri", CLIENT ) );
	server.AddClient( new Client( "Anri", CLIENT ) );
	server.AddClient( new Client( "Henry", CLIENT ) );

	Client*		Jean = server.GetClient( "Jean" );
	Client*		Charles = server.GetClient( "Charles" );
	Client*		Hanri = server.GetClient( "Hanri" );
	Client*		Enri = server.GetClient( "Enri" );
	Client*		Anri = server.GetClient( "Anri" );
	Client*		Henry = server.GetClient( "Henry" );
	Channel*	channel = server.GetChannel( "#Test" );


	channel->AddClientToChannel( *Hanri, 0 );
	channel->AddClientToChannel( *Enri, 0 );
	channel->AddClientToChannel( *Anri, 0 );
	channel->AddClientToChannel( *Henry, 0 );

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
		CHECK( (*Jean).GetMessage() == ": 441 PRIVMSG : No recipient.\r\n" );
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
		CHECK(  (*Jean).GetMessage() == ": 401 PRIVMSG : Unknown user.\r\n" );
	}

	SUBCASE ( "PRIVMSG jean charles : 2 user" )
	{
		std::string	str( "PRIVMSG jean charles : 2 user" );
		privateMessage( server, *Jean, str );
		CHECK(  (*Jean).GetMessage() == ": 441 PRIVMSG : Too many recipients.\r\n" );
	}

	SUBCASE ( "PRIVMSG #test :Channel" )
	{
		std::string	str( "PRIVMSG #test :Channel" );
		privateMessage( server, *Jean, str );
		CHECK(  (*Jean).GetMessage() == ": 403 PRIVMSG : Unknown channel.\r\n" );
	}

	SUBCASE ( "PRIVMSG #Test :Bonjour Henri" )
	{
		std::string	str( "PRIVMSG #Test :Bonjour Henri" );
		privateMessage( server, *Jean, str );
		CHECK(  (*Hanri).GetMessage() == "Bonjour Henri" );
		CHECK(  (*Anri).GetMessage() == "Bonjour Henri" );
		CHECK(  (*Enri).GetMessage() == "Bonjour Henri" );
		CHECK(  (*Henry).GetMessage() == "Bonjour Henri" );
	}

	SUBCASE ( "PRIVMSG #Test :Bonjour Henri" )
	{
		std::string	str( "PRIVMSG #Test :Bonjour Henri" );
		privateMessage( server, *Hanri, str );
		CHECK(  (*Anri).GetMessage() == "Bonjour Henri" );
		CHECK(  (*Enri).GetMessage() == "Bonjour Henri" );
		CHECK(  (*Henry).GetMessage() == "Bonjour Henri" );
	}
}