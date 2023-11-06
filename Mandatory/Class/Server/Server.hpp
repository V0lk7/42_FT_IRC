/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 09:47:52 by jduval            #+#    #+#             */
/*   Updated: 2023/11/06 15:22:38 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <list>

class Channel;
class Client;

class Server {

	private:
		
		std::string					_Password;
		int							_MasterSocket;
		std::list<Channel *>		_ChannelList;
		std::list<Client *>			_ClientList;
//		std::map<int, std::string>	_Messages;

		Server	&operator=(Server const &rhs);
		Server(Server const &src);
		Server();

	public:
		Server(std::string const &);
		~Server();

}

#endif
