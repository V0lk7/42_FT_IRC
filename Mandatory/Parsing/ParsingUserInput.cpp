/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingUserInput.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jduval <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:49:27 by jduval            #+#    #+#             */
/*   Updated: 2023/11/03 12:59:53 by jduval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

bool		CheckUserInputs(int ac, char **av)
{
	if (CheckArgsNumber(ac) != true)
		return (false);
	if (CheckPortNumber(av[1]) != true)
		return (false);
	return (true);
}

static bool	CheckArgsNumber(int ac)
{
	if (ac != 3)
	{
		std::cerr	<< "ircserv: bad arguments.\n"
					<< "Pathern: [executable] [Port] [Password]"
					<< std::endl;
		return (false);
	}
	return (true);
}
