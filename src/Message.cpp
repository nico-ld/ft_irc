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

// NOTE: all outgoing sends go through Server::queueWrite() rather than a raw
// send() call, so a slow/blocked client backs up into its NetworkBuffer
// (drained on EPOLLOUT) instead of silently losing data. See ServerHelper.cpp.

void Server::broadcastServer(std::string message) {
	std::map<int, User>::iterator it = _users.begin();
	for (; it != _users.end(); ++it) {
    	queueWrite(it->second, message);
	}
}

void Server::broadcast(const Channel &channel, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();
	for (; it != channel.getMembers().end(); ++it) {
    	queueWrite(*it->second, message);
	}
}

void Server::broadcast(const Channel &channel, const User *user, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();
	std::string messageError = user->getUsername() + message;
	for (; it != channel.getMembers().end(); ++it) {
    	queueWrite(*it->second, messageError);
	}
}

void Server::notification(const User *user, std::string message) {
	std::string messageError = message + "\r\n";
	queueWrite(*const_cast<User *>(user), messageError);
}

void Server::privateMessageChannel(const User *src, const Channel &channel, std::string message) {
	if (!channel.isMember(src->getFd())) {
		sendReply(*src, ERR_NOTONCHANNEL, "You're not in this channel");
		throw std::runtime_error("User not on the channel.");
	}
	std::string privateMessage = channel.getName() + "-> " + src->getNickname() + ": " + message + "\r\n";
	for (std::map<int, User *>::const_iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it)
		queueWrite(*it->second, privateMessage);
}

void Server::privateMessageUser(const User *src, const User *dest, std::string message) {
	std::string privateMessage = src->getNickname() + ": " + message + "\r\n";
	queueWrite(*const_cast<User *>(dest), privateMessage);
}
