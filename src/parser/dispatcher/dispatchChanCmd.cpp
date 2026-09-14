/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchChanCmd.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:26:19 by nico              #+#    #+#             */
/*   Updated: 2026/09/14 16:05:10 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../../../includes/Parser.hpp"
#include "Channel.hpp"

/* > Dispatcher */
void	channelCommandsDispatch(Server &server, std::string command, User &user, Parser &parser) {
	std::vector<std::string> parameters = parser.getParameters();
	std::vector<Channel> listChannel;
	std::vector<std::string> listKey;
	
	// === JOIN ===
	if (command == "join") {
		if (missingParam(server, user, command, parameters, 1))
			return ;

		listChannel = parser.getChannelList(parameters[0], server, user);

		// Dispatch on channels amount
		if (parameters.size() > 1) {
			listKey = parser.getKeyList(parameters[1]);
			server.join(listChannel, listKey, &user, parser);
		}
		else {
			server.join(listChannel, &user, parser);
		}
	}

	// === KICK ===
	else if (command == "kick") {
		if (missingParam(server, user, command, parameters, 2))
			return ;
		
		// Parse parameters
		Channel *channel = server.getChannelByName(parameters[0]);
		if (channelNotExist(server, user, channel, parameters[0]))
			return ;

		User *kicked = server.getUserByNickname(parameters[1]);
		if (userNotExist(server, user, kicked, parameters[1]))
			return ;
		
		// Dispatch
		if (!parser.getTrailing().empty())
			server.kick(*channel, kicked, parser.getTrailing(), &user);
		else
			server.kick(*channel, kicked, "", &user);
	}

	// === PART ===
	else if (command == "part") {
		if (missingParam(server, user, command, parameters, 1))
			return ;
		
		// Dispatch
		listChannel = parser.getChannelList(parameters[0], server, user);
		if (!parser.getTrailing().empty())
			server.part(listChannel, parser.getTrailing(), &user);
		else
			server.part(listChannel, "", &user);
	}

	// === INVITE ===
	else if (command == "invite") {
		if (missingParam(server, user, command, parameters, 2))
			return ;

		// Parse channel name
		Channel *channel = server.getChannelByName(parameters[1]);
		if (channelNotExist(server, user, channel, parameters[0]))
			return ;

		server.invite(parameters[0], *channel, &user);
	}

	// === TOPIC ===
	else if (command == "topic") {
		if (missingParam(server, user, command, parameters, 1))
			return ;

		// Parse channel name
		Channel *channel = server.getChannelByName(parameters[0]);
		if (channelNotExist(server, user, channel, parameters[0]))
			return ;

		// Dispatch
		if (!parser.getTrailing().empty())
			server.topic(*channel, parser.getTrailing(), &user, parser);
		else
			server.topic(*channel, &user);
	}

	// === MODE ===
	else if (command == "mode") {
		if (missingParam(server, user, command, parameters, 1))
			return ;

		Channel *channel = server.getChannelByName(parameters[0]);
		if (channelNotExist(server, user, channel, parameters[0]))
			return ;
		
			
		// Dispatch on parameter amount
		if (parameters.size() == 1)
			server.sendReply(user, RPL_CHANNELMODEIS, server.displayChannelStatus(*channel));
		else if (parameters.size() == 2)
			server.mode(*channel, parameters[1], &user);
		else if (parameters.size() > 2) {
			std::string listMode = parameters[1];
			parameters.erase(parameters.begin(), parameters.begin() + 2);
			server.mode(*channel, listMode, &user, parameters);
		}
	}

	// === COMMAND UNKNOW ===
	else {
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Command unknow : " + command);
		server.sendReply(user, ERR_UNKNOWNCOMMAND, "Command unknow : " + command);
	}
}
