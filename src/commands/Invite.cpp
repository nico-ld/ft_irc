/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 17:25:04 by afons             #+#    #+#             */
/*   Updated: 2026/08/26 16:50:52 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "../../includes/Parser.hpp" // absolute path to avoid conflict with bot Parser
#include <stdexcept>

void Server::invite(const std::string &nickname, Channel &channel, const User *user, Parser &parser) {
	// Check if User that send command is in the channel
	if (!channel.isMember(user->getFd())) {
		sendReply(*user, ERR_NOTONCHANNEL, "You're not in channel '" + channel.getName() + "'");
		throw std::runtime_error("[LOG] User are not on the channel.");
	}

	// Check if the target is on the server or registered
	if (!getUserByNickname(nickname)) {
		sendReply(*user, ERR_NOSUCHNICK, nickname + " is not on the server or not registered");
		throw std::runtime_error("[LOG] User are not on the server.");
	}

	// If channel is on invite only, check if user have the right permission to invite someone
	if (channel.isInviteOnly()) {
		if (!channel.isOperator(user->getFd())) {
			sendReply(*user, ERR_CHANOPRIVSNEEDED, "You need operator permission to do that");
			throw std::runtime_error("[LOG] User is not operator");
		}
	}

	// Check if invited user is on the channel
	if (channel.isInvited(getUserByNickname(nickname)->getFd()) || channel.isMember(getUserByNickname(nickname)->getFd())) {
		sendReply(*user, ERR_USERONCHANNEL, nickname + " is already on the channel");
		throw std::runtime_error("[LOG] User already on this channel");
	}

	// If every guard are OK, invite user on the channel
	channel.inviteUser(getUserByNickname(nickname));
	std::string message = user->getNickname() + " invited you on the channel.";
	notification(getUserByNickname(nickname), message);
}
