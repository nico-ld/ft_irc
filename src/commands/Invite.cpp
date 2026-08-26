/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 17:25:04 by afons             #+#    #+#             */
/*   Updated: 2026/08/26 11:17:37 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Parser.hpp"
#include <stdexcept>

void Server::invite(const std::string &nickname, Channel &channel, const User *user, Parser &parser) {
	if (!parser.checkChannelName(channel.getName())) {
		notification(user, "Name channel must start with #.");
		throw std::runtime_error("[LOG] Name channel must start with #.");
	}

	if (!channel.isMember(user->getFd())) {
		notification(user, "403 ERR_NOSUCHCHANNEL");
		throw std::runtime_error("[LOG] User are not on the channel.");
	}
	if (!getUserByNickname(nickname)) {
		notification(user, "442 ERR_NOTONSERVER");
		throw std::runtime_error("[LOG] User are not on the server.");
	}
	if (channel.isInviteOnly())
		if (!channel.isOperator(user->getFd())) {
			notification(user, "482 ERR_CHANOPRIVSNEEDED");
			throw std::runtime_error("[LOG] User is not operator");
		}
	if (channel.isInvited(getUserByNickname(nickname)->getFd()) || channel.isMember(getUserByNickname(nickname)->getFd())) {
		notification(user, "443 ERR_USERONCHANNEL");
		throw std::runtime_error("[LOG] User already on this channel");
	}

	channel.inviteUser(getUserByNickname(nickname));
	std::string message = user->getNickname() + " invited you on the channel.";
	notification(getUserByNickname(nickname), message);
}
