/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchMessage.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:19:20 by nico              #+#    #+#             */
/*   Updated: 2026/08/26 10:56:46 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../../../includes/Parser.hpp"

void messageCommandsDispatch(Server &server, std::string command, User &user, Parser &parser) {
	std::vector<std::string> parameters = parser.getParameters();

	if (command == "privmsg") {
		// Check if target is given
		if (parameters.size() == 0)
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing target for PRIVMSG command");
		
		// Check if there is a message
		if (parameters.size() > 1 || !parser.getTrailing().empty()) {
			std::string message = parser.getMessage();
			
			// If message target is a channel
			if (parameters[0][0] == '#') {
				Channel *channel = server.getChannelByName(parameters[0]);
				
				// Check if channel exist
				if (!channel) {
					server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + parameters[0] + "' doesn't exist");
					throw std::runtime_error("Channel doesn't exist");
				}
				
				// Send message to channel
				server.privateMessageChannel(&user, *channel, message);
			}
			
			// Is message target is a user
			else {
				// Check if user exist
				if (!server.getUserByNickname(parameters[0])) {
					server.sendReply(user, ERR_NOSUCHNICK, "User '" + parameters[0] + "' doesn't exist");
					throw std::runtime_error("User doesn't exist");
				}
				
				// Send message to user
				server.privateMessageUser(&user, server.getUserByNickname(parameters[0]), message);
			}
		}
	}
}
