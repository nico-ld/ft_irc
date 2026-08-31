/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 12:07:19 by nico              #+#    #+#             */
/*   Updated: 2026/08/31 11:39:58 by nico             ###   ########.fr       */
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
	// Routed through the buffered write path (see ServerHelper.cpp) instead of
	// a raw send(), so a slow client doesn't silently lose the reply.
	queueWrite(const_cast<User &>(user), msg);
	dash->log(SERVER, msg);
}
