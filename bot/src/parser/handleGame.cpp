/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGame.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 17:17:07 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 15:52:29 by nico             ###   ########.fr       */
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
				botData.dash->log(WARNING, "There is already a game that running in this channel.");
				return ;
			}
		}

		// if wrong amount of parameter is given
		if (param.size() != 2)
			botData.dash->log(WARNING, (param.size() == 1 ) ? "Missing argument for !game create" : "Too many argument for !game create");
		
		// Uno case
		else if (param[1] == "uno") {
			Game *newGame = new Uno(botData.sock, channel);
			newGame->initGame(botData.parser.getUserName(), botData);
			botData.games.push_back(newGame);
		}

		// Unknow parameter
		else
			botData.dash->log(WARNING, "Unknow parameter for !game create : " + param[1]);
	}

	// Other commands, for a better code only one for loop instead of one loop by if
	else if (command == "join" || command == "leave" || command == "start" || command == "end") {
		for (std::vector<Game *>::iterator it = botData.games.begin(); it != botData.games.end(); ++it) {
			if (channel == (*it)->getChannel()) {
				if (command == "join")
					(*it)->addPlayer(botData.parser.getUserName(), botData);
				else if (command == "leave")
					(*it)->removePlayer(botData.parser.getUserName(), botData);
				else if (command == "start")
					(*it)->startGame(botData);
				else
					(*it)->endGame(botData);
				return ;
			}
		}
		// If the channel doesn't exist -> in fact this is impossible but handled
		botData.dash->log(WARNING, "the channel " + channel + "doesn't exist");
	}
	else
		botData.dash->log(WARNING, "unknow command for !game : " + command);
}
