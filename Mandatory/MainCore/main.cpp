/*            _       _                     _          _            _         */
/*           /\ \    /\ \                  /\ \       /\ \        /\ \        */
/*          /  \ \   \_\ \                 \ \ \     /  \ \      /  \ \       */
/*         / /\ \ \  /\__ \                /\ \_\   / /\ \ \    / /\ \ \      */
/*        / / /\ \_\/ /_ \ \     ____     / /\/_/  / / /\ \_\  / / /\ \ \     */
/*       / /_/_ \/_/ / /\ \ \  /\____/\  / / /    / / /_/ / / / / /  \ \_\    */
/*      / /____/\ / / /  \/_/  \/____\/ / / /    / / /__\/ / / / /    \/_/    */
/*     / /\____\// / /                 / / /    / / /_____/ / / /             */
/*    / / /     / / /              ___/ / /__  / / /\ \ \  / / /________      */
/*   / / /     /_/ /              /\__\/_/___\/ / /  \ \ \/ / /_________\     */
/*   \/_/      \_\/               \/_________/\/_/    \_\/\/____________/     */

// #include "Parsing.hpp"
// #include "Server.hpp"

//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 

#include "Server.hpp"
#include "Client.hpp"
#include "Parsing.hpp"

#include <algorithm>
#include <stdio.h> 
#include <cstring> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <sys/select.h>
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
#include <iostream>
#include <string>
#include <sys/types.h> 
#include <sys/socket.h> 

static int
parse( int argc , char *argv[], std::string& rawPort ) {

	int		port = 8888;

	if(argc == 2)
	{
		rawPort = argv[1];
		if(rawPort.size() < 7 && atoi(rawPort.c_str()) > 0 && atoi(rawPort.c_str()) < 65536)
			port = atoi(rawPort.c_str());
		else
		{
			std::cerr << "Bad port!!" << std::endl;
			exit(1);
		}
	}
    return ( port );
}

int main(int argc , char *argv[]) 
{
    Server* server;
    Client* person;
	std::string	rawPort;

    int      port = parse( argc, argv, rawPort );                                //TODO make better

    server = new Server( "password" );



	int		opt = true;
	int		new_socket;
	int		activity;
	int		addrlen;
	int		valread;
    int     sd;
    int     max_sd;
    std::list<Client *> list;


	struct	sockaddr_in address; // STRUCTURE SOCKET mieux identifier

	char buffer[1025]; //  BUFFER DE LECCTURE data buffer of 1K 

    bzero(buffer, sizeof(buffer));
	//set of socket descriptors 
	fd_set readfds; 

	//a message 
	std::string message( "ECHO Daemon v1.0 \r\n" ) ;


	//initialise all client_socket[] to 0 so not checked 
    // SERVER    // TODO check init
	// for (i = 0; i < max_clients; i++) 
	// { 
	// 	client_socket[i] = 0; 
	// } 
		
    server->SetMasterSocket( socket( AF_INET , SOCK_STREAM , 0 ) );
	if( server->GetMasterSocket() == 0) 
	{ 
		perror( "socket failed" ); 
		exit( EXIT_FAILURE ); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
    // SERVER 
	if( setsockopt( server->GetMasterSocket(), SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof( opt ) ) < 0 ) 
	{ 
		perror( "setsockopt" ); 
		exit( EXIT_FAILURE ); 
	} 
	
	//type of socket created 
    // SERVER
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( port );
		
	//bind the socket to localhost port 8888 
    // SERVER
	if ( bind( server->GetMasterSocket(), (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", port); 
		
	//try to specify maximum of 3 pending connections for the master socket 
    // SERVER 
	if ( listen( server->GetMasterSocket(), 3 ) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
    // SEREVER
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
		
    while ( true ) {

        FD_ZERO(&readfds);
		FD_SET( server->GetMasterSocket() , &readfds );
        max_sd = server->GetMasterSocket();

        for ( std::list<Client *>::iterator it = list.begin();
                it != list.end(); it++ ) {
            sd = (*it)->GetSocket();
            if ( sd > 0 )
                FD_SET( sd, &readfds );
            if ( sd > max_sd )
                max_sd = sd;
        }

        activity = select( max_sd + 1, &readfds, NULL, NULL, NULL );
        if ( ( activity < 0 ) && ( errno != EINTR) )
            std::cout << "select error" << std::endl;

        if ( FD_ISSET( server->GetMasterSocket(), &readfds ) ) {
            new_socket = \
             accept( server->GetMasterSocket(),
                                (struct sockaddr *)&address, ( socklen_t* )&addrlen) ;
            if ( new_socket > 0 ) {
                person = new Client();
                person->SetSocket( new_socket );
                server->AddClient( person );
            }

        }

        list = server->getCllist();

        for ( std::list<Client *>::iterator it = list.begin();
                it != list.end(); it++ ) {

            sd = (*it)->GetSocket();

            if ( FD_ISSET( sd, &readfds ) ) {
                valread = read( sd, buffer, 1024 );

                if ( valread == 0 ) {
                    getpeername( new_socket, ( struct sockaddr* )&address,
                            ( socklen_t* )&addrlen );
                    std::cout << "Host disconnected" << std::endl;
                    close( (*it)->GetSocket() );
                    (*it)->SetSocket( -1 );
                }
                else
                    buffer[ valread ] = '\0';

                std::cout << "\t*buffer*\n" << buffer                         // TODO DEBUG
                    << "socket: " << (*it)->GetSocket() << "\n" << std::endl; // TODO DEBUG

            }
            // test( **it );                                                  // TODO debug

            if ( *buffer ) {
                handleCommand( buffer, *server, **it );
                bzero(buffer, sizeof(buffer));
            }
            std::cout << **it << std::endl;
        }
    }
    // TODO handle routine of kill program idea check socket descriptor of       // TODO
    // ClientList which been to -1                                               // TODO
    return ( 0 );
}
