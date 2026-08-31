/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchUser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:13:34 by nico              #+#    #+#             */
/*   Updated: 2026/08/31 09:48:37 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Server.hpp"
#include "../../../includes/Parser.hpp"

#define VALID_CHAR "[\\]^_`{|}"

static bool isNicknameValid(Server &server, User &user, std::string nickname) {
	// Check if the nickname is available
	if (server.getUserByNickname(nickname)) {
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : Nickname is already used");
		server.sendReply(user, ERR_NICKNAMEINUSE, "Nickname is already used");
		return (false);
	}

	// Parse nickame for IRC grammar
	if (nickname.size() > 9) {
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : Nickname too long");
		server.sendReply(user, ERR_ERRONEUSNICKNAME, "Nickname too long");
		return (false);
	}
	else if (!isalpha(nickname[0]) && !strchr(VALID_CHAR, nickname[0])) {
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : First character of nickname is invalid");
		server.sendReply(user, ERR_ERRONEUSNICKNAME, "First character of nickname is invalid");
		return (false);
	}
	
	for (size_t i = 1; i < nickname.size(); ++i) {
		if (!isalpha(nickname[i]) && isdigit(nickname[i]) && !strchr(VALID_CHAR, nickname[i]) && nickname[i] != '-') {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : Invalid char in nickname");
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
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : Missing parameter for USER command");
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for USER command");
		}
		
		user.setRealname(parameters[0]);
		user.setProvidedUser(true);
	}

	// === NICK ===
	else if (command == "nick") {
		if (parameters.empty()) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : Missing parameter for NICK command");
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for NICK command");
			return ;
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
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : Missing parameter for PASS command");
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for PASS command");
			return ;
		}
		
		if (parameters[0] != server.getPassword()) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + " : Invalid password");
			server.sendReply(user, ERR_PASSWDMISMATCH, "Invalid password");
			return ;
		}
		user.setProvidedPassword(true);
	}
	
	// Authenticate user if possible
	server.dash->log(DEBUG, "Check if user " + toStr(user.getFd()) + " is fully registered "); // DEBUG
	if (user.hasProvidedNick() && user.hasProvidedUser() && user.hasProvidedPassword()) {
		user.setAuthenticated(true);
		user.setPrefix(user.getNickname() + "!" + user.getUsername() + "@" + user.getHostname());
		
		server.dash->log(SUCCESS, "User '" + user.getNickname() + "' Successfully being authenticated");
		server.sendReply(user, RPL_WELCOME, "User '" + user.getNickname() + "' successfully registered");
		
		server.dash->increaseInfo(server.dash->getSectionByIndex(1), LEFT, 3);
		server.dash->decreaseInfo(server.dash->getSectionByIndex(1), LEFT, 2);
	}
	else {
		if (!user.hasProvidedNick())
			server.dash->log(DEBUG, "Missing Nickname");
		if (!user.hasProvidedUser())
			server.dash->log(DEBUG, "Missing User name");
		if (!user.hasProvidedPassword())
			server.dash->log(DEBUG, "Missing Password");
	}
}
