/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/09/02 15:20:00 by nico             ###   ########.fr       */
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
			server.dash->log(WARNING, "Empty or Invalid prefix");
			return ;
			
		case 2: // Command unknow
			server.dash->log(WARNING, "Command unknow : " + command);
			server.sendReply(user, ERR_UNKNOWNCOMMAND, "Command " + command + " is unknow");
			return ;
			
		case 3: // Error while trying to get param
			server.dash->log(WARNING, "Impossible to get command parameters");
			server.sendReply(user, ERR_NEEDMOREPARAMS, "Impossible to get command parameters, please try again");
			return ;
		
		default:
			break;
	}

	command = parser.getCommand();
	
	// === QUIT ===
	if (command == "quit") {
		server.removeUser(user.getFd(), user.getPrefix() + " " + parser.getRawString());
		return ;
	}


	// Dispatch and handle command
	switch (parser.getCommandId())
	{
		case 1:
			if (user.isAuthenticated())
				channelCommandsDispatch(server, command, user, parser);
			else {
				server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", User is not registered yet");
				server.sendReply(user, ERR_NOTREGISTERED, "User is not registered yet");
			}
			break ;
		case 2:
			if (user.isAuthenticated())
				messageCommandsDispatch(server, command, user, parser);
			else {
				server.dash->log(WARNING, "Fd : " + toStr(user.getFd()) + ", User is not registered yet");
				server.sendReply(user, ERR_NOTREGISTERED, "User is not registered yet");
			}
			break ;
		case 3:
			userCommandsDispatch(command, user, server, parser);
			break ;

		default:
			server.dash->log(ERROR_LVL, "How the fuck did you get here ?? This is mathematically impossible ! XoX");
			break ;
	}
}