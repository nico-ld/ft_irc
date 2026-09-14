/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandGuards.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:48:10 by nico              #+#    #+#             */
/*   Updated: 2026/09/14 16:25:10 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>

bool missingParam(Server &server, User &user, std::string command,
					std::vector<std::string> params, size_t expected)
{
	if (params.size() < expected) {
		std::transform(command.begin(), command.end(), command.begin(), ::toupper);
		server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter(s) for " + command + " command");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Missing parameter(s) for " + command + " command");
		return (true);
	}
	else
		return (false);
}

bool notRegistered(Server &server, User &user) {
	if (user.hasProvidedPassword() == false) {
		server.sendReply(user, ERR_NOTREGISTERED, "You're not registered, please provide the password");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Not registered, password not provided");
		return (true);
	}
	else
		return (false);
}

bool notFullyAuthenticated(Server &server, User &user) {
	if (notRegistered(server, user))
		return (true);
	else if (user.hasProvidedNick() == false) {
		server.sendReply(user, ERR_NOTREGISTERED, "You're not fully authenticated, please provide a nickname");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": User not authenticated: missing nickname");
		return (true);
	}
	else if (user.hasProvidedUser() == false) {
		server.sendReply(user, ERR_NOTREGISTERED, "You're not fully authenticated, please provide a realname");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": User not authenticated: missing realname");
		return (true);
	}
	else
		return (false);
}

bool alreadyAuthenticated(Server &server, User &user) {
	if (user.isAuthenticated() == true) {
		server.sendReply(user, ERR_NOTREGISTERED, "You're already authenticated");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": User already authenticated");
		return (true);
	}
}

bool channelNotExist(Server &server, User &user, Channel *channel, std::string chanName) {
	if (!channel) {
		server.sendReply(user, ERR_NOSUCHCHANNEL, "Channel '" + chanName + "' doesn't exist");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Channel '" + chanName + "' doesn't exist");
		return (true);
	}
	else
		return (false);
}

bool userNotExist(Server &server, User &user, User *expected, std::string userName) {
	if (!expected) {
		server.sendReply(user, ERR_NOSUCHNICK, "User '" + userName + "' doesn't exist");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": User '" + userName + "' doesn't exist");
		return (true);
	}
	else
		return (false);
}

bool userNotOnChannel(Server &server, User &user, Channel &channel) {
	if (!channel.isMember(user.getFd())) {
		server.sendReply(user, ERR_NOTONCHANNEL, "You're not on this channel");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": User not on channel '" + channel.getName() + "'");
		return (true);
	}
	else
		return (false);
}

bool targetNotOnChannel(Server &server, User &user, Channel &channel, User &target) {
	if (!channel.isMember(target.getFd())) {
		server.sendReply(user, ERR_USERNOTINCHANNEL, "User '" + target.getNickname() + "' isn't on this channel");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Target not the same channel than sender");
		return (true);
	}
	else
		return (false);
}

bool notOperator(Server &server, User &user, Channel &channel) {
	if (!channel.isOperator(user.getFd())) {
		server.sendReply(user, ERR_CHANOPRIVSNEEDED, "You need operator privilege to do this");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Doesn't get operator privilege");
		return (true);
	}
	else
		return (false);
}

bool missingFlagParameter(Server &server, User &user, std::string flag,
							std::vector<std::string> list, std::vector<std::string>::iterator current)
{
	if (list.size() <= 0 || current == list.end()) {
		server.sendReply(user, ERR_NEEDMOREPARAMS, "Missing parameter for " + flag + " MODE flag");
		server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ": Missing parameter for " + flag + " MODE flag");
		return (true);
	}
	else
		return (false);
}
