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
#include <stdexcept>

void Server::topic(const Channel &channel, User *user) {
	if (channel.getTopic().empty()) {
		notification(user, "No topic on this channel");
		return ;
	}
	notification(user, channel.getTopic());
}

void Server::topic(Channel &channel, std::string newTopic, User *user) {
	if (channel.isTopicRestricted())
		if (!channel.isOperator(user->getFd()))
			throw std::runtime_error("ERR_CHANOPRIVSNEEDED");

	channel.setTopic(newTopic);
	std::string message = "New topic of the channel: " + channel.getTopic();
	broadcast(channel, message);
}
