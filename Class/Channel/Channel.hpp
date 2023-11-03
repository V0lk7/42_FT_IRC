/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 12:55:35 by jduval            #+#    #+#             */
/*   Updated: 2023/11/03 14:18:18 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <list>
# include <utility>

/*########################################################################################
	Class wich will keep all the informations usefull of the channel.
	_Name => Name of the channel;
	_Password => Password for a new client to enter the channel;
	_Topic => Topic of the channel;
	_LimitUsers => Define the numbers of client the channel can have (0 = no limit);
	_Mode => Define the MODE parameters of the channel:
		-> Mode[0] = "i" Invite only channel 0 = NO, 1 = YES.
		-> Mode[1] = "t" Restriction of the TOPIC command,
					only to channel operator. 0 = NO, 1 = YES.
		-> Mode[2] = "k" Define a password for the channel. 0 = NO, 1 = YES.
	_Users => Define the client list which is in the channel.
		List containing a pair;
			Pair contening the actual grade of the client (channel operator or not)
			And containing the client.
########################################################################################*/

enum mode {
	I,
	K,
	T
};

class Client;

class Channel{

	private :
		std::string							_Name;
		std::string							_Password;
		std::string							_Topic;
		size_t								_LimitUsers;
		bool								_Mode[3];

		std::list<std::pair<int, Client &>>	_Users;

		Channel	&operator=(Channel const &rhs);
		Channel(Channel const &src);

	public :
		Channel();
		Channel(std::string const &);
		~Channel();

		void	SetName(std::string const &);
		void	SetPassword(std::string const &);
		void	SetTopic(std::string const &);
		void	SetLimitUsers(size_t const &);
		void	SetMode(int, int);

		std::string const	&GetName(void) const;
		std::string const	&GetPassword(void) const;
		std::string const	&GetTopic(void) const;
		size_t const		&GetLimitUsers(void) const;
		bool const			&GetMode(int) const;
};

#endif
