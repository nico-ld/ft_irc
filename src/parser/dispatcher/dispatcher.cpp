/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/20 10:26:47 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

static void	channelCommandsDispatch(std::string command, User &user) {
	/*
	std::vector<std::string> parameters = Parser::getParameters();
	
	if (command == "join") {
		if (parameters[1])
			Server::join(parameters[0], parameters[1], user);
		else
			Server::join(parameters[0], user);
	}
	*/
}

static void messageCommandsDispatch(std::string command) {
	/* code */
}

static void userCommandsDispatch(std::string command, User &user) {
	/* code */
}

void dispatchCommand(User &user) {
	std::string command = Parser::getCommand();

	switch (Parser::getCommandListId())
	{
		case 1:
			channelCommandsDispatch(command, user);
			break ;
		case 2:
			messageCommandsDispatch(command);
			break ;
		case 3:
			userCommandsDispatch(command, user);
			break ;

		default:
			throw Parser::InvalidCommandException();
			break ;
	}
}
