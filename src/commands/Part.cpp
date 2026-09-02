/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 16:43:41 by afons             #+#    #+#             */
/*   Updated: 2026/09/02 15:35:02 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::part(std::vector<Channel> &channelsList, std::string reason, User *user) {
	int fd = user->getFd();

	for (std::vector<Channel>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		// Get pointer on current channel
		Channel *channel = getChannelByName(it->getName());
		
		// Check if channel exist
		if (!channel) {
			dash->log(WARNING, "Fd : " + toStr(fd) + ", Try to leave a channel that doesn't exist");
			sendReply(*user, ERR_NOSUCHCHANNEL, "Channel doesn't exist");
			return ;
		}

		// Check if user is in the channel
		if (!channel->isMember(fd)) {
			dash->log(WARNING, "Fd : " + toStr(fd) + ", Try to leave a channel without being on this channel");
			sendReply(*user, ERR_NOTONCHANNEL, "You're not on this channel");
			return ;
		}
		
		// Remove user from the channel
		channel->removeMember(user);
		
		std::string message = user->getPrefix() + " PART " + channel->getName();
		if (!reason.empty())
			message.append(" :" + reason);
		
		broadcast(*channel, message);
		notification(user, message);
		
		// if channel is empty, delete it
		if (channel->getMembers().empty()) {
			std::string message = channel->getName() + " has been deleted." + "\r\n";
			_channels.erase(channel->getName());

			// Update dashboard
			dash->decreaseInfo(dash->getSectionByIndex(1), LEFT, 0); // Decrease Channel amount
		}
	}
}
