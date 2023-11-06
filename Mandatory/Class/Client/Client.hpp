/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:41:21 by jduval            #+#    #+#             */
/*   Updated: 2023/11/06 13:59:56 by jduval           ###   ########.fr       */
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

		std::string	_Input;

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
