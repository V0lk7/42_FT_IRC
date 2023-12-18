#include "doctest.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

#include "../../../Mandatory/Commands/PRIVMSG/Privmsg.cpp"

TEST_CASE ( "PrivateMessage" )
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
		PrivateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == ":Jean PRIVMSG Charles : Unmot\r\n");
	}

	SUBCASE ( "PRIVMSG Charles :" )
	{
		std::string	str( "PRIVMSG Charles :" );
		PrivateMessage( server, *Jean, str );
		CHECK( (*Jean).GetMessage() == ": 412 PRIVMSG : No message\r\n" );
	}

	SUBCASE ( "PRIVMSG Charles : Deux mots" )
	{
		std::string	str( "PRIVMSG Charles : Deux mots" );
		PrivateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == ":Jean PRIVMSG Charles : Deux mots\r\n");
	}

	SUBCASE ( "PRIVMSG Charles : Beaucoup                 d'espaces" )
	{
		std::string	str( "PRIVMSG Charles : Beaucoup                 d'espaces" );
		PrivateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == ":Jean PRIVMSG Charles : Beaucoup                 d'espaces\r\n");
	}

	SUBCASE ( "PRIVMSG Charles :Ch@r@ct3r&s @ |a c0n !<>,./$%^*()_+*:" )
	{
		std::string	str( "PRIVMSG Charles :Ch@r@ct3r&s @ |a c0n !<>,./$%^*()_+*:" );
		PrivateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == ":Jean PRIVMSG Charles :Ch@r@ct3r&s @ |a c0n !<>,./$%^*()_+*:\r\n");
	}

	SUBCASE ( "PRIVMSGCharles :Commande colle au user" )
	{
		std::string	str( "PRIVMSGCharles :Commande colle au user" );
		PrivateMessage( server, *Jean, str );
		std::string test = (*Jean).GetMessage();
		CHECK( (*Jean).GetMessage() == ": 441 PRIVMSG : No recipient\r\n" );
	}

	SUBCASE ( "PRIVMSG Charles:Deux points colle au user" )
	{
		std::string	str( "PRIVMSG Charles:Deux points colle au user" );
		PrivateMessage( server, *Jean, str );
		CHECK( (*Charles).GetMessage() == ":Jean PRIVMSG Charles :Deux points colle au user\r\n");
	}

	SUBCASE ( "PRIVMSG pacome : Utilisateur inconnu" )
	{
		std::string	str( "PRIVMSG pacome : Utilisateur inconnu" );
		PrivateMessage( server, *Jean, str );
		CHECK(  (*Jean).GetMessage() == ": 401 PRIVMSG : Unknown user\r\n" );
	}

	SUBCASE ( "PRIVMSG jean charles : 2 user" )
	{
		std::string	str( "PRIVMSG jean charles : 2 user" );
		PrivateMessage( server, *Jean, str );
		CHECK(  (*Jean).GetMessage() == ": 441 PRIVMSG : Too many recipients\r\n" );
	}

	SUBCASE ( "PRIVMSG #test :Channel" )
	{
		std::string	str( "PRIVMSG #test :Channel" );
		PrivateMessage( server, *Jean, str );
		CHECK(  (*Jean).GetMessage() == ": 403 PRIVMSG : Unknown channel\r\n" );
	}

	SUBCASE ( "PRIVMSG #Test :Bonjour Henri" )
	{
		std::string	str( "PRIVMSG #Test :Bonjour Henri" );
		PrivateMessage( server, *Jean, str );
		CHECK(  (*Hanri).GetMessage() == ":Jean PRIVMSG #Test :Bonjour Henri\r\n" );
		CHECK(  (*Anri).GetMessage() == ":Jean PRIVMSG #Test :Bonjour Henri\r\n" );
		CHECK(  (*Enri).GetMessage() == ":Jean PRIVMSG #Test :Bonjour Henri\r\n" );
		CHECK(  (*Henry).GetMessage() == ":Jean PRIVMSG #Test :Bonjour Henri\r\n" );

	}

	SUBCASE ( "PRIVMSG #Test :Bonjour Henri" )
	{
		std::string	str( "PRIVMSG #Test :Bonjour Henri" );
		PrivateMessage( server, *Hanri, str );
		CHECK(  (*Anri).GetMessage() == ":Hanri PRIVMSG #Test :Bonjour Henri\r\n" );
		CHECK(  (*Enri).GetMessage() == ":Hanri PRIVMSG #Test :Bonjour Henri\r\n" );
		CHECK(  (*Henry).GetMessage() == ":Hanri PRIVMSG #Test :Bonjour Henri\r\n" );
	}
}
