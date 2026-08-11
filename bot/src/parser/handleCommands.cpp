/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:02:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 10:24:24 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void catchCommand(std::string line, t_bot_data &botData)
{
	Parser parser;
	
	parser.parseIRC(line);
	
	std::string command = parser.getCommand();
	std::vector<std::string> parameters = parser.getParameters();

	if (command == "INVITE") {
		std::string channel = parameters[0];
		send(botData.sock, ("JOIN " + channel + "\r\n").c_str(), 7 + channel.size(), 0);
		botData.dash->log(CLIENT, ":RouxBot JOIN " + channel);
	}
	else if (command == "PRIVMSG") {
		std::string trailing = parser.getTrailing();
		parser.parseMessage(trailing);

		botData.dash->log(DEBUG, "Parser catch command " + parser.getGameCmd());
		
		command = parser.getGameCmd();
		if (command == "!help") {
			handleHelp(botData.sock, botData.dash);
		}
		else if (command == "!game") {
			handleGame(botData);
		}
		else if (command == "!uno") {
			// handleUno
		}
	}
}
