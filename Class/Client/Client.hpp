/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:41:21 by jduval            #+#    #+#             */
/*   Updated: 2023/11/03 14:15:09 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

/*########################################################################################
	Class Client which is for taking all the information of the potential new client
	connected to the server.
	_Socket => File Descriptor of the socket link to the client.
	_Nickname => Nickname used for the IRC protocol. 9 characters only.
	_Username => Obligation name to be a valid client.
	_Auth => List of the setting conditions.
########################################################################################*/

enum step{
	OK,
	PASSWD,
	NICK,
	USER
};

class Client{

	private :
		int			_Socket;
		std::string	_Nickname;
		std::string	_Username;
		bool		_Auth[4];
		//Maybe need to add a container  containing the history of all the NickName
		//the client has had. (Just to coincide with the RFC1459)

		Client(Client const &src);
		Client	&operator=(Client const &rhs);

	public :
		Client();
		~Client();

		void	SetNickname(std::string const &);
		void	SetUsername(std::string const &);
		void	SetSocket(int const &);
		void	SetPasswd(void);

		std::string const	&GetNickname(void) const;
		std::string const	&GetUsername(void) const;
		int const			&GetSocket(void) const;
		bool const			&GetStatement(void) const;
};

#endif
