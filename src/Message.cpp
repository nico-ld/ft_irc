/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:07:48 by afons             #+#    #+#             */
/*   Updated: 2026/08/31 14:52:12 by nico             ###   ########.fr       */
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
	
	dash->log(SERVER, message);
	for (; it != _users.end(); ++it) {
    	queueWrite(it->second, message);
	}
}

void Server::broadcast(const Channel &channel, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();
	
	dash->log(SERVER, message);
	for (; it != channel.getMembers().end(); ++it) {
    	queueWrite(*it->second, message);
	}
}

void Server::broadcast(const Channel &channel, const User *user, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();
	std::string messageError = user->getUsername() + message;
	
	dash->log(SERVER, messageError);
	for (; it != channel.getMembers().end(); ++it) {
    	queueWrite(*it->second, messageError);
	}
}

void Server::notification(const User *user, std::string message) {
	std::string messageError = message;
	
	if (message.find("\r\n") == std::string::npos)
		messageError.append("\r\n");
	
	dash->log(SERVER, messageError);
	queueWrite(*const_cast<User *>(user), messageError);
}

void Server::privateMessageChannel(const User *src, const Channel &channel, std::string message) {
	if (!channel.isMember(src->getFd())) {
		dash->log(WARNING, "Fd : " + toStr(src->getFd()) + ", Is not on the channel");
		sendReply(*src, ERR_NOTONCHANNEL, "You're not in this channel");
		return ;
	}

	std::string channelMessage = src->getPrefix() + " PRIVMSG " + channel.getName() + " :" + message;
	if (message.find("\r\n") == std::string::npos)
		channelMessage.append("\n\r");
	
	dash->log(SERVER, message);
	for (std::map<int, User *>::const_iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it) {
		if (it->second->getFd() != src->getFd())
			queueWrite(*it->second, channelMessage);
	}
}

void Server::privateMessageUser(const User *src, const User *dest, std::string message) {
	std::string privateMessage = src->getNickname() + " PRIVMSG " + dest->getNickname() + " :" + message;
	
	if (message.find("\r\n") == std::string::npos)
		privateMessage.append("\r\n");
	
	dash->log(SERVER, privateMessage);
	queueWrite(*const_cast<User *>(dest), privateMessage);
}
