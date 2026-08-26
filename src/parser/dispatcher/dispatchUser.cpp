/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchUser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:13:34 by nico              #+#    #+#             */
/*   Updated: 2026/08/26 10:58:52 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Server.hpp"
#include "../../../includes/Parser.hpp"

#define VALID_CHAR "[\\]^_`{|}"

static bool isNicknameValid(Server &server, User &user, std::string nickname) {
	// Check if the nickname is available
	if (server.getUserByNickname(nickname)) {
		server.sendReply(user, ERR_NICKNAMEINUSE, "Nickname is already used");
		return (false);
	}

	// Parse nickame for IRC grammar
	if (nickname.size() > 9) {
		server.sendReply(user, ERR_ERRONEUSNICKNAME, "Nickname too long");
		return (false);
	}
	else if (!isalpha(nickname[0]) || isdigit(nickname[0]) || !strchr(VALID_CHAR, nickname[0])) {
		server.sendReply(user, ERR_ERRONEUSNICKNAME, "First character of nickname is invalid");
		return (false);
	}
	
	for (size_t i = 1; i < nickname.size(); ++i) {
		if (!isalpha(nickname[i]) && isdigit(nickname[i]) && !strchr(VALID_CHAR, nickname[i]) && nickname[i] != '-') {
			server.sendReply(user, ERR_ERRONEUSNICKNAME, "Invalid character in nickname");
			return (false);
		}
	}
	return (true);
}

void userCommandsDispatch(std::string command, User &user, Server &server, Parser &parser) {
	std::vector<std::string> parameters = parser.getParameters();

	// === USER ===
	if (command == "user") {
		if (parameters.empty()) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for USER command");
			throw std::runtime_error("Error: Missing parameter for USER command.");
		}
		
		user.setRealname(parameters[0]);
		user.setProvidedUser(true);
	}

	// === NICK ===
	else if (command == "nick") {
		if (parameters.empty()) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for NICK command");
			throw std::runtime_error("Error: Missing parameter for NICK command.");
		}
		
		// Parse Nickname
		if (isNicknameValid(server, user, parameters[0])) {
			user.setNickname(parameters[0]);
			user.setProvidedNick(true);
		}
	}

	// === PASS ===
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
	
	// Authenticate user if possible
	if (user.hasProvidedNick() && user.hasProvidedUser() && user.hasProvidedPassword()) {
		user.setAuthenticated(true);
		user.setPrefix(user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname());
		server.sendReply(user, RPL_WELCOME, "User '" + user.getNickname() + "' successfully registered");
	}
}
