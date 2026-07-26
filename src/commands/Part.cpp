/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 16:43:41 by afons             #+#    #+#             */
/*   Updated: 2026/07/26 17:18:58 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/Server.hpp"
#include "../core/User.hpp"
#include "../core/Channel.hpp"
#include <stdexcept>

void Server::part(std::vector<Channel> &channels, User *user) {
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (!it->isMember(user->getFd()))
			throw std::runtime_error("ERR_USERNOTINCHANNEL");
		it->removeMember(user);
		std::string message = user->getNickname() + " left the channel.";
		broadcast(*it, message);
		if (it->getMembers().empty()) {
			std::string message = it->getName() + " has been deleted.";
			broadcastServer(message);
			_channels.erase(it->getName());
		}
	}
}

void Server::part(std::vector<Channel> &channels, std::string reason, User *user) {
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (!it->isMember(user->getFd()))
			throw std::runtime_error("ERR_USERNOTINCHANNEL");
		it->removeMember(user);
		std::string message = user->getNickname() + " left the channel.\n Reason: " + reason;
		broadcast(*it, message);
		if (it->getMembers().empty()) {
			std::string message = it->getName() + " has been deleted.";
			broadcastServer(message);
			_channels.erase(it->getName());
		}
	}
}
