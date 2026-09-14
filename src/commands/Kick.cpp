/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:24:09 by afons             #+#    #+#             */
/*   Updated: 2026/09/14 16:37:14 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include <iostream>

void Server::kick(Channel &channel, User *kicked, std::string reason, const User *op) {	
	if (userNotOnChannel(*this, const_cast<User&>(*op), channel)
		|| notOperator(*this, const_cast<User&>(*op), channel)
		|| targetNotOnChannel(*this, const_cast<User&>(*op), channel, *kicked)
	)
		return ;

	// If every guards are OK, kick user
	channel.removeMember(kicked);
	
	// Build kick message
	std::string kickMessage = " KICK " + channel.getName() + " " + kicked->getNickname();
	std::string message = op->getPrefix() + kickMessage + " :";

	if (!reason.empty())
		message.append(reason);
	
	// Send message
	broadcast(channel, message);
	notification(kicked, message);
}
