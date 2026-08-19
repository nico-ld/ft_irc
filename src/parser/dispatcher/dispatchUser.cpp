/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchUser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:13:34 by nico              #+#    #+#             */
/*   Updated: 2026/08/19 14:03:23 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Server.hpp"
#include "../../../includes/Parser.hpp"

void userCommandsDispatch(std::string command, User &user, Server &server) {
	std::vector<std::string> parameters = Parser::getParameters();

	if (command == "user") {
		if (parameters.empty()) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for USER command");
			throw std::runtime_error("Error: Missing parameter for USER command.");
		}
		user.setRealname(parameters[0]);
		user.setProvidedUser(true);
	}
	else if (command == "nick") {
		if (parameters.empty()) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for NICK command");
			throw std::runtime_error("Error: Missing parameter for NICK command.");
		}
		user.setNickname(parameters[0]);
		user.setProvidedNick(true);
	}
	else if (command == "pass") {
		if (parameters.empty()) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for PASS command");
			throw std::runtime_error("Error: Missing parameter for PASS command.");
		}
		if (parameters[0] != server.getPassword()) {
			server.sendReply(user, ERR_PASSWDMISMATCH, "Invalid password");
			throw std::runtime_error("Error: Invalid password");
		}
		user.setProvidedPassword(true);
	}
	
	if (user.hasProvidedNick() && user.hasProvidedUser() && user.hasProvidedPassword()) {
		user.setAuthenticated(true);
		server.sendReply(user, RPL_WELCOME, "User '" + user.getNickname() + "' successfully registered");
	}
}
