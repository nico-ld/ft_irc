/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:10:53 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 17:23:47 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Bot.hpp"

void Game::initGame(std::string userName, t_bot_data &botData) {
	_playerList.push_back(userName);
	_gameState = WAITING;
	_timeSinceEnd = -1;

	sendMessage(botData, _channel, convertType(_gameType) + " has been created by " + userName + "!", INFO);
	sendMessage(botData, _channel, "Join the game with '!game join', or start it with '!game start'", CLIENT);

	// Update information in dashboard
	// Add game in element list of GAME section
	botData.dash->addElem(botData.dash->getSectionByIndex(2), LEFT, addGame(_channel, "1", "WAITING"));

	botData.dash->increaseInfo(botData.dash->getSectionByIndex(2), LEFT, 0); // Increase game amount for Uno
	botData.dash->increaseInfo(botData.dash->getSectionByIndex(2), LEFT, 1); // Increase player amount for Uno
	botData.dash->increaseInfo(botData.dash->getSectionByIndex(1), LEFT, 1); // Increase games started for BOT
	botData.dash->increaseInfo(botData.dash->getSectionByIndex(1), LEFT, 2); // Increase player amount for BOT
}

void Game::setGameState(e_state state, t_bot_data &botData) {
	_gameState = state;

	// Update information in dashboard
	std::vector<INFO_LIST> *elemList = botData.dash->getElemList(botData.dash->getSectionByIndex(2), LEFT);
	if (!elemList)
		return ;

	for (size_t index = 0; index < elemList->size(); ++index) {
		if ((*elemList)[index][0].first == _channel) {
			(*elemList)[index][2].second = convertState(state);
			break ;
		}
	}

	botData.dash->render();
}

void Game::startGame(t_bot_data &botData) {
	// Check if player is in the game
	if (!isPlayerInGame(botData.parser.getUserName(), _playerList)) {
		sendMessage(botData, _channel, "Sorry " + botData.parser.getUserName() + " you can't start a game in wich you're not.", CLIENT);
		return ;
	}

	// Check if game can start
	if (_gameState != READY) {
		if (_gameState == ENDED)
			sendMessage(botData, _channel, "Cannot start a game that just end.", CLIENT);
		else
			sendMessage(botData, _channel, "Not enought player to start the game.", CLIENT);
	}

	// Set the game as started
	setGameState(STARTED, botData);

	// Start the Game
	launchGame(botData);
}

void Game::endGame(t_bot_data &botData) {
	if (!_playerList.empty() && !isPlayerInGame(botData.parser.getUserName(), _playerList)) {
		sendMessage(botData, _channel, "Sorry " + botData.parser.getUserName() + " you can't end a game in wich you're not.", CLIENT);
		return ;
	}

	// Check if the Game is already ENDED
	if (_timeSinceEnd != -1)
		return ;

	setGameState(ENDED, botData);
	_timeSinceEnd = std::time(NULL);

	botData.dash->decreaseInfo(botData.dash->getSectionByIndex(1), LEFT, 1); // Decrease Game amount for BOT
	botData.dash->decreaseInfo(botData.dash->getSectionByIndex(1), LEFT, 2); // Decrease Player amount for BOT
	
	botData.dash->log(INFO, convertType(_gameType) + " just end in " + _channel);
}
