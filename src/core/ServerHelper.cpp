/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHelper.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 09:41:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 10:00:55 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::addUnauthenticatedUser(int clientFd) {
    _users.insert(std::make_pair(clientFd, User(clientFd)));
}

void Server::removeUser(int clientFd, std::string message) {
	// Get user
	User* user = getUserById(clientFd);
	if (!user) {
		throw std::runtime_error("[LOG] Unknow Fd");
	}
	
	// Get joined channels
	std::vector<std::string> channelsList = user->getJoinedChannels();
	for (std::vector<std::string>::iterator it = channelsList.begin(); it != channelsList.end(); ++it) {
		Channel *channel = getChannelByName(*it);
		if (!channel) {
			throw std::runtime_error("[LOG] User trying too leave a channel that doesn't exist");
		}
		
		// Announce that user leave the server/channel
		broadcast(*channel, message);

		// Remove user from channel
		channel->removeMember(user);
		if (channel->isOperator(clientFd))
			channel->removeOperator(user);

		// If channel is empty, delete it
		if (channel->getMembers().empty()) {
			std::string message = channel->getName() + " has been deleted." + "\r\n";
			broadcastServer(message);
			_channels.erase(channel->getName());
		}

	}
	
	// Warn client that the user has been disconnected
	notification(user, "Error: closing link: " + message);
	
	// Remove fd/user from server
    epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd, NULL);
    close(clientFd);
    _users.erase(clientFd);
}

User* Server::getUserById(int fd) {
    std::map<int, User>::iterator it = _users.find(fd);
    if (it != _users.end()) {
        return &(it->second);
    }
    return NULL;
}

Channel* Server::getChannelByName(const std::string &name){
    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->second.getName() == name) {
            return &(it->second);
        }
    }
    return NULL;
}
 
User* Server::getUserByNickname(const std::string& nickname) {
    for (std::map<int, User>::iterator it = _users.begin(); it != _users.end(); ++it) {
        if (it->second.getNickname() == nickname) {
            return &(it->second);
        }
    }
    return NULL;
}

