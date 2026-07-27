/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 17:25:04 by afons             #+#    #+#             */
/*   Updated: 2026/07/27 17:01:43 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/Server.hpp"
#include "../core/Channel.hpp"
#include "../core/User.hpp"
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
