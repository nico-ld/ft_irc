/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 16:43:41 by afons             #+#    #+#             */
/*   Updated: 2026/08/27 09:55:50 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::part(std::vector<Channel> &channelsList, User *user) {
	for (std::vector<Channel>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		// Get pointer on current channel
		Channel *channel = getChannelByName(it->getName());
		
		// Check if channel exist
		if (!channel) {
			sendReply(*user, ERR_NOSUCHCHANNEL, "Channel doesn't exist");
			throw std::runtime_error("[LOG] Channel doesn't exist");
		}
		
		// Check is user is in the channel
		if (!channel->isMember(user->getFd())) {
			sendReply(*user, ERR_NOTONCHANNEL, "You're not on this channel");
			throw std::runtime_error("[LOG] User is not in the channel");
		}
		
		// Remove user from the channel
		channel->removeMember(user);
		std::string message = user->getNickname() + " left the channel." + "\r\n";
		broadcast(*channel, message);
		
		// If channel is empty, delete it
		if (channel->getMembers().empty()) {
			std::string message = channel->getName() + " has been deleted." + "\r\n";
			broadcastServer(message);
			_channels.erase(channel->getName());
		}
	}
}

void Server::part(std::vector<Channel> &channelsList, std::string reason, User *user) {
	for (std::vector<Channel>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		// Get pointer on current channel
		Channel *channel = getChannelByName(it->getName());
		
		// Check if channel exist
		if (!channel) {
			sendReply(*user, ERR_NOSUCHCHANNEL, "Channel doesn't exist");
			throw std::runtime_error("[LOG] Channel doesn't exist");
		}

		// Check if user is in the channel
		if (!channel->isMember(user->getFd())) {
			sendReply(*user, ERR_NOTONCHANNEL, "You're not on this channel");
			throw std::runtime_error("[LOG] User is not in the channel");
		}
		
		// Remove user from the channel
		channel->removeMember(user);
		std::string message = user->getNickname() + " left the channel.\n Reason: " + reason + "\r\n";
		broadcast(*channel, message);
		
		// if channel is empty, delete it
		if (channel->getMembers().empty()) {
			std::string message = channel->getName() + " has been deleted." + "\r\n";
			broadcastServer(message);
			_channels.erase(channel->getName());
		}
	}
}
