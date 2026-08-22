/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 15:31:36 by afons             #+#    #+#             */
/*   Updated: 2026/08/03 17:00:48 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Parser.hpp"
#include <stdexcept>

// Shared guard for both overloads: the channel name must be well-formed and
// the caller must actually be in the channel before any topic access.
static void checkTopicAccess(const Channel &channel, User *user, Server &server) {
	if (!Parser::checkNameChannel(channel.getName())) {
		server.notification(user, "Name channel must start with #");
		throw std::runtime_error("[LOG] Name channel must start with #");
	}

	if (!channel.isMember(user->getFd())) {
		server.notification(user, "442 ERR_NOTONCHANNEL");
		throw std::runtime_error("[LOG] User is not on channel");
	}
}

// TOPIC #chan : read the current topic
void Server::topic(const Channel &channel, User *user) {
	checkTopicAccess(channel, user, *this);

	if (channel.getTopic().empty()) {
		notification(user, "331 RPL_NOTOPIC");
		return ;
	}
	notification(user, channel.getTopic());
}

// TOPIC #chan :new topic : change the topic
void Server::topic(Channel &channel, std::string newTopic, User *user) {
	checkTopicAccess(channel, user, *this);

	// Hypothesis of +t set, where only operators may change the topic
	if (channel.isTopicRestricted() && !channel.isOperator(user->getFd())) {
		notification(user, "482 ERR_CHANOPRIVSNEEDED");
		throw std::runtime_error("[LOG] User is not operator");
	}

	channel.setTopic(newTopic);
	std::string message = "New topic of the channel: " + channel.getTopic() + "\r\n";
	broadcast(channel, message);
}
