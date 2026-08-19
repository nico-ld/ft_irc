/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/08/19 11:25:43 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

void dispatchCommand(Server &server, User &user) {
	std::string command = Parser::getCommand();
	Parser::buildPrefix(user);

	try
	{
		switch (Parser::getCommandListId())
		{
		case 1:
			channelCommandsDispatch(server, command, user);
			break ;
		case 2:
			messageCommandsDispatch(server, command, user);
			break ;
		case 3:
			userCommandsDispatch(command, user, server);
			break ;

		default:
			throw Parser::InvalidCommandException();
			break ;
		}
	}

	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}