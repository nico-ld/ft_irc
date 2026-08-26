/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:24:09 by afons             #+#    #+#             */
/*   Updated: 2026/08/26 16:50:25 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>

void Server::kick(Channel &channel, User *kicked, const User *op, Parser &parser) {
	// Check if sender is on the channel
	if (!channel.isMember(op->getFd())) {
		sendReply(*op, ERR_NOTONCHANNEL, "You're not on this channel");
		throw std::runtime_error("[LOG] Sender is not on the channel");
	}
	
	// Check if sender get operator rights
	else if (!channel.isOperator(op->getFd())) {
		sendReply(*op, ERR_CHANOPRIVSNEEDED, "You need to get operator priviledge to do this");
		throw std::runtime_error("[LOG] Sender doesn't get operator priviledge");
	}

	// Check if target is on the channel
	else if (!channel.isMember(kicked->getFd())) {
		sendReply(*op, ERR_USERNOTINCHANNEL, "User you trying to kick is not on this channel");
		throw std::runtime_error("[LOG] Kicked is not on the channel");
	}
	
	// If every guards are OK, kick user
	channel.removeMember(kicked); // Remove it from the channel
	
	// Build kick message
	std::string id = ":" + op->getNickname() + "!" + op->getRealname() + "@" + op->getHostname();
	std::string kickMessage = " KICK " + channel.getName() + " " + kicked->getNickname();
	std::string message = id + kickMessage;
	
	// Send message
	broadcast(channel, kicked, message);
	notification(kicked, "You've been kicked from the channel");
}

void Server::kick(Channel &channel, User *kicked, std::string reason, const User *op, Parser &parser) {
	// Check if sender is on the channel
	if (!channel.isMember(op->getFd())) {
		sendReply(*op, ERR_NOTONCHANNEL, "You're not on this channel");
		throw std::runtime_error("[LOG] Sender is not on the channel");
	}
	
	// Check if sender get operator rights
	else if (!channel.isOperator(op->getFd())) {
		sendReply(*op, ERR_CHANOPRIVSNEEDED, "You need to get operator priviledge to do this");
		throw std::runtime_error("[LOG] Sender doesn't get operator priviledge");
	}

	// Check if target is on the channel
	else if (!channel.isMember(kicked->getFd())) {
		sendReply(*op, ERR_USERNOTINCHANNEL, "User you trying to kick is not on this channel");
		throw std::runtime_error("[LOG] Kicked is not on the channel");
	}

	// If every guards are OK, kick user
	channel.removeMember(kicked);
	
	// Build kick message
	std::string id = ":" + op->getNickname() + "!" + op->getRealname() + "@" + op->getHostname();
	std::string kickMessage = " KICK " + channel.getName() + " " + kicked->getNickname() + ":" + reason;
	std::string message = id + kickMessage;
	
	// Send message
	broadcast(channel, kicked, message);
	notification(kicked, "You've been kicked from the channel");
}
