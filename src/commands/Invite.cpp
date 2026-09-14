/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 17:25:04 by afons             #+#    #+#             */
/*   Updated: 2026/09/14 16:45:31 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "../../includes/Parser.hpp"

void Server::invite(const std::string &nickname, Channel &channel, const User *user) {
	User *target = getUserByNickname(nickname);

	if (userNotOnChannel(*this, const_cast<User&>(*user), channel)
		|| userNotExist(*this, const_cast<User&>(*user), target, nickname))
		return ;

	// Check if the target is registered
	if (!target->isAuthenticated()) {
		dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to invite a non authenticated user");
		sendReply(*user, ERR_NOSUCHNICK, nickname + " is not authenticated");
		return ;
	}

	// If channel is on invite only, check if user have the right permission to invite someone
	if (channel.isInviteOnly() && notOperator(*this, const_cast<User&>(*user), channel))
		return ;

	// Check if invited user is on the channel
	if (channel.isInvited(getUserByNickname(nickname)->getFd()) || channel.isMember(getUserByNickname(nickname)->getFd())) {
		dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to invite someone already on the channel");
		sendReply(*user, ERR_USERONCHANNEL, nickname + " is already on the channel");
		return ;
	}

	// If every guards are OK, invite user on the channel
	channel.inviteUser(getUserByNickname(nickname));

	// Reply
	sendReply(*user, RPL_INVITING, nickname + " " + channel.getName());
	notification(getUserByNickname(nickname), user->getPrefix() + " INVITE " + nickname + " :" + channel.getName());
}
