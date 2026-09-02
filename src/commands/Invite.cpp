/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 17:25:04 by afons             #+#    #+#             */
/*   Updated: 2026/09/02 11:58:25 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "../../includes/Parser.hpp"

void Server::invite(const std::string &nickname, Channel &channel, const User *user) {
	// Check if User that send command is in the channel
	if (!channel.isMember(user->getFd())) {
		dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to invite on a channel where is not");
		sendReply(*user, ERR_NOTONCHANNEL, "You're not in channel '" + channel.getName() + "'");
		return ;
	}

	// Check if the target is on the server or registered
	if (!getUserByNickname(nickname)) {
		dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to invite a non authenticated user or non existing user");
		sendReply(*user, ERR_NOSUCHNICK, nickname + " is not on the server or not registered");
		return ;
	}

	// If channel is on invite only, check if user have the right permission to invite someone
	if (channel.isInviteOnly()) {
		if (!channel.isOperator(user->getFd())) {
			dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Don't have the permission to invite");
			sendReply(*user, ERR_CHANOPRIVSNEEDED, "You need operator permission to do that");
			return ;
		}
	}

	// Check if invited user is on the channel
	if (channel.isInvited(getUserByNickname(nickname)->getFd()) || channel.isMember(getUserByNickname(nickname)->getFd())) {
		dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to invite someone already on the channel");
		sendReply(*user, ERR_USERONCHANNEL, nickname + " is already on the channel");
		return ;
	}

	// If every guard are OK, invite user on the channel
	channel.inviteUser(getUserByNickname(nickname));

	// Reply
	sendReply(*user, RPL_INVITING, nickname + " " + channel.getName());
	notification(getUserByNickname(nickname), user->getNickname() + " invite you to join " + channel.getName());
}
