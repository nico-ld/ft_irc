/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:07:48 by afons             #+#    #+#             */
/*   Updated: 2026/09/04 10:26:51 by nico             ###   ########.fr       */
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

	if (message.find("\r\n") == std::string::npos)
		message.append("\r\n");

	if (message.size() > 512) {
		dash->log(ERROR_LVL, "Reply too long, message being trimed");
		message = message.substr(0, 510).append("\r\n");
	}
	
	dash->log(SERVER, message);
	for (; it != _users.end(); ++it) {
    	queueWrite(it->second, message);
	}
}

void Server::broadcast(const Channel &channel, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();

	if (message.find("\r\n") == std::string::npos)
		message.append("\r\n");

	if (message.size() > 512) {
		dash->log(ERROR_LVL, "Reply too long, message being trimed");
		message = message.substr(0, 510).append("\r\n");
	}
	
	dash->log(SERVER, message);
	for (; it != channel.getMembers().end(); ++it) {
    	queueWrite(*it->second, message);
	}
}

void Server::broadcast(const Channel &channel, const User *user, std::string message) {
	std::map<int, User *>::const_iterator it = channel.getMembers().begin();
	std::string messageError = user->getUsername() + message;
	
	if (message.find("\r\n") == std::string::npos)
		message.append("\r\n");

	dash->log(SERVER, messageError);
	for (; it != channel.getMembers().end(); ++it) {
    	queueWrite(*it->second, messageError);
	}
}

void Server::notification(const User *user, std::string message) {
	std::string messageError = message;
	
	if (message.find("\r\n") == std::string::npos)
		messageError.append("\r\n");
	
	if (message.size() > 512) {
		dash->log(ERROR_LVL, "Reply too long, message being trimed");
		message = message.substr(0, 510).append("\r\n");
	}
	
	dash->log(SERVER, messageError);
	queueWrite(*const_cast<User *>(user), messageError);
}

// In case of too long message, this function trim the message to send it in two times (or more)
static std::vector<std::string> trimMessage(const User *src, std::string dest, std::string message, bool isNotice) {
	// Select right command
	std::string command = " PRIVMSG ";
	if (isNotice)
		command = " NOTICE ";

	// Write begin of message (in case of too long message we can split content and re-use this)
	std::string messagePrefix = src->getPrefix() + command + dest + " :";

	// Check if message get CLRF
	if (message.find("\r\n") == std::string::npos)
		message.append("\r\n");

	std::vector<std::string> messages;
	if (messagePrefix.size() + message.size() > 512) {
		// Strip trailing CRLF so we split the raw content only
		std::string content = message.substr(0, message.size() - 2);

		// Max bytes available for content in each chunk (512 total - prefix - CRLF)
		size_t maxContentSize = 512 - messagePrefix.size() - 2;

		size_t pos = 0;
		while (pos < content.size()) {
			size_t chunkLen = std::min(maxContentSize, content.size() - pos);
			messages.push_back(messagePrefix + content.substr(pos, chunkLen) + "\r\n");
			pos += chunkLen;
		}
	} else {
		messages.push_back(messagePrefix + message);
	}

	return (messages);
}

// Send a message to the channel. The boolean 'sendErr' is used to know wich command call this function (PRIVMSG or NOTICE).
// Also this boolean is used to trigger an error notification
void Server::privateMessageChannel(const User *src, const Channel &channel, std::string message, bool isNotice) {
	if (!channel.isMember(src->getFd()) && isNotice == false) {
		dash->log(WARNING, "Fd : " + toStr(src->getFd()) + ", Is not on the channel");
		sendReply(*src, ERR_NOTONCHANNEL, "You're not in this channel");
		return ;
	}

	// Build message (add prefix, and split message if content is too long)
	std::vector<std::string> messages = trimMessage(src, channel.getName(), message, isNotice);
	
	// Log message
	for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
		dash->log(SERVER, *it);
	}

	// Send message
	for (std::map<int, User *>::const_iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it) {
		// Don't send message to sender (avoid duplicate message display)
		if (it->second->getFd() != src->getFd()) {
			for (std::vector<std::string>::iterator msg = messages.begin(); msg != messages.end(); ++msg) {
				queueWrite(*it->second, *msg);
			}
		}
	}
}

// Send a message to a specific user
void Server::privateMessageUser(const User *src, const User *dest, std::string message, bool isNotice) {
	// Build message (add prefix, and split message if content is too long)
	std::vector<std::string> messages = trimMessage(src, dest->getNickname(), message, isNotice);
	
	// Log message
	for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
		dash->log(SERVER, *it);
	}
	
	// Send message
	for (std::vector<std::string>::iterator msg = messages.begin(); msg != messages.end(); ++msg) {
		queueWrite(*const_cast<User *>(dest), *msg);
	}
}
