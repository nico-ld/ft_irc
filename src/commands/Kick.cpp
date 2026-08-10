/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:24:09 by afons             #+#    #+#             */
/*   Updated: 2026/08/03 17:00:34 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>

void Server::kick(Channel &channel, User *kicked, const User *op) {
	if (!Parser::checkNameChannel(channel.getName()))
			throw std::runtime_error("Name channel must start with #");

	std::map<std::string, Channel>::iterator findChannel = _channels.find(channel.getName());
	if (findChannel == _channels.end())
		throw std::runtime_error("ERR_NOSUCHCHANNEL");

	std::map<int, User *>::const_iterator findUser = findChannel->second.getMembers().begin();
	for (; findUser != findChannel->second.getMembers().end(); ++findUser) {
		if (findUser->second->getNickname() == kicked->getNickname())
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

void Server::kick(Channel &channel, User *kicked, std::string reason, const User *op) {
	if (!Parser::checkNameChannel(channel.getName()))
			throw std::runtime_error("Name channel must start with #");

	std::map<std::string, Channel>::iterator findChannel = _channels.find(channel.getName());
	if (findChannel == _channels.end())
		throw std::runtime_error("ERR_NOSUCHCHANNEL");

	std::map<int, User *>::const_iterator findUser = findChannel->second.getMembers().begin();
	for (; findUser != findChannel->second.getMembers().end(); ++findUser) {
		if (findUser->second->getNickname() == kicked->getNickname())
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
