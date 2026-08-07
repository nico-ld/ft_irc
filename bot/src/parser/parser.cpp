/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:02:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/07 16:17:27 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <sys/socket.h>

void catchCommand(int sock, std::string line, DashData &data, Dashboard &dash)
{
	Parser::parse(line);

	std::string command = Parser::getCommand();
	std::vector<std::string> parameters = Parser::getParameters();

	if (command == "INVITE") {
		std::string channel = parameters[0];
		send(sock, ("JOIN " + channel + "\r\n").c_str(), 7 + channel.size(), 0);
		dash.log(CLIENT, ":RouxBot JOIN " + channel);
	}
	else if (command == "PRIVMSG") {}
}
