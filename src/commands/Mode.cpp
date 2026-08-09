/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:46:21 by afons             #+#    #+#             */
/*   Updated: 2026/08/03 18:43:40 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

static std::vector<std::string> split_mode(std::string listMode) {
	std::vector<std::string> listString;
	std::string::iterator it = listMode.begin();

	while(it != listMode.end()) {
		while (it != listMode.end() && (*it == '+' || *it == '-')) {
			++it;
		}
		std::string::iterator pos_tmp = it;
		while (it != listMode.end() && *it != '+' && *it != '-') {
			++it;
		}
		std::string string = listMode.substr(pos_tmp - listMode.begin() - 1, it - pos_tmp + 1);
		listString.push_back(string); 
	}
	return listString;
}

static void launchMode(Channel &channel, std::vector<std::string> modestring, std::vector<std::string> params) {
	std::vector<std::string>::iterator it_modestring = modestring.begin();
	std::vector<std::string>::iterator it_params;
	if (params.size() > 0)
		it_params = params.begin();

	for(; it_modestring != modestring.end(); ++it_modestring) {
		if (*it_modestring == "+") {
			*it_modestring++;

			if (*it_modestring == "i")
				channel.setInviteOnly(true);
			else if (*it_modestring == "t")
				channel.setTopicRestricted(true);
			else if (*it_modestring == "k") {
				if (params.size() <= 0)
					throw std::runtime_error("need argument");
	
				channel.setKey(*it_params);
				it_params++;
			}
			else if (*it_modestring == "l") {
				if (params.size() <= 0)
					throw std::runtime_error("need argument");

				std::stringstream ss(*it_params);
				int limit;
				ss >> limit;
				channel.setUserLimit(limit);
				it_params++;
			}
			else
				throw std::runtime_error("This mode doesn't existe");
		}
	}
}

void Server::mode(Channel &channel, std::string listMode, std::vector<std::string> params, User *user) {
	(void)user;

	std::map<std::string, Channel>::iterator it = _channels.find(channel.getName());
	if (it == _channels.end())
		throw std::runtime_error("ERR_NOSUCHCHANNEL");

	//PARSING
	std::vector<std::string> modestring = split_mode(listMode);

	// LAUNCH MODE
	launchMode(channel, modestring, params);
}

void Server::mode(Channel &channel, std::string listMode, User *user) {
	(void)user;

	std::map<std::string, Channel>::iterator it = _channels.find(channel.getName());
	if (it == _channels.end())
		throw std::runtime_error("ERR_NOSUCHCHANNEL");

	//PARSING
	std::vector<std::string> modestring = split_mode(listMode);
	std::vector<std::string> params;
	// LAUNCH MODE
		launchMode(channel, modestring, params);
}
