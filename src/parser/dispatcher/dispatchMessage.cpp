/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchMessage.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:19:20 by nico              #+#    #+#             */
/*   Updated: 2026/08/19 11:30:52 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../../../includes/Parser.hpp"

void messageCommandsDispatch(Server &server, std::string command, User &user) {
	std::vector<std::string> parameters = Parser::getParameters();

	if (command == "privmsg") {
		if (parameters.size() == 0)
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing target for PRIVMSG command");
		if (parameters.size() > 1) {
			std::string message = Parser::getMessage(parameters);
			if (parameters[0][0] == '#') {
				Channel *channel = server.getChannelByName(parameters[0]);
				if (!channel) {
					server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + parameters[0] + "' doesn't exist");
					throw std::runtime_error("Channel doesn't exist");
				}
				server.privateMessageChannel(&user, *channel, message);
			}
			else {
				if (!server.getUserByNickname(parameters[0])) {
					server.sendReply(user, ERR_NOSUCHNICK, "User '" + parameters[0] + "' doesn't exist");
					throw std::runtime_error("User doesn't exist");
				}
				server.privateMessageUser(&user, server.getUserByNickname(parameters[0]), message);
			}
		}
	}
}
