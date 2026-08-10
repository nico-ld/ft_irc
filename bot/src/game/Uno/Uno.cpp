/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 11:41:38 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include <sys/socket.h>
#include <algorithm>
#include <stdexcept>

Uno::Uno(int sock, std::string &channel) {
	_sock = sock;
	_channel = channel;
}

Uno::~Uno() {}

// === METHODS ===
void Uno::initGame(std::string &userName, DashData &data, Dashboard &dash) {
	_playerList.push_back(userName);
	_userHost = userName;
	_gameState = WAITING;

	send(_sock, ("PRIVMSG :" + userName + " start a Uno game !\r\t").c_str(), userName.size() + 32, 0);
	dash.log(CLIENT, ":RouxBot PRIVMSG :" + userName + " start a Uno game !");
	
	send(_sock, ("PRIVMSG :Join the game with '!game join', or start it with '!game start'"), 72, 0);
	dash.log(CLIENT, ":RouxBot PRIVMSG :Join the game with '!game join', or start it with '!game start'");

	// Update information in dashboard
	GameChannelInfo info;
	info.name = _channel;
	info.playerAmount = 1;
	info.gameState = "WAITING";

	data.games[0].gamesAmount += 1;
	data.games[0].playerAmount += 1;
	data.games[0].channels.push_back(info);

	dash.setGames(data.games);
	dash.render();
}

int Uno::addPlayer(std::string playerName, DashData &data, Dashboard &dash) {	
	std::vector<std::string>::iterator it = _playerList.begin();
	for ( ; it != _playerList.end(); ++it) {
		if (*it == playerName)
			return (1);
	}
	_playerList.push_back(playerName);
	_gameState = READY;

	// Update Dashboard
	size_t index = 0;
	std::vector<GameChannelInfo> tmp = data.games[0].channels;
	for ( ; index < tmp.size(); ++index) {
		if (tmp[index].name == _channel)
			break ;
	}
	if (index == tmp.size())
		return (2);
	
	data.games[0].channels[index].gameState = "READY";
	data.games[0].channels[index].playerAmount += 1;
	data.games[0].playerAmount += 1;
		
	dash.setGames(data.games);
	dash.render();
	return (0);
}

int Uno::removePlayer(std::string playerName, DashData &data, Dashboard &dash) {
	std::vector<std::string>::iterator it = _playerList.begin();
	for ( ; it != _playerList.end(); ++it) {
		if (*it == playerName)
			break ;
	}
	if (it == _playerList.end())
		return (1);
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
	std::vector<GameChannelInfo> tmp = data.games[0].channels;
	for ( ; index < tmp.size(); ++index) {
		if (tmp[index].name == _channel)
			break ;
	}
	if (index == tmp.size())
		return (2);

	data.games[0].channels[index].gameState = convertState(_gameState);
	data.games[0].channels[index].playerAmount -= 1;
	data.games[0].playerAmount -= 1;
	dash.setGames(data.games);
	dash.render();
	return (0);
}

int Uno::setGameState(e_state state, DashData &data, Dashboard &dash) {
	_gameState = state;
	
	// Update Dashboard
	size_t index = 0;
	std::vector<GameChannelInfo> tmp = data.games[0].channels;
	for ( ; index < tmp.size(); ++index) {
		if (tmp[index].name == _channel)
			break ;
	}
	if (index == tmp.size())
		return (1);
	
	data.games[0].channels[index].gameState = convertState(_gameState);
	dash.setGames(data.games);
	dash.render();
	return (0);
}
