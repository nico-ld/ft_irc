/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GamePlayer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:05:05 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 17:25:31 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Bot.hpp"

void Game::addPlayer(std::string playerName, t_bot_data &botData) {
	// Check if game accept new player
	if (_gameState != READY && _gameState != WAITING) {
		sendMessage(botData, _channel, "Game is in " + convertState(_gameState) + " state. You can't join it anymore.", WARNING);
		return ;
	}

	// Check if player is already in the game
	if (isPlayerInGame(playerName, _playerList)) {
		sendMessage(botData, _channel, "You are already in the game.", WARNING);
		return ;
	}
	
	// Add player in the game
	_playerList.push_back(playerName);
	_gameState = READY;
	
	// Log the new update
	sendMessage(botData, _channel, playerName + " join the " + convertType(_gameType) + " !", INFO);

	// Update Dashboard
	std::vector<INFO_LIST> *elemList = botData.dash->getElemList(botData.dash->getSectionByIndex(2), LEFT);
	if (!elemList)
		return ;

	for (size_t index = 0; index < elemList->size(); ++index) {
		if ((*elemList)[index][0].first == _channel) {
			(*elemList)[index][2].second = convertState(READY);
			(*elemList)[index][1].second = toStr(toInt((*elemList)[index][1].second) + 1);
			break ;
		}
	}

	botData.dash->increaseInfo(botData.dash->getSectionByIndex(1), LEFT, 2); // Increase player amount for BOT
	botData.dash->increaseInfo(botData.dash->getSectionByIndex(1), LEFT, 2); // Increase player amount for BOT
	botData.dash->render();
}

void Game::removePlayer(std::string playerName, t_bot_data &botData, bool isKicked) {
	// Find player in the list
	std::vector<std::string>::iterator it = _playerList.begin();
	for ( ; it != _playerList.end(); ++it) {
		if (*it == playerName)
			break ;
	}
	
	// If player not found, player has not joined the game 
	if (it == _playerList.end()) {
		sendMessage(botData, _channel, (isKicked) ? "You can't kick who is not in the game" : "You're not in the game, you can't leave.", WARNING);
		return ;
	}
	
	// If player found, remove it 
	_playerList.erase(it);
	
	// Log the update
	sendMessage(botData, _channel, (isKicked) ? playerName + " has been kicked of the game." : playerName + " leave the " + convertType(_gameType) + " !", INFO);
	
	// If game state have to be updated
	// If 1 player remaining
	if (_playerList.size() == 1) {
		// If game was on READY, not anymore enough player
		if (_gameState == READY)
			setGameState(WAITING, botData);
		// If game was started, stop it, not enough player to continue
		else if (_gameState == STARTED) {
			endGame(botData);
			return ;
		}
	}
	// If not anymore player, stop the game
	else if (_playerList.empty()) {
		endGame(botData);
		return ;
	}
	
	// Update Dashboard
	std::vector<INFO_LIST> *elemList = botData.dash->getElemList(botData.dash->getSectionByIndex(2), LEFT);
	if (!elemList)
		return ;

	for (size_t index = 0; index < elemList->size(); ++index) {
		if ((*elemList)[index][0].first == _channel) {
			(*elemList)[index][2].second = convertState(_gameState);
			(*elemList)[index][1].second = toStr(toInt((*elemList)[index][1].second) - 1);
			break ;
		}
	}

	botData.dash->decreaseInfo(botData.dash->getSectionByIndex(2), LEFT, 1); // decrease player amount for GAME
	botData.dash->decreaseInfo(botData.dash->getSectionByIndex(1), LEFT, 2); // decrease player amount for BOT
	botData.dash->render();
}
