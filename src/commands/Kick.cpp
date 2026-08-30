/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:24:09 by afons             #+#    #+#             */
/*   Updated: 2026/08/30 10:36:24 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <stdexcept>
#include <iostream>

/* > Check if sender is on the channel and get operator rights, then if kicked user is on the channel*/
static bool commandValid(Server *server, Channel &channel, User *kicked, const User *op) {
	int opFd = op->getFd();
	
	// Check if sender is on the channel
	if (!channel.isMember(opFd)) {
		server->dash->log(WARNING, "Fd : " + toStr(opFd) + ", Is not on the channel");
		server->sendReply(*op, ERR_NOTONCHANNEL, "You're not on this channel");
		return (false);
	}

	// Check if sender get operator rights
	if (!channel.isOperator(opFd)) {
		server->dash->log(WARNING, "Fd : " + toStr(opFd) + ", doesn't get operator privilege to kick");
		server->sendReply(*op, ERR_CHANOPRIVSNEEDED, "You need to get operator privilege to kick someone");
		return (false);
	}

	// Check if target is on channel
	if (!channel.isMember(kicked->getFd())) {
		server->dash->log(WARNING, "Fd : " + toStr(opFd) + ", Try to kick someone that is not on the channel");
		server->sendReply(*op, ERR_USERNOTINCHANNEL, "User you trying to kick is not on this channel");
		return (false);
	}

	return (true);
}

void Server::kick(Channel &channel, User *kicked, std::string reason, const User *op) {
	// Check if command is valid
	if (!commandValid(this, channel, kicked, op))
		return ;

	// If every guards are OK, kick user
	channel.removeMember(kicked);
	
	// Build kick message
	std::string id = ":" + op->getNickname() + "!" + op->getRealname() + "@" + op->getHostname();
	std::string kickMessage = " KICK " + channel.getName() + " " + kicked->getNickname();
	std::string message = id + kickMessage;

	if (!reason.empty())
		message.append(" :" + reason);
	
	// Send message
	broadcast(channel, kicked, message);
	notification(kicked, "You've been kicked from the channel");
}
