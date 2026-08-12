/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGame.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 17:17:07 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 11:18:52 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Uno.hpp"

void handleGame(t_bot_data &botData)
{
	// Get value from parser
	std::string channel = botData.parser.getParameters()[0];
	std::vector<std::string> param = botData.parser.getGameCmdParam();
	std::string command = param[0];

	if (command == "create") {
		// Check if there is already a game in this channel
		for (std::vector<Game *>::iterator it = botData.games.begin(); it != botData.games.end(); ++it) {
			if ((*it)->getChannel() == channel && (*it)->getGameState() != ENDED) {
				sendMessage(botData, channel, "There is already a game that running in this channel.", WARNING);
				return ;
			}
		}

		// if wrong amount of parameter is given
		if (param.size() != 2)
			sendMessage(botData, channel, (param.size() == 1) ? "Missing argument for !game create" : "Too many argument for !game create", WARNING);
		
		// Uno case
		else if (param[1] == "uno") {
			Game *newGame = new Uno(botData.sock, channel);
			newGame->initGame(botData.parser.getUserName(), botData);
			botData.games.push_back(newGame);
		}

		// Unknow parameter
		else
			sendMessage(botData, channel, "Unknow parameter for !game create : " + param[1], WARNING);
	}

	// Other commands, for a better code only one for loop instead of one loop by if
	else if (command == "join" || command == "leave" || command == "start" || command == "end" || command == "kick") {
		for (std::vector<Game *>::iterator it = botData.games.begin(); it != botData.games.end(); ++it) {
			if (channel == (*it)->getChannel()) {
				if (command == "join")
					(*it)->addPlayer(botData.parser.getUserName(), botData);
				else if (command == "leave")
					(*it)->removePlayer(botData.parser.getUserName(), botData, false);
				else if (command == "kick")
					(*it)->removePlayer(param[1], botData, true);
				else if (command == "start")
					(*it)->startGame(botData);
				else
					(*it)->endGame(botData);
				return ;
			}
		}
		// If the channel doesn't exist -> in fact this is impossible but handled
		botData.dash->log(ERROR_LVL, "the channel " + channel + "doesn't exist");
	}
	else
		sendMessage(botData, channel, "Unknow command for !game : " + command, WARNING);
}
