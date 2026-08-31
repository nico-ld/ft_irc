/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 15:31:36 by afons             #+#    #+#             */
/*   Updated: 2026/08/31 17:44:23 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "../../includes/Parser.hpp"
#include <stdexcept>

// Shared guard for both overloads: the channel name must be well-formed and
// the caller must actually be in the channel before any topic access.
void Server::topic(const Channel &channel, User *user) {
	if (channel.getTopic().empty())
		sendReply(*user, RPL_NOTOPIC, channel.getName() + " :No Topic setted on this channel");
	else
		sendReply(*user, RPL_TOPIC, channel.getName() + " :" + channel.getTopic());
}

// TOPIC #chan :new topic : change the topic
void Server::topic(Channel &channel, std::string newTopic, User *user, Parser &parser) {
	// Check is mode topic restricted is enabled (+t)
	if (channel.isTopicRestricted()) {
		if (!channel.isOperator(user->getFd())) {
			dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ", Doesn't get operator privilege to change topic");
			sendReply(*user, ERR_CHANOPRIVSNEEDED, "You need operator privilege to do this");
			return ;
		}
	}

	// Set new topic
	channel.setTopic(newTopic);

	// Reply
	broadcast(channel,  user->getPrefix() + " " + parser.getRawString());
}
