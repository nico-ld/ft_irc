/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:46:21 by afons             #+#    #+#             */
/*   Updated: 2026/09/01 15:21:34 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

//Handle MODE command without argument neither option
std::string Server::displayChannelStatus(Channel &channel) {
	std::string status;
	std::string parameter;

	if (channel.isInviteOnly()) status += "i";
	if (channel.isTopicRestricted()) status += "t";
	if (channel.getKey().size()) {
		status += "k";
		parameter += channel.getKey() + " ";
	}
	if (channel.getUserLimit() != -1) {
		status += "l";
		std::stringstream ss;
		ss << channel.getUserLimit();
		parameter += ss.str()  + " ";
	}
	return "+" + status + " " + parameter;
}

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
				if ((*it_modestring)[i] == 'i') {
					channel.setInviteOnly(true);
					broadcast(channel, "Invite mode is activated\r\n");
				}
				// t : Restrict room topics to administrators choice
				else if ((*it_modestring)[i] == 't') {
					channel.setTopicRestricted(true);
					broadcast(channel, "Topic mode is activated\r\n");
				}

				// k : Set a room password
				else if ((*it_modestring)[i] == 'k') {
					if (params.size() <= 0) {
						sendReply(*user, ERR_NEEDMOREPARAMS, "Need more params");
						i++;
						continue ;
					}

					channel.setKey(*it_params);
					std::string message = "The new key of the channel is: " + *it_params+"\r\n";
					broadcast(channel, message);
					it_params++;
				}

				// l : Set a maximum user limit for the room
				else if ((*it_modestring)[i] == 'l') {
					if (params.size() <= 0)
						sendReply(*user, ERR_NEEDMOREPARAMS, "Limit of user need to be more than 0");

					std::stringstream ss(*it_params);
					int limit;
					ss >> limit;
					if (limit <= 0)
						sendReply(*user, ERR_UNKNOWNMODE, "Limit of user need to be more than 0");

					channel.setUserLimit(limit);
					std::string message = "The new limit of users for this channel is: " + *it_params + "\r\n";
					broadcast(channel, message);
					it_params++;
				}

				// o : Grant administrator/operator privileges to another user (requires user's name as parameter)
				else if ((*it_modestring)[i] == 'o') {
					if (params.size() <= 0)
						sendReply(*user, ERR_NEEDMOREPARAMS, "Need more params");
					channel.addOperator(getUserByNickname(*it_params));
					std::string message = *it_params + " has just been promoted as an operator\r\n";
					broadcast(channel, message);
				}
				// Reject unrecognized settings letter
				else {
					sendReply(*user, ERR_UNKNOWNMODE, "Invalid flag.");
					return ;
				}
				++i;
			}
		}

		// Handling letter setting for REMOVING, removing ends the rules defined by adding
		// so removing t, for exemple, means everyone can select a topic for the room
		else if ((*it_modestring)[i] == '-') {
			i++;
			while((*it_modestring)[i]) {
				if ((*it_modestring)[i] == 'i') {
					channel.setInviteOnly(false);
					broadcast(channel, "Invite mode is disabled\r\n");
				}
				else if ((*it_modestring)[i] == 't') {
					channel.setTopicRestricted(false);
					broadcast(channel, "Topic mode is disabled\r\n");
				}
				else if ((*it_modestring)[i] == 'k') {
					channel.setKey("");
					broadcast(channel, "Key has been deleted\r\n");
				}
				else if ((*it_modestring)[i] == 'l') {
					channel.setUserLimit(-1);
					broadcast(channel, "Limit mode is disabled\r\n");
				}
				else {
					sendReply(*user, ERR_UNKNOWNMODE, "Invalid flag.");
					return ;
				}
				i++;
			}
		}
	}
}

// Entry point for handling a user's request to change room settings.
// It verifies that the room exists, validates the user has permission, and triggers the settings update.
void Server::mode(Channel &channel, std::string listMode, User *user, std::vector<std::string> params) {
    // Verify that the room name uses the required formatting (must start with '#')
	if (!Parser::checkNameChannel(channel.getName())) {
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
