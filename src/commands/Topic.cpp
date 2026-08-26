/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 15:31:36 by afons             #+#    #+#             */
/*   Updated: 2026/08/26 11:24:07 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::topic(const Channel &channel, User *user, Parser &parser) {
	if (!parser.checkChannelName(channel.getName())) {
		notification(user, "Name channel must start with #");
		throw std::runtime_error("[LOG] Name channel must start with #");
	}

	if (channel.getTopic().empty()) {
		notification(user, "No topic on this channel");
		return ;
	}
	notification(user, channel.getTopic());
}

void Server::topic(Channel &channel, std::string newTopic, User *user, Parser &parser) {
	if (!parser.checkChannelName(channel.getName())) {
		notification(user, "Name channel must start with #");
		throw std::runtime_error("[LOG] Name channel must start with #");
	}

	if (channel.isTopicRestricted())
		if (!channel.isOperator(user->getFd())) {
			notification(user, "482 ERR_CHANOPRIVSNEEDED");
			throw std::runtime_error("[LOG] User is not operator");
		}

	channel.setTopic(newTopic);
	std::string message = "New topic of the channel: " + channel.getTopic() + "\r\n";
	broadcast(channel, message);
}
