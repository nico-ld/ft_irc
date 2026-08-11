/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoGame.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 22:37:48 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

// == Constructor & Destructor ==

Uno::Uno(int sock, std::string &channel) {
	_sock = sock;
	_channel = channel;
}

Uno::~Uno() {}


// === Game Management ===

void Uno::initGame(std::string userName, t_bot_data &botData) {
	_playerList.push_back(userName);
	_userHost = userName;
	_gameState = WAITING;
	_timeSinceEnd = -1;

	std::string message = "PRIVMSG " + _channel + " :" + userName + " create a Uno game !\r\t";
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

	botData.data.games[0].gamesAmount += 1;
	botData.data.games[0].playerAmount += 1;
	botData.data.games[0].channels.push_back(info);
	botData.data.bot.gamesAmount += 1;
	botData.data.bot.playerAmount += 1;

	botData.dash->setGames(botData.data.games);
	botData.dash->setBotInfo(botData.data.bot);
	botData.dash->render();
}

void Uno::setGameState(e_state state, t_bot_data &botData) {
	_gameState = state;
	
	// Update Dashboard
	size_t index = 0;
	std::vector<GameChannelInfo> tmp = botData.data.games[0].channels;
	for ( ; index < tmp.size(); ++index) {
		if (tmp[index].name == _channel)
			break ;
	}
	if (index == tmp.size()) {
		botData.dash->log(WARNING, _channel + " isn't registered in the dashboard");
		return ;
	}
	
	botData.data.games[0].channels[index].gameState = convertState(_gameState);
	botData.dash->setGames(botData.data.games);
	botData.dash->render();
}

void Uno::startGame(t_bot_data &botData) {
	if (!isPlayerInGame(botData.parser.getUserName(), _playerList)) {
		std::string message = "PRIVMSG " + _channel + " :Sorry " + botData.parser.getUserName() + " you can't start a game in wich you're not.";
		send(botData.sock, message.c_str(), message.size(), 0);
		botData.dash->log(CLIENT, message);
		return ;
	}

	if (_gameState != READY) {
		if (_gameState == ENDED) {
			std::string message = "PRIVMSG " + _channel + " :Cannot start a game that just end.";
			send(botData.sock, message.c_str(), message.size(), 0);
			botData.dash->log(CLIENT, message);
		}
		else {
			std::string message = "PRIVMSG " + _channel + " :Not enought player to start the game.";
			send(botData.sock, message.c_str(), message.size(), 0);
			botData.dash->log(CLIENT, message);
		}
	}

	setGameState(STARTED, botData);

	std::vector<std::string>::iterator it;
	for (it = _playerList.begin(); it != _playerList.end(); ++it) {
		std::string message = "PRIVMSG " + *it + " :This is your cards : not defined";
		send(botData.sock, message.c_str(), message.size(), 0);
		botData.dash->log(CLIENT, message);
	}

	std::string message = "PRIVMSG " + _channel + " :The game has started you get your cards in private message ! Good luck !";
	send(botData.sock, message.c_str(), message.size(), 0);
	botData.dash->log(CLIENT, message);

	botData.dash->log(INFO, "The Uno in " + _channel + " has started");
}

void Uno::endGame(t_bot_data &botData) {
	if (!isPlayerInGame(botData.parser.getUserName(), _playerList)) {
		std::string message = "PRIVMSG " + _channel + " :Sorry " + botData.parser.getUserName() + " you can't end a game in wich you're not.";
		send(botData.sock, message.c_str(), message.size(), 0);
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
	botData.dash->setBotInfo(botData.data.bot);
	botData.dash->render();
	botData.dash->log(INFO, "Uno just end in " + _channel);
}


// === Player Management ===

void Uno::addPlayer(std::string playerName, t_bot_data &botData) {
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
	botData.dash->log(INFO, playerName + " join the Uno in " + _channel);

	// Update Dashboard
	// Find channel in dashboard data
	size_t index = findGameByChannel(botData.data.games[0].channels, _channel);
	if (index == std::string::npos) {
		botData.dash->log(ERROR_LVL, _channel + " isn't registered in the dashboard");
		return ;
	}

	// Update dashboard data
	botData.data.games[0].channels[index].gameState = "READY";
	botData.data.games[0].channels[index].playerAmount += 1;
	botData.data.games[0].playerAmount += 1;
	botData.data.bot.playerAmount += 1;
		
	applyDashData(botData);
}

void Uno::removePlayer(std::string playerName, t_bot_data &botData) {
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
	botData.dash->log(INFO, playerName + " leave the Uno in " + _channel);

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
	size_t index = findGameByChannel(botData.data.games[0].channels, _channel);
	if (index == std::string::npos) {
		botData.dash->log(ERROR_LVL, _channel + " isn't registered in the dashboard");
		return ;
	}	

	// Update dashboard data
	botData.data.games[0].channels[index].gameState = convertState(_gameState);
	botData.data.games[0].channels[index].playerAmount -= 1;
	botData.data.games[0].playerAmount -= 1;
	botData.data.bot.playerAmount -= 1;

	applyDashData(botData);
}

