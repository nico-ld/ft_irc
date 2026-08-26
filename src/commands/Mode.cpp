/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:46:21 by afons             #+#    #+#             */
/*   Updated: 2026/08/26 11:22:46 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

// Breaks down a raw string of settings (like "+i-t+k") into individual action groups
// (like ["+i", "-t", "+k"]) so they can be processed one by one.
static std::vector<std::string> split_mode(std::string listMode) {
	std::vector<std::string> listString;
	std::string::iterator it = listMode.begin();

	while(it != listMode.end()) {
	    // Skip + and - to find the being pos of the letters
		while (it != listMode.end() && (*it == '+' || *it == '-')) { ++it; }
		std::string::iterator pos_tmp = it;
		// Skip until another + or - to find the end pos of the letters
		while (it != listMode.end() && *it != '+' && *it != '-') { ++it; }
		// Extract the letters part, to then happen it as a string to the list
		std::string string = listMode.substr(pos_tmp - listMode.begin() - 1, it - pos_tmp + 1);
		listString.push_back(string);
	}
	return listString;
}

// Applies each requested change in settings (mode) to a specific chat room (channel).
// Modes enable or disable room features like invite-only, passwords, or member limits.
void Server::launchMode(Channel &channel, std::vector<std::string> modestring, std::vector<std::string> params, User *user) {
	std::vector<std::string>::iterator it_modestring = modestring.begin();
	std::vector<std::string>::iterator it_params;
	if (params.size() > 0) it_params = params.begin();

	// Process letter setting for ADDING then REMOVING
	for(; it_modestring != modestring.end(); ++it_modestring) {
		size_t i = 0;

		// Handling letter setting for ADDING
		if ((*it_modestring)[i] == '+') {
			i++;
			while((*it_modestring)[i]) {
			    // i : Make the room invite-only
				if ((*it_modestring)[i] == 'i') channel.setInviteOnly(true);
				// t : Restrict room topics to administrators choice
				else if ((*it_modestring)[i] == 't') channel.setTopicRestricted(true);
				// k : Set a room password
				else if ((*it_modestring)[i] == 'k') {
					if (params.size() <= 0) {
						notification(user, "461 ERR_NEEDMOREPARAMS");
						throw std::runtime_error("need argument");
					}
					channel.setKey(*it_params);
					it_params++;
				}

				// l : Set a maximum user limit for the room
				else if ((*it_modestring)[i] == 'l') {
					if (params.size() <= 0) {
						notification(user, "461 ERR_NEEDMOREPARAMS");
						throw std::runtime_error("need argument");
					}

					std::stringstream ss(*it_params);
					int limit;
					ss >> limit;
					channel.setUserLimit(limit);
					it_params++;
				}

				// o : Grant administrator/operator privileges to another user (requires user's name as parameter)
				else if ((*it_modestring)[i] == 'o') {
					if (params.size() <= 0) {
						notification(user, "461 ERR_NEEDMOREPARAMS");
						throw std::runtime_error("need argument");
					}
					channel.addOperator(user);
				}
				// Reject unrecognized settings letter
				else {
					notification(user, "This mode doesn't exist.");
					throw std::runtime_error("This mode doesn't exist");
				}
				++i;
			}
		}

		// Handling letter setting for REMOVING, removing ends the rules defined by adding
		// so removing t, for exemple, means everyone can select a topic for the room
		else if ((*it_modestring)[i] == '-') {
			while((*it_modestring)[i]) {
				i++;

				if ((*it_modestring)[i] == 'i') channel.setInviteOnly(false);
				else if ((*it_modestring)[i] == 't') channel.setTopicRestricted(false);
				else if ((*it_modestring)[i] == 'k') channel.setKey("");
				else if ((*it_modestring)[i] == 'l') channel.setUserLimit(-1);
				else {
					notification(user, "This mode doesn't exist.");
					throw std::runtime_error("This mode doesn't exist");
				}
			}
		}
	}
}

// Entry point for handling a user's request to change room settings.
// It verifies that the room exists, validates the user has permission, and triggers the settings update.
void Server::mode(Channel &channel, std::string listMode, User *user, std::vector<std::string> params, Parser &parser) {
    // Verify that the room name uses the required formatting (must start with '#')
	if (!parser.checkChannelName(channel.getName())) {
		notification(user, "Name channel must start with #");
		throw std::runtime_error("[LOG] Name channel must start with #");
	}

	// Ensure the requested room actually exists on the server
	std::map<std::string, Channel>::iterator it = _channels.find(channel.getName());
	if (it == _channels.end()) {
		notification(user, "403 ERR_NOSUCHCHANNEL");
		throw std::runtime_error("[LOG] Channel doesn't exist");
	}

	// Ensure the user requesting changes is a room administrator
	if (!channel.isOperator(user->getFd())) {
		notification(user, "482 ERR_CHANOPRIVSNEEDED");
		throw std::runtime_error("[LOG] You're not channel operator");
	}

	// Break down the settings string into manageable pieces
	std::vector<std::string> modestring = split_mode(listMode);

	// Apply the parsed settings to the room
	launchMode(channel, modestring, params, user);
}
