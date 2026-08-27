/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchChanCmd.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:26:19 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 08:32:56 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../../../includes/Parser.hpp"
#include "Channel.hpp"

void	channelCommandsDispatch(Server &server, std::string command, User &user, Parser &parser) {
	std::vector<std::string> parameters = parser.getParameters();
	std::vector<Channel> listChannel;
	std::vector<std::string> listKey;
	
	// === JOIN ===
	if (command == "join") {
		listChannel = parser.getChannelList(parameters[0]);
		
		// Get channel(s) from command input
		for (std::vector<Channel>::iterator it_listChannel = listChannel.begin(); it_listChannel != listChannel.end(); ++it_listChannel) {
			std::cout << it_listChannel->getName() << std::endl;
		}
		
		// Dispatch on channels amount
		if (parameters.size() == 0) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing channel name for JOIN command");
			throw std::runtime_error("[LOG] Need a channel name.");
		}
		else if (parameters.size() > 1) {
			listKey = parser.getKeyList(parameters[1]);
			server.join(listChannel, listKey, &user, parser);
		}
		else
			server.join(listChannel, &user, parser);
	}

	// === KICK ===
	else if (command == "kick") {
		// Check user enter every parameters
		if (parameters.size() == 0) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing channel and nickname for KICK command");
			throw std::runtime_error("Need a channel name.");
		}
		else if (parameters.size() == 1) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing a parameter for KICK command");
			throw std::runtime_error("Need a nickname.");
		}
		
		// Parse parameters
		Channel *channel = server.getChannelByName(parameters[0]);
		if (!channel) {
			server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + parameters[0] + "' doesn't exist");
			throw std::runtime_error("[LOG] channel doesn't exist");
		}
		User *kicked = server.getUserByNickname(parameters[1]);
		if (!kicked) {
			server.sendReply(user, ERR_NOSUCHNICK, "User '" + parameters[0] + "' doesn't exist");
			throw std::runtime_error("[LOG] User doesn't exist");
		}
		
		// Dispatch
		if (parameters.size() > 2)
			server.kick(*channel, kicked, parameters[2], &user, parser);
		else
			server.kick(*channel, kicked, &user, parser);
	}

	// === PART ===
	else if (command == "part") {
		// Check if there is a channel name
		if (parameters.size() == 0) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing channel name for PART command");
			throw std::runtime_error("Need a channel name.");
		}
		
		// Dispatch
		listChannel = parser.getChannelList(parameters[0]);
		if (parameters.size() > 1)
			server.part(listChannel, parameters[1], &user, parser);
		else
			server.part(listChannel, &user, parser);
	}

	// === INVITE ===
	else if (command == "invite") {
		// Check parameters amount
		if (parameters.size() == 0) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing channel and nickname for INVITE command");
			throw std::runtime_error("[LOG] Need a nickname and a channel name.");
		}
		else if (parameters.size() == 1) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing a parameter for INVITE command");
			throw std::runtime_error("[LOG] Need a channel name.");
		}

		// Parse channel name
		Channel *channel = server.getChannelByName(parameters[1]);
		if (!channel) {
			server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + parameters[1] + "' doesn't exist");
			throw std::runtime_error("[LOG] channel doesn't exist");
		}

		server.invite(parameters[0], *channel, &user, parser);
	}

	// === TOPIC ===
	else if (command == "topic") {
		// Check if there is a channel name
		if (parameters.size() == 0) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing channel name for TOPIC command");
			throw std::runtime_error("Need a channel name.");
		}

		// Parse channel name
		Channel *channel = server.getChannelByName(parameters[0]);
		if (!channel) {
			server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + parameters[0] + "' doesn't exist");
			throw std::runtime_error("This channel doesn't exist.");
		}

		// Dispatch
		if (parameters.size() > 1)
			server.topic(*channel, parameters[1], &user);
		else
			server.topic(*channel, &user);
	}

	// === MODE ===
	else if (command == "mode") {
		// Check if there is a channel name
		if (parameters.size() == 0) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing channel name for MODE command");
			throw std::runtime_error("Need a channel name.");
		}

		// Ensure channel on server 
		Channel *channel = server.getChannelByName(parameters[0]);
		if (!channel) {
			server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + parameters[0] + "' doesn't exist");
			throw std::runtime_error("[LOG] channel doesn't exist");
		}
		
		// Check command parameters amount
		if (parameters.size() == 1) {
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for MODE command");
			throw std::runtime_error("Need a mode.");
		}
		
		// Dispatch
		if (parameters.size() == 2)
			server.mode(*channel, parameters[1], &user);
		else if (parameters.size() > 2) {
			std::string listMode = parameters[1];
			parameters.erase(parameters.begin(), parameters.begin() + 2);
			server.mode(*channel, listMode, &user, parameters);
		}
	}
}
