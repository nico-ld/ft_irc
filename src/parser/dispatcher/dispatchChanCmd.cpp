/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchChanCmd.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:26:19 by nico              #+#    #+#             */
/*   Updated: 2026/08/31 14:32:29 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../../../includes/Parser.hpp"
#include "Channel.hpp"

/* > If amount of parameter is lower than expected, send an error message */
static bool missingParameter(User &user, Server &server, std::string command,
						size_t size, size_t expected)
{
	if (size >= expected)
		return (false);

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", Missing parameter(s) for " + command + " command");
	server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter(s) for " + command + " command");
	return (true);
}

/* > If pointer on channel is NULL, channel doesn't exist, so an error message is send*/
static bool channelNotExist(User &user, Server &server, Channel *channel, std::string chanName) {
	if (channel)
		return (false);
	
	server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", Channel '" + chanName + "' doesn't exist");
	server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + chanName + "' doesn't exist");
	return (true);
}

/* > Dispatcher */
void	channelCommandsDispatch(Server &server, std::string command, User &user, Parser &parser) {
	std::vector<std::string> parameters = parser.getParameters();
	std::vector<Channel> listChannel;
	std::vector<std::string> listKey;
	
	// === JOIN ===
	if (command == "join") {
		// Check if there is somes parameters
		if (missingParameter(user, server, command, parameters.size(), 1))
			return ;
		listChannel = parser.getChannelList(parameters[0]);
		
		// Get channel(s) from command input
		for (std::vector<Channel>::iterator it_listChannel = listChannel.begin(); it_listChannel != listChannel.end(); ++it_listChannel) {
			std::cout << it_listChannel->getName() << std::endl;
		}
		
		// Dispatch on channels amount
		if (parameters.size() > 1) {
			listKey = parser.getKeyList(parameters[1]);
			server.join(listChannel, listKey, &user, parser);
		}
		else
			server.join(listChannel, &user, parser);
	}

	// === KICK ===
	else if (command == "kick") {
		// Check user enter every parameters
		if (missingParameter(user, server, command, parameters.size(), 2))
			return ;
		
		// Parse parameters
		Channel *channel = server.getChannelByName(parameters[0]);
		if (channelNotExist(user, server, channel, parameters[0]))
			return ;

		User *kicked = server.getUserByNickname(parameters[1]);
		if (!kicked) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", User '" + parameters[0] + "' doesn't exist");
			server.sendReply(user, ERR_NOSUCHNICK, "User '" + parameters[0] + "' doesn't exist");
			return ;
		}
		
		// Dispatch
		if (parameters.size() > 2)
			server.kick(*channel, kicked, parameters[2], &user);
		else
			server.kick(*channel, kicked, "", &user);
	}

	// === PART ===
	else if (command == "part") {
		// Check if there is a channel name
		if (missingParameter(user, server, command, parameters.size(), 1))
			return ;
		
		// Dispatch
		listChannel = parser.getChannelList(parameters[0]);
		if (parameters.size() > 1)
			server.part(listChannel, parameters[1], &user);
		else
			server.part(listChannel, "", &user);
	}

	// === INVITE ===
	else if (command == "invite") {
		// Check parameters amount
		if (missingParameter(user, server, command, parameters.size(), 2))
			return ;

		// Parse channel name
		Channel *channel = server.getChannelByName(parameters[1]);
		if (channelNotExist(user, server, channel, parameters[0]))
			return ;

		server.invite(parameters[0], *channel, &user, parser);
	}

	// === TOPIC ===
	else if (command == "topic") {
		// Check if there is a channel name
		if (parameters.size() == 0) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", Missing parameter for TOPIC command");
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for TOPIC command");
			return ;
		}

		// Parse channel name
		Channel *channel = server.getChannelByName(parameters[0]);
		if (!channel) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", Channel '" + parameters[0] + "' doesn't exist");
			server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + parameters[0] + "' doesn't exist");
			return ;
		}

		// Check if user is on the channel
		if (!channel->isMember(user.getFd())) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", User not on channel target ('" + parameters[0] + "')");
			server.sendReply(user, ERR_NOTONCHANNEL, "You're not on this channel");
			return ;
		}

		// Dispatch
		if (parameters.size() > 1)
			server.topic(*channel, parameters[1], &user, parser);
		else
			server.topic(*channel, &user);
	}

	// === MODE ===
	else if (command == "mode") {
		// Check if there is a channel name
		if (missingParameter(user, server, command, parameters.size(), 1))
			return ;

		// Ensure channel on server 
		Channel *channel = server.getChannelByName(parameters[0]);
		if (channelNotExist(user, server, channel, parameters[0]))
			return ;
		
		// Check command parameters amount => WRONG IMPLEMENTATION
		if (parameters.size() == 1) {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", Missing parameter for MODE command");
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for MODE command");
			return ;
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
