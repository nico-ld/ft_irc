/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 10:35:03 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include <sys/socket.h>
#include <algorithm>
#include <stdexcept>

/*
Username start a Uno game !
	- to join it : !game join
	- to start it : !game start
	- to end it : !game end
*/

Uno::Uno() {}
Uno::~Uno() {}

// === METHODS ===
void Uno::initGame(std::string &userName, DashData &data, Dashboard &dash) {
	_playerList.push_back(userName);
	_userHost = userName;
	_gameState = WAITING;

	send(_sock, ("PRIVMSG :" + userName + " start a Uno game !\r\t").c_str(), userName.size() + 32, 0);
	dash.log(CLIENT, ":RouxBot PRIVMSG :" + userName + " start a Uno game !");
	
	send(_sock, ("PRIVMSG :Join the game with '!game join', or start it with '!game start"), 72, 0);
	dash.log(CLIENT, "PRIVMSG :Join the game with '!game join', or start it with '!game start");

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
	std::vector<GameChannelInfo>::iterator itChan;
	std::vector<GameChannelInfo> tmp = data.games[0].channels;
	for (itChan = tmp.begin() ; itChan != tmp.end(); ++itChan) {
		if (itChan->name == _channel)
			break ;
	}
	if (itChan == tmp.end())
		return (2);
	
	itChan->gameState = "READY";
	itChan->playerAmount += 1;
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

	std::vector<GameChannelInfo>::iterator itChan;
	std::vector<GameChannelInfo> tmp = data.games[0].channels;
	for (itChan = tmp.begin(); itChan != tmp.end(); ++it) {
		if (itChan->name == _channel)
			break ;
	}
	if (itChan == tmp.end())
		return (2);

	itChan->gameState = convertState(_gameState);
	dash.setGames(data.games);
	dash.render();
	return (0);
}

int Uno::setGameState(e_state state, DashData &data, Dashboard &dash) {
	_gameState = state;
	
	// Update Dashboard
	std::vector<GameChannelInfo>::iterator itChan = data.games[0].channels.begin();
	for ( ; itChan != data.games[0].channels.end(); ++itChan) {
		if (itChan->name == _channel)
			break ;
	}
	if (itChan == data.games[0].channels.end())
		return (1);
	
	itChan->gameState = convertState(_gameState);
	dash.setGames(data.games);
	dash.render();
	return (0);
}
