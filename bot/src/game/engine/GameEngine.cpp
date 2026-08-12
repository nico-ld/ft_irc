/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameEngine.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:10:53 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 09:55:21 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Bot.hpp"

void Game::initGame(std::string userName, t_bot_data &botData) {
	_playerList.push_back(userName);
	_userHost = userName;
	_gameState = WAITING;
	_timeSinceEnd = -1;

	std::string message = "PRIVMSG " + _channel + " :" + userName + " create a " + convertType(_gameType) +" game !\r\t";
	send(_sock, (message).c_str(), message.size(), 0);
	botData.dash->log(CLIENT, ":RouxBot " + message);
	
	message = "PRIVMSG " + _channel + " :Join the game with '!game join', or start it with '!game start'";
	send(_sock, (message).c_str(), message.size(), 0);
	botData.dash->log(CLIENT, ":RouxBot " + message);

	// Update information in dashboard
	GameChannelInfo info;
	info.name = _channel;
	info.playerAmount = 1;
	info.gameState = "WAITING";

	botData.data.games[_gameType].gamesAmount += 1;
	botData.data.games[_gameType].playerAmount += 1;
	botData.data.games[_gameType].channels.push_back(info);
	botData.data.bot.gamesAmount += 1;
	botData.data.bot.playerAmount += 1;

	applyDashData(botData);
}

void Game::setGameState(e_state state, t_bot_data &botData) {
	_gameState = state;
	
	// Update Dashboard
	size_t index = findGameByChannel(botData.data.games[_gameType].channels, _channel);
	if (index == std::string::npos) {
		botData.dash->log(ERROR_LVL, _channel + " isn't registered in the dashboard");
		return ;
	}
	
	botData.data.games[0].channels[index].gameState = convertState(_gameState);
	applyDashData(botData);
}

void Game::startGame(t_bot_data &botData) {
	// Check if player is in the game
	if (!isPlayerInGame(botData.parser.getUserName(), _playerList)) {
		std::string message = "PRIVMSG " + _channel + " :Sorry " + botData.parser.getUserName() + " you can't start a game in wich you're not.";
		send(_sock, message.c_str(), message.size(), 0);
		botData.dash->log(CLIENT, message);
		return ;
	}

	// Check if game can start
	if (_gameState != READY) {
		if (_gameState == ENDED) {
			std::string message = "PRIVMSG " + _channel + " :Cannot start a game that just end.";
			send(_sock, message.c_str(), message.size(), 0);
			botData.dash->log(CLIENT, message);
		}
		else {
			std::string message = "PRIVMSG " + _channel + " :Not enought player to start the game.";
			send(_sock, message.c_str(), message.size(), 0);
			botData.dash->log(CLIENT, message);
		}
	}

	// Set the game as started
	setGameState(STARTED, botData);

	// Start the Game
	// code //
}

void Game::endGame(t_bot_data &botData) {
	if (!isPlayerInGame(botData.parser.getUserName(), _playerList)) {
		std::string message = "PRIVMSG " + _channel + " :Sorry " + botData.parser.getUserName() + " you can't end a game in wich you're not.";
		send(_sock, message.c_str(), message.size(), 0);
		botData.dash->log(CLIENT, message);
		return ;
	}

	// Check if the Game is already ENDED
	if (_timeSinceEnd != -1)
		return ;

	setGameState(ENDED, botData);
	_timeSinceEnd = std::time(NULL);

	botData.data.bot.gamesAmount -= 1;
	botData.data.bot.playerAmount -= 1;
	applyDashData(botData);
	
	botData.dash->log(INFO, convertType(_gameType) + " just end in " + _channel);
}
