/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GamePlayer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:05:05 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 11:21:56 by nico             ###   ########.fr       */
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
	sendMessage(botData, _channel, playerName + "join the " + convertType(_gameType) + " !", INFO);

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
	if (_playerList.size() == 1) {
		if (_gameState == READY)
			setGameState(WAITING, botData);
		else if (_gameState == STARTED) {
			endGame(botData);
			return ;
		}
	}
	else if (_playerList.empty()) {
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
