/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:02:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 15:31:56 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Parser.hpp"
#include <sys/socket.h>

void catchCommand(int sock, std::string line, DashData &data, Dashboard &dash)
{
	Parser parser;
	
	parser.parse(line);
	
	std::string command = parser.getCommand();
	std::vector<std::string> parameters = parser.getParameters();

	(void)data;
	if (command == "INVITE") {
		std::string channel = parameters[0];
		send(sock, ("JOIN " + channel + "\r\n").c_str(), 7 + channel.size(), 0);
		dash.log(CLIENT, ":RouxBot JOIN " + channel);
	}
	else if (command == "PRIVMSG") {
		dash.log(DEBUG, parser.getUserName() + " send : " + parser.getTrailing());
	}
}
