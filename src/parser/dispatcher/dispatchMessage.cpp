/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchMessage.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:19:20 by nico              #+#    #+#             */
/*   Updated: 2026/09/14 16:14:34 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../../../includes/Parser.hpp"

void messageCommandsDispatch(Server &server, std::string command, User &user, Parser &parser) {
	std::vector<std::string> parameters = parser.getParameters();

	// === PRIVMSG ===
	if (command == "privmsg") {
		if (missingParam(server, user, command, parameters, 1))
			return ;
		
		// Check if there is a message
		if (parameters.size() > 1 || !parser.getTrailing().empty()) {
			std::string message = parser.getMessage();
			
			// If message target is a channel
			if (parameters[0][0] == '#') {
				Channel *channel = server.getChannelByName(parameters[0]);
				if (channelNotExist(server, user, channel, parameters[0]))
					return ;
				
				// Send message to channel
				server.privateMessageChannel(&user, *channel, message, false);
			}
			
			// If message target is an user
			else {
				if (userNotExist(server, user, server.getUserByNickname(parameters[0]), parameters[0]))
					return ;
				
				// Send message to user
				server.privateMessageUser(&user, server.getUserByNickname(parameters[0]), message, false);
			}
		}
	}

	// === NOTICE ===
	else if (command == "notice") {
		// If channel name is missing
		if (parameters.empty()) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Missing parameter for NOTICE command");
			return ;
		}

		// Check if there is a message
		std::string message = parser.getMessage();		
		if (message.empty()) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Missing parameter for NOTICE command");
			return ;
		}

		std::string target = parameters[0];
		
		// If target is a channel
		if (target[0] == '#') {
			Channel *channel = server.getChannelByName(target);
			if (!channel) {
				server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Trying to use NOTICE in channel that doesn't exist");
				return ;
			}
			
			server.privateMessageChannel(&user, *channel, message, true);
		}

		// If target is a user
		else {
			User *userTarget = server.getUserByNickname(target);
			if (!userTarget) {
				server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Trying to send a NOTICE message to a user who doesn't exist");
				return ;
			}
			
			server.privateMessageUser(&user, userTarget, message, true);
		}
	}

	// === UNKNOW ===
	else {
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Command unknow : " + command);
		server.sendReply(user, ERR_UNKNOWNCOMMAND, "Command unknow : " + command);
	}
}
