/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/16 15:35:12 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

// == Constructor & Destructor ==

Uno::Uno(int sock, std::string &channel) {
	_sock = sock;
	_channel = channel;
	_gameType = UNO;
}

Uno::~Uno() {}

void Uno::launchGame(t_bot_data &botData) {
	// Init each player information
	for (std::vector<std::string>::iterator it = _playerList.begin(); it != _playerList.end(); ++it)
	{
		t_player_info newPlayer;
		newPlayer.name = *it;
		newPlayer.unoMode = false;
		_playerInfo.push_back(newPlayer);
	}
	
	// Created cards pool
	_pool = fillPool();

	sendMessage(botData, _channel, "Game start ! you will receive your card by private message...", INFO);

	// Send cards to every player
	for (std::vector<std::string>::iterator it = _playerList.begin(); it != _playerList.end(); ++it) {
		sendCard(botData, *it, 7);
	}	

	// define first card
	int color = (std::rand() % 4);
	int rank = std::rand() % 10;
	_lastCard = static_cast<e_card>(color * 10 + rank);
	try {
		_currentColor = colorToString(color);
	} catch(std::exception &e) {
		(void)e;
		botData.dash->log(ERROR_LVL, "Invalid color index");
	}
	
	// Define first player
	_playerTurn = _playerList[std::rand() % _playerList.size()];
	sendMessage(botData, _channel, "Everyone received their cards ! First person to play is " + _playerTurn + " and the first card is : " + convertCard(_lastCard), CLIENT);
}

void Uno::whoseTurn(t_bot_data &botData) const {
	sendMessage(botData, _channel, "This is " + _playerTurn + " turn !", CLIENT);
}

std::string Uno::nextPlayer(bool skip) const{
	std::vector<std::string>::const_iterator it;

	for (it = _playerList.begin(); it != _playerList.end(); ++it) {
		if (*it == _playerTurn) {
			break ;
		}
	}

	if (_reversed) {
		if (it - 1 == _playerList.begin())
			it = _playerList.end() - 1;
		else 
			--it;
		if (skip)
			--it;
	}
	else {
		if (it + 1 == _playerList.end())
			it = _playerList.begin() + 1;
		else
			++it;
		if (skip)
			++it;
	}
	
	return (*it);
}
