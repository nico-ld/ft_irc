/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGame.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 17:17:07 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 11:16:38 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Uno.hpp"

void handleGame(t_bot_data &botData, std::string channel, std::string command)
{
	// Get value from parser
	std::vector<std::string> param = botData.parser.getGameCmdParam();

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
		else if (param[1] == "werewolf" || param[1] == "ww")
			sendMessage(botData, channel, "Sorry about that, Werewolf is not handled for the moment. But you can play Uno !", CLIENT);

		// Unknow parameter
		else
			sendMessage(botData, channel, "Unknow parameter for !game create : " + param[1], WARNING);
	}
	else {
		// Other commands
		Game *currentGame = getCurrentGame(botData, channel);
		
		if (!currentGame) {
			sendMessage(botData, channel, "There is no game in this channel", WARNING);
			return ;
		}
		
		if (command == "join")
			currentGame->addPlayer(botData.parser.getUserName(), botData);
		else if (command == "leave")
			currentGame->removePlayer(botData.parser.getUserName(), botData, false);
		else if (command == "kick")
			currentGame->removePlayer(param[1], botData, true);
		else if (command == "start")
			currentGame->startGame(botData);
		else if (command == "end")
			currentGame->endGame(botData);
		else
			sendMessage(botData, channel, "Unknow command : " + command, WARNING);
	}
}
