/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 12:07:19 by nico              #+#    #+#             */
/*   Updated: 2026/08/26 13:38:53 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include <cstdio>

void Server::sendReply(const User &user, const std::string &code, const std::string &rest) {
	std::string nick = user.getNickname();
	if (nick.empty())
		nick = "*";
	
	std::string msg = ":ircserv " + code + " " + nick + " " + rest + "\r\n";
	if (send(user.getFd(), msg.c_str(), msg.size(), MSG_NOSIGNAL) == -1)
		std::perror("send crashed");
}
