/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoGame.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 11:45:18 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

Uno::Uno(int sock, std::string &channel) {
	_sock = sock;
	_channel = channel;
}

Uno::~Uno() {}

// === METHODS ===
void Uno::initGame(std::string userName, t_bot_data &botData) {
	_playerList.push_back(userName);
	_userHost = userName;
	_gameState = WAITING;
	_gameType = UNO;

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

int Uno::addPlayer(std::string playerName, t_bot_data &botData) {
	if (_gameState != READY && _gameState != WAITING) {
		botData.dash->log(WARNING, "Channel " + _channel + " is in " + convertState(_gameState) + " state. " + playerName + " Cannot join the game");
		return (3);
	}

	std::vector<std::string>::iterator it = _playerList.begin();
	for ( ; it != _playerList.end(); ++it) {
		if (*it == playerName) {
			botData.dash->log(WARNING, playerName + " is already in the game");
			return (1);
		}
	}
	_playerList.push_back(playerName);
	_gameState = READY;

	// Update Dashboard
	size_t index = 0;
	std::vector<GameChannelInfo> tmp = botData.data.games[0].channels;
	for ( ; index < tmp.size(); ++index) {
		if (tmp[index].name == _channel)
			break ;
	}
	if (index == tmp.size()) {
		botData.dash->log(WARNING, _channel + " isn't registered in the dashboard");
		return (2);
	}
	
	botData.dash->log(INFO, playerName + " join the Uno in " + _channel);

	botData.data.games[0].channels[index].gameState = "READY";
	botData.data.games[0].channels[index].playerAmount += 1;
	botData.data.games[0].playerAmount += 1;
	botData.data.bot.playerAmount += 1;
		
	botData.dash->setGames(botData.data.games);
	botData.dash->setBotInfo(botData.data.bot);
	botData.dash->render();
	return (0);
}

int Uno::removePlayer(std::string playerName, t_bot_data &botData) {
	std::vector<std::string>::iterator it = _playerList.begin();
	for ( ; it != _playerList.end(); ++it) {
		if (*it == playerName)
			break ;
	}
	if (it == _playerList.end()) {
		botData.dash->log(WARNING, playerName + " isn't in the game");
		return (1);
	}
	_playerList.erase(it);

	if (_playerList.size() == 1) {
		if (_gameState == READY)
			_gameState = WAITING;
		else if (_gameState == STARTED)
			_gameState = ENDED;
	}
	else if (_playerList.size() <= 0) {
		_gameState = ENDED;
	}

	// Update dashboard
	size_t index = 0;
	std::vector<GameChannelInfo> tmp = botData.data.games[0].channels;
	for ( ; index < tmp.size(); ++index) {
		if (tmp[index].name == _channel)
			break ;
	}
	if (index == tmp.size()) {
		botData.dash->log(WARNING, _channel + " isn't registered in the dashboard");
		return (2);
	}

	botData.dash->log(INFO, playerName + " leave the Uno in " + _channel);

	botData.data.games[0].channels[index].gameState = convertState(_gameState);
	botData.data.games[0].channels[index].playerAmount -= 1;
	botData.data.games[0].playerAmount -= 1;
	botData.data.bot.playerAmount -= 1;
	botData.dash->setGames(botData.data.games);
	botData.dash->setBotInfo(botData.data.bot);
	botData.dash->render();
	return (0);
}

int Uno::setGameState(e_state state, t_bot_data &botData) {
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
		return (1);
	}
	
	botData.data.games[0].channels[index].gameState = convertState(_gameState);
	botData.dash->setGames(botData.data.games);
	botData.dash->render();
	return (0);
}

void Uno::startGame(t_bot_data &botData) {
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
