/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 15:31:36 by afons             #+#    #+#             */
/*   Updated: 2026/08/26 16:49:34 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::topic(const Channel &channel, User *user, Parser &parser) {
	if (channel.getTopic().empty())
		sendReply(*user, RPL_NOTOPIC, "No Topic setted on this channel");
	else
		sendReply(*user, RPL_TOPIC, "Channel topic is : " + channel.getTopic());
}

void Server::topic(Channel &channel, std::string newTopic, User *user, Parser &parser) {
	// Check is mode topic restricted is enabled (+t)
	if (channel.isTopicRestricted()) {
		if (!channel.isOperator(user->getFd())) {
			sendReply(*user, ERR_CHANOPRIVSNEEDED, "You need operator priviledge to do this");
			throw std::runtime_error("[LOG] User is not operator");
		}
	}

	// Set new topic
	channel.setTopic(newTopic);
	std::string message = "New topic of the channel: " + channel.getTopic() + "\r\n";
	broadcast(channel, message);
}
