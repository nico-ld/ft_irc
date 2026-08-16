/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUno.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 10:52:26 by nico              #+#    #+#             */
/*   Updated: 2026/08/16 16:07:48 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void handleUno(t_bot_data &botData) {
	// Get value from parser
	std::string channel = botData.parser.getParameters()[0];
	std::string userName = botData.parser.getUserName();
	std::vector<std::string> param = botData.parser.getGameCmdParam();
	std::string command = param[0];

	Uno *currentGame = static_cast<Uno *>(getCurrentGame(botData, channel));
	
	// Check if the game have started
	if (!currentGame || currentGame->getGameState() != STARTED) {
		if (!currentGame)
			sendMessage(botData, channel, "There is no Uno in this channel.", WARNING);
		else
			sendMessage(botData, channel, "The Uno not start yet.", WARNING);
		return ;
	}

	// Check if player is in the game
	std::vector<std::string> playerList = currentGame->getPlayerList();
	std::vector<std::string>::iterator player = std::find(playerList.begin(), playerList.end(), userName);
	if (player == playerList.end()) {
		sendMessage(botData, channel, "You're not in the game " + userName, WARNING);
		return ;
	}

	if (command == "turn")
		currentGame->whoseTurn(botData);
	else if (command == "play") {
		if (param.size() == 3)
			currentGame->playCard(botData, userName, param[1], param[2]);
		else
			currentGame->playCard(botData, userName, param[1], "");
	}
	else if (command == "hand")
		currentGame->showHand(botData, userName);
}
