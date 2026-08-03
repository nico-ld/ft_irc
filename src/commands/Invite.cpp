/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 17:25:04 by afons             #+#    #+#             */
/*   Updated: 2026/08/03 17:00:15 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <stdexcept>

void Server::invite(const std::string &nickname, Channel &channel, const User *user) {
	if (!channel.isMember(user->getFd()))
		throw std::runtime_error("ERR_NOSUCHCHANNEL");
	if (!getUserByNickname(nickname))
		throw std::runtime_error("ERR_NOTONSERVER");
	if (channel.isInviteOnly())
		if (!channel.isOperator(user->getFd()))
			throw std::runtime_error("ERR_CHANOPRIVSNEEDED");
	if (channel.isInvited(getUserByNickname(nickname)->getFd()) || channel.isMember(getUserByNickname(nickname)->getFd()))
		throw std::runtime_error("ERR_USERONCHANNEL");

	channel.inviteUser(getUserByNickname(nickname));
	std::string message = user->getNickname() + " invited you on the channel.";
	broadcast(channel, message);
}
