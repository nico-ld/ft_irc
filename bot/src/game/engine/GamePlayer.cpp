/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GamePlayer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:05:05 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 09:58:15 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Bot.hpp"

void Game::addPlayer(std::string playerName, t_bot_data &botData) {
	// Check if game accept new player
	if (_gameState != READY && _gameState != WAITING) {
		botData.dash->log(WARNING, "Channel " + _channel + " is in " + convertState(_gameState) + " state. " + playerName + " Cannot join the game");
		return ;
	}

	// Check if player is already in the game
	if (isPlayerInGame(playerName, _playerList)) {
		botData.dash->log(WARNING, playerName + " is already in the game");
		return ;
	}
	
	// Add player in the game
	_playerList.push_back(playerName);
	_gameState = READY;
	
	// Log the new update
	botData.dash->log(INFO, playerName + " join the " + convertType(_gameType) + " in " + _channel);

	// Update Dashboard
	// Find channel in dashboard data
	size_t index = findGameByChannel(botData.data.games[_gameType].channels, _channel);
	if (index == std::string::npos) {
		botData.dash->log(ERROR_LVL, _channel + " isn't registered in the dashboard");
		return ;
	}

	// Update dashboard data
	botData.data.games[_gameType].channels[index].gameState = "READY";
	botData.data.games[_gameType].channels[index].playerAmount += 1;
	botData.data.games[_gameType].playerAmount += 1;
	botData.data.bot.playerAmount += 1;
		
	applyDashData(botData);
}

void Game::removePlayer(std::string playerName, t_bot_data &botData) {
	// Find player in the list
	std::vector<std::string>::iterator it = _playerList.begin();
	for ( ; it != _playerList.end(); ++it) {
		if (*it == playerName)
			break ;
	}
	
	// If player not found, player has not joined the game 
	if (it == _playerList.end()) {
		botData.dash->log(WARNING, playerName + " isn't in the game");
		return ;
	}
	
	// If player found, remove it 
	_playerList.erase(it);
	
	// Log the update
	botData.dash->log(INFO, playerName + " leave the " + convertType(_gameType) + " in " + _channel);

	// If game state have to be updated
	if (_playerList.size() == 1) {
		if (_gameState == READY)
			setGameState(WAITING, botData);
		else if (_gameState == STARTED) {
			endGame(botData);
			return ;
		}
	}
	else if (_playerList.size() <= 0) {
		endGame(botData);
		return ;
	}

	// Update dashboard
	// Find the channel in dashboard data
	size_t index = findGameByChannel(botData.data.games[_gameType].channels, _channel);
	if (index == std::string::npos) {
		botData.dash->log(ERROR_LVL, _channel + " isn't registered in the dashboard");
		return ;
	}	

	// Update dashboard data
	botData.data.games[_gameType].channels[index].gameState = convertState(_gameState);
	botData.data.games[_gameType].channels[index].playerAmount -= 1;
	botData.data.games[_gameType].playerAmount -= 1;
	botData.data.bot.playerAmount -= 1;

	applyDashData(botData);
}
