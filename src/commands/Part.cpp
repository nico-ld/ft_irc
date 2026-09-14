/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 16:43:41 by afons             #+#    #+#             */
/*   Updated: 2026/09/14 16:49:19 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::part(std::vector<Channel> &channelsList, std::string reason, User *user) {
	for (std::vector<Channel>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		// Get pointer on current channel
		Channel *channel = getChannelByName(it->getName());
		
		if (channelNotExist(*this, *user, channel, it->getName())
			|| userNotOnChannel(*this, *user, *channel))
			return ;
		
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
