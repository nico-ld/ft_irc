/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserHelper.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 09:11:31 by nico              #+#    #+#             */
/*   Updated: 2026/08/31 17:30:04 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp" // absolute path to avoid conflict with bot Parser
#include "Server.hpp"
#include "Channel.hpp"

std::vector<Channel> Parser::getChannelList(std::string parameter, Server &server, User &user) {
	std::vector<Channel> channelList;
	size_t search = 0;
	size_t pos;
	
	// Loop to split every channel name on ','
	while ((pos = parameter.find(',', search)) != std::string::npos) {
		std::string currentName = parameter.substr(search, pos - search);
		
		if (checkChannelName(currentName) == true)
			channelList.push_back(Channel(currentName));
		else {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Invalid channel name : " + currentName);
			server.sendReply(user, ERR_NOSUCHCHANNEL, "Invalid channel name : " + currentName);
		}
		search = pos + 1;
	}

	// Get the last channel name
	if (search < parameter.size()) {
		std::string currentName = parameter.substr(search);
		if (checkChannelName(currentName) == true)
			channelList.push_back(Channel(currentName));
		else {
			server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Invalid channel name : " + currentName);
			server.sendReply(user, ERR_NOSUCHCHANNEL, "Invalid channel name : " + currentName);
		}
	}

	return (channelList);
}

std::vector<std::string> Parser::getKeyList(std::string parameter) {
	std::vector<std::string> listKey;
	size_t search = 0;
	size_t pos;
	
	// Loop to split every key on ','
	while ((pos = parameter.find(',', search)) != std::string::npos) {
		std::string str = parameter.substr(search, pos - search);
		listKey.push_back(str);
		search = pos + 1;
	}
	
	// Get the last key
	if (search < parameter.size()) {
		std::string str = parameter.substr(search);
		listKey.push_back(str);
	}
	
	return (listKey);
}

std::string Parser::getMessage( void ) {
	std::vector<std::string>::iterator it;
	std::string message;

	// Skip message target and get every other parameter
	if (_parameters.size() > 0) {
		for (it = _parameters.begin() + 1; it != _parameters.end(); ++it) {
			if (!message.empty())
				message.append(" ");
			message.append(*it);
		}
	}
		
	// If the trailing is not empty, add it to the message
	if (!_trailing.empty()) {
		if (!message.empty())
			message.append(" ");
		message.append(_trailing);
	}

	return (message);
}

bool Parser::checkChannelName(std::string name) {
	if (name.empty())
		return (false);
	else if (name[0] != '#')
		return (false);
	else if (name.size() <= 1)
		return (false);
	return (true);
}
