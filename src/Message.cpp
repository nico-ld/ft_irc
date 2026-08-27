/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:07:48 by afons             #+#    #+#             */
/*   Updated: 2026/08/27 10:16:09 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include <sys/socket.h>
#include <cstdio>

void Server::broadcastServer(std::string message) {
	std::map<int, User>::const_iterator it = _users.begin();
	for (; it != _users.end(); ++it) {
    	if (send(it->second.getFd(), message.c_str(), message.size(), MSG_NOSIGNAL) == -1)
        	std::perror("Send crashed.");
	}
}

void Server::broadcast(const Channel &channel, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();
	for (; it != channel.getMembers().end(); ++it) {
    	if (send(it->second->getFd(), message.c_str(), message.size(), MSG_NOSIGNAL) == -1)
        	std::perror("Send crashed.");
	}
}

void Server::broadcast(const Channel &channel, const User *user, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();
	std::string messageError = user->getUsername() + message;
	for (; it != channel.getMembers().end(); ++it) {
    	if (send(it->second->getFd(), messageError.c_str(), messageError.size(), MSG_NOSIGNAL) == -1)
        	std::perror("Send crashed.");
	}
}

void Server::notification(const User *user, std::string message) {
	std::string messageError = message + "\r\n";
	if (send(user->getFd(), messageError.c_str(), messageError.size(), MSG_NOSIGNAL) == -1)
        std::perror("Send crashed.");
}

void Server::privateMessageChannel(const User *src, const Channel &channel, std::string message) {
	if (!channel.isMember(src->getFd())) {
		sendReply(*src, ERR_NOTONCHANNEL, "You're not in this channel");
		throw std::runtime_error("User not on the channel.");
	}
	std::string privateMessage = channel.getName() + "-> " + src->getNickname() + ": " + message + "\r\n";
	for (std::map<int, User *>::const_iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it)
		if (send(it->second->getFd(), privateMessage.c_str(), privateMessage.size(), MSG_NOSIGNAL) == -1)
        	std::perror("Send crashed.");
}

void Server::privateMessageUser(const User *src, const User *dest, std::string message) {
	std::string privateMessage = src->getNickname() + ": " + message + "\r\n";
	if (send(dest->getFd(), privateMessage.c_str(), privateMessage.size(), MSG_NOSIGNAL) == -1)
        std::perror("Send crashed.");
}
