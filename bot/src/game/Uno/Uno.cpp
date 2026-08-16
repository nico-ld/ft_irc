/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/16 13:41:52 by nico             ###   ########.fr       */
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

	// Define first player
	_playerTurn = _playerList[std::rand() % _playerList.size()];
	sendMessage(botData, _channel, "Everyone received their cards ! First person to play is " + _playerTurn, CLIENT);
}

void Uno::whoseTurn(t_bot_data &botData) const {
	sendMessage(botData, _channel, "This is " + _playerTurn + " turn !", CLIENT);
}
