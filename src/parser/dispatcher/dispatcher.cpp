/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/08/29 11:32:05 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

void dispatchCommand(Server &server, User &user, std::string command) {
	Parser parser;
	int flag = parser.parse(command); // Parse command
	
	switch (flag)
	{
		case 1: // Prefix empty or invalid
			// Add log (no numeric reply for this error)
			return ;
			
		case 2: // Command unknow
			server.sendReply(user, ERR_UNKNOWNCOMMAND, "Command " + command + " is unknow");
			return ;
			
		case 3: // Error while trying to get param
			// Add log (no numeric reply for this error)
			return ;
		
		default:
			break;
	}


	// Dispatch and handle command
	switch (parser.getCommandId())
	{
		case 1:
			if (user.isAuthenticated())
				channelCommandsDispatch(server, command, user, parser);
			else
				server.sendReply(user, ERR_NOTREGISTERED, "User is not registered yet");
			break ;
		case 2:
			if (user.isAuthenticated())
				messageCommandsDispatch(server, command, user, parser);
			else
				server.sendReply(user, ERR_NOTREGISTERED, "User is not registered yet");
			break ;
		case 3:
			userCommandsDispatch(command, user, server, parser);
			break ;

		default:
			// Impossible case, but Add log
			break ;
	}
}