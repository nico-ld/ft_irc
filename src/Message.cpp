/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:07:48 by afons             #+#    #+#             */
/*   Updated: 2026/07/21 15:07:11 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/User.hpp"
#include "core/Channel.hpp"
#include "core/Server.hpp"
#include <sys/socket.h>
#include <cstdio>

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
	std::string messageError = user->getUsername() + message;
	if (send(user->getFd(), messageError.c_str(), messageError.size(), MSG_NOSIGNAL) == -1)
        std::perror("Send crashed.");
}

void Server::privateMessage(const User *src, const User *dest, std::string message) {
	std::string privateMessage = src->getUsername() + ": " + message;
	if (send(dest->getFd(), privateMessage.c_str(), privateMessage.size(), MSG_NOSIGNAL) == -1)
        std::perror("Send crashed.");
}
