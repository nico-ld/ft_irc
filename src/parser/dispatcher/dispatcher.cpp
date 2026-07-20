/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/20 11:15:40 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

/* Commands to manage channel and user */
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

/* Commands to send a message */
static void messageCommandsDispatch(std::string command) {
	/* code */
}

/* Commands to register a user */
static void userCommandsDispatch(std::string command, User &user) {
	std::vector<std::string> parameters = Parser::getParameters();

	if (command == "user") {
		if (parameters.empty())
			throw std::runtime_error("Error: Invalid parameter for USER command.");
		user.setRealname(parameters[0]);
		user.setProvidedUser(true);
	}
	else if (command == "nick") {
		if (parameters.empty())
			throw std::runtime_error("Error: Invalid parameter for NICK command.");
		user.setNickname(parameters[0]);
		user.setProvidedNick(true);
	}
	else if (command == "pass") {
		if (parameters.empty())
			throw std::runtime_error("Error: Invalid parameter for PASS command.");
		// if (parameters[0] != Server::getPassword())
		// 	throw std::runtime_error("Error: Invalid password");
		user.setProvidedPassword(true);
	}
	
	if (user.hasProvidedNick() && user.hasProvidedUser() && user.hasProvidedPassword())
		user.setAuthenticated(true);
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
