/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/22 15:42:42 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

/* Commands to manage channel and user */
static void	channelCommandsDispatch(Server &server, std::string command, User &user) {
	// std::cout << command << " is no handled for the moment." << std::endl;
	
	std::vector<std::string> parameters = Parser::getParameters();
	
	std::cout << command << std::endl;
	if (command == "join") {
		if (parameters.size() > 1)
			server.join(parameters[0], parameters[1], &user);
		else
			server.join(parameters[0], &user);
	}
	if (command == "kick") {
		if (parameters.size() > 2)
			server.kick(parameters[0], parameters[1], parameters[2], &user);
		else 
			server.kick(parameters[0], parameters[1], &user);
	}
}

/* Commands to send a message */
static void messageCommandsDispatch(std::string command, User &user) {
	(void)user;
	std::cout << command << " is not handled for the moment." << std::endl;
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
			messageCommandsDispatch(command, user);
			break ;
		case 3:
			userCommandsDispatch(command, user);
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
