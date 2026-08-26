/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 16:43:41 by afons             #+#    #+#             */
/*   Updated: 2026/08/26 11:23:25 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::part(std::vector<Channel> &channels, User *user, Parser &parser) {
	
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (!parser.checkChannelName(it->getName())) {
			notification(user, "Name channel must start with #");
			throw std::runtime_error("[LOG] Name channel must start with #");
		}
		std::map<std::string, Channel>::iterator getChan = _channels.find(it->getName());
		if (!getChan->second.isMember(user->getFd())) {
			notification(user, "441 ERR_USERNOTINCHANNEL");
			throw std::runtime_error("[LOG] User is not in the channel");
		}
		getChan->second.removeMember(user);
		std::string message = user->getNickname() + " left the channel." + "\r\n";
		broadcast(getChan->second, message);
		if (getChan->second.getMembers().empty()) {
			std::string message = getChan->second.getName() + " has been deleted." + "\r\n";
			broadcastServer(message);
			_channels.erase(getChan->second.getName());
		}
	}
}

void Server::part(std::vector<Channel> &channels, std::string reason, User *user, Parser &parser) {
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
		if (!parser.checkChannelName(it->getName())) {
			notification(user, "Name channel must start with #");
			throw std::runtime_error("[LOG] Name channel must start with #");
		}

		if (!it->isMember(user->getFd())) {
			notification(user, "441 ERR_USERNOTINCHANNEL");
			throw std::runtime_error("[LOG] User is not in the channel");
		}
		it->removeMember(user);
		std::string message = user->getNickname() + " left the channel.\n Reason: " + reason + "\r\n";
		broadcast(*it, message);
		if (it->getMembers().empty()) {
			std::string message = it->getName() + " has been deleted." + "\r\n";
			broadcastServer(message);
			_channels.erase(it->getName());
		}
	}
}
