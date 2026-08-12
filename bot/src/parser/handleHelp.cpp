/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleHelp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 16:24:03 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 10:58:25 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void handleHelp(t_bot_data &botData) {
	std::string channel = botData.parser.getParameters()[0];
	
	std::string header = "Menu help of RouxBot !\n";
	std::string cmdAvailable = "Command available : !help, !game, !uno\n";
	std::string gamePart = "Game command, to create and manage games : \n\t";
	std::string gameCmd = "- create <name> : To create a new game (e.g. : !game create uno)\n\t- join : join the game in your channel\n\t- start : Start the game when there is enought players\n\t";
	std::string gameCmd2 = "- leave : Leave the current lobby \n\t- kick <player> : Kick someone from the game";
	
	// Concatenate message
	std::string message;
	message.append(header + cmdAvailable + gamePart + gameCmd + gameCmd2);
	
	sendMessage(botData, channel, message, CLIENT);

	std::string unoPart = "Uno command, to play uno : \n\t";
	std::string unoCmd = "- rules : Show Uno rules \n\t- hand : Show your hand (by Private message) \n\t- play <card> : Play a card \n\t- challenge : Challenge a wild draw 4 card\n\t- color : Choose the color after a wild";

	// Concatenate message
	message.clear();
	message.append(unoPart + unoCmd);

	sendMessage(botData, channel, message, CLIENT);
}
