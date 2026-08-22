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
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <climits>

// Breaks down a raw string of settings (like "+i-t+k") into individual action groups
// (like ["+i", "-t", "+k"]) so they can be processed one by one.
// Every group returned is guaranteed to start with '+' or '-' followed by >= 1 letter.
// A leading group with no sign (e.g. "it") is treated as "+it", as real IRC servers do.
static std::vector<std::string> split_mode(const std::string &listMode) {
	std::vector<std::string> listString;
	std::string::size_type i = 0;
	char sign = '+';

	while (i < listMode.size()) {
		// Consume the signs, so "+-i" only retains the last sign seen, -
		while (i < listMode.size() && (listMode[i] == '+' || listMode[i] == '-')) {
			sign = listMode[i];
			++i;
		}
		// Collect the letters up to the next sign
		std::string::size_type start = i;
		while (i < listMode.size() && listMode[i] != '+' && listMode[i] != '-') { ++i; }
		if (i > start)
			listString.push_back(std::string(1, sign) + listMode.substr(start, i - start));
	}
	return listString;
}

// Parses the numeric argument of +l. Returns false on anything that is not a
// strictly positive integer ("abc", "12x", "", "-5", "0", overflow).
static bool parse_limit(const std::string &s, int &out) {
	if (s.empty()) return false;
	char *end = NULL;
	long v = std::strtol(s.c_str(), &end, 10);
	if (*end != '\0' || v <= 0 || v > INT_MAX) return false;
	out = static_cast<int>(v);
	return true;
}

// Applies each requested change in settings (mode) to a specific chat room (channel).
// Modes enable or disable room features like invite-only, passwords, or member limits.
//
// This is done in two passes so a request is all-or-nothing:
//   1. validate every flag and every parameter without touching the channel
//   2. apply them, which can no longer fail
void Server::launchMode(Channel &channel, std::vector<std::string> modestring, std::vector<std::string> params, User *user) {
	std::vector<std::string>::const_iterator it_params;
	std::vector<std::string>::const_iterator it_group;

	/* ---------- validate-only phase ---------- */
	it_params = params.begin();
	for (it_group = modestring.begin(); it_group != modestring.end(); ++it_group) {
		bool adding = ((*it_group)[0] == '+');

		for (size_t i = 1; i < it_group->size(); ++i) {
			char c = (*it_group)[i];

			// Reject unrecognized settings letter before anything has been applied
			if (c != 'i' && c != 't' && c != 'k' && c != 'l' && c != 'o') {
				notification(user, "472 ERR_UNKNOWNMODE");
				throw std::runtime_error("[LOG] This mode doesn't exist");
			}

			// +k, +l, +o and -o consume one parameter; the rest consume none
			bool needsParam = (c == 'o') || (adding && (c == 'k' || c == 'l'));
			if (!needsParam) continue;

			if (it_params == params.end()) {
				notification(user, "461 ERR_NEEDMOREPARAMS");
				throw std::runtime_error("[LOG] need argument");
			}

			// l : argument must be a valid positive number
			if (c == 'l') {
				int tmp;
				if (!parse_limit(*it_params, tmp)) {
					notification(user, "461 ERR_NEEDMOREPARAMS");
					throw std::runtime_error("[LOG] invalid user limit");
				}
			}

			// o : argument must be the nickname of a user who is in the channel
			if (c == 'o') {
				User *target = getUserByNickname(*it_params);
				if (!target) {
					notification(user, "401 ERR_NOSUCHNICK");
					throw std::runtime_error("[LOG] No such nick");
				}
				if (!channel.isMember(target->getFd())) {
					notification(user, "441 ERR_USERNOTINCHANNEL");
					throw std::runtime_error("[LOG] User not in channel");
				}
			}
			++it_params;
		}
	}

	/* ---------- apply-only phase (cannot fail anymore) ---------- */
	it_params = params.begin();
	for (it_group = modestring.begin(); it_group != modestring.end(); ++it_group) {
		bool adding = ((*it_group)[0] == '+');

		for (size_t i = 1; i < it_group->size(); ++i) {
			char c = (*it_group)[i];

			// i : Make the room invite-only (or not)
			if (c == 'i') channel.setInviteOnly(adding);

			// t : Restrict room topics to operators (or not)
			else if (c == 't') channel.setTopicRestricted(adding);

			// k : Set / clear the room password
			else if (c == 'k') {
				if (adding) { channel.setKey(*it_params); ++it_params; }
				else        { channel.setKey(""); }
			}

			// l : Set / clear the maximum number of users
			else if (c == 'l') {
				if (adding) {
					int limit = 0;
					parse_limit(*it_params, limit); // already validated in pass 1
					channel.setUserLimit(limit);
					++it_params;
				}
				else channel.setUserLimit(-1);
			}

			// o : Grant / revoke operator privileges to the TARGET user (not the sender)
			else if (c == 'o') {
				User *target = getUserByNickname(*it_params); // validated in pass 1
				if (adding) channel.addOperator(target);
				else        channel.removeOperator(target);
				++it_params;
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

	// Apply the parsed settings to the room (validated first, then applied atomically)
	launchMode(channel, modestring, params, user);
}
