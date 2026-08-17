/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUno.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 10:52:26 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 11:10:27 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void handleUno(t_bot_data &botData, std::string channel, std::string command) {
	// Get value from parser
	std::string userName = botData.parser.getUserName();
	std::vector<std::string> param = botData.parser.getGameCmdParam();

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
	std::vector<std::string>::iterator itPlayer = std::find(playerList.begin(), playerList.end(), userName);
	if (itPlayer == playerList.end()) {
		sendMessage(botData, channel, "You're not in the game " + userName, WARNING);
		return ;
	}


	// Command for every player
	if (command == "turn")
		currentGame->whoseTurn(botData);
	else if (command == "hand")
		currentGame->showHand(botData, userName);
	else if (command == "uno")
		currentGame->uno(botData, userName);
	else if (command == "cards")
		currentGame->showCardsAmount(botData);

	// Check if this is the good player who play
	if (userName != currentGame->getPlayerTurn()) {
		sendMessage(botData, channel, "This is not you're turn " + userName, WARNING);
		return ;
	}

	t_player_info player = currentGame->getPlayerInfo(userName);
	
	// Command for current player
	// If player just drew a wild card
	if (player.wildDrawed) {
		if (command == "play" || command == "draw" || command == "challenge") {
			sendMessage(botData, channel, "You have to choose a color with '!uno color <color>", WARNING);
			return ;
		}
		else if (command == "color")
			currentGame->color(botData, player, param[1]);
	}
	
	if (command == "play") {
		if (param.size() == 3)
			currentGame->playCard(botData, userName, param[1], param[2]);
		else
			currentGame->playCard(botData, userName, param[1], "");
	}
	else if (command == "draw")
		currentGame->draw(botData, currentGame->getPlayerInfo(userName));
	else if (command == "challenge")
		currentGame->challenge(botData, userName);
	else
		sendMessage(botData, channel, "Unknow command : " + command, WARNING);
}
