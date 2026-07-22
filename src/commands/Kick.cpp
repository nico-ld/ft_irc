/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:24:09 by afons             #+#    #+#             */
/*   Updated: 2026/07/22 16:47:16 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/Server.hpp"
#include "../core/Channel.hpp"
#include "../core/User.hpp"
#include <stdexcept>

void Server::kick(std::string nameChannel, std::string kicked, const User *op) {
	std::map<std::string, Channel>::iterator findChannel = _channels.find(nameChannel);
	if (findChannel == _channels.end())
		throw std::runtime_error("ERR_NOSUCHCHANNEL");

	std::map<int, User *>::const_iterator findUser = findChannel->second.getMembers().begin();
	for (; findUser != findChannel->second.getMembers().end(); ++findUser) {
		if (findUser->second->getNickname() == kicked)
			break;
	}
	if (findUser == findChannel->second.getMembers().end())
		throw std::runtime_error("ERR_NOTONCHANNEL");

	if (!findChannel->second.isOperator(op->getFd()))
		throw std::runtime_error("ERR_CHANOPRIVSNEEDED");
	if (!findChannel->second.isMember(findUser->second->getFd()))
		throw std::runtime_error("ERR_USERNOTINCHANNEL");

	findChannel->second.removeMember(findUser->second);
	std::string id = ":" + op->getNickname() + "!" + op->getRealname() + "@" + op->getHostname();
	std::string kickMessage = " KICK " + findChannel->second.getName() + " " + findUser->second->getNickname();
	std::string message = id + kickMessage;
	broadcast(findChannel->second, findUser->second, message);
}

void Server::kick(std::string nameChannel, std::string kicked, std::string reason, const User *op) {
	std::map<std::string, Channel>::iterator findChannel = _channels.find(nameChannel);
	if (findChannel == _channels.end())
		throw std::runtime_error("ERR_NOSUCHCHANNEL");

	std::map<int, User *>::const_iterator findUser = findChannel->second.getMembers().begin();
	for (; findUser != findChannel->second.getMembers().end(); ++findUser) {
		if (findUser->second->getNickname() == kicked)
			break;
	}
	if (findUser == findChannel->second.getMembers().end())
		throw std::runtime_error("ERR_NOTONCHANNEL");

	if (!findChannel->second.isOperator(op->getFd()))
		throw std::runtime_error("ERR_CHANOPRIVSNEEDED");
	if (!findChannel->second.isMember(findUser->second->getFd()))
		throw std::runtime_error("ERR_USERNOTINCHANNEL");

	findChannel->second.removeMember(findUser->second);
	std::string id = ":" + op->getNickname() + "!" + op->getRealname() + "@" + op->getHostname();
	std::string kickMessage = " KICK " + findChannel->second.getName() + " " + findUser->second->getNickname() + " :" + reason;
	std::string message = id + kickMessage;
	broadcast(findChannel->second, findUser->second, message);
}
