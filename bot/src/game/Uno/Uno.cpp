/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 15:12:13 by nico             ###   ########.fr       */
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
	_reversed = false;

	// Init each player information
	for (std::vector<std::string>::iterator it = _playerList.begin(); it != _playerList.end(); ++it)
	{
		t_player_info newPlayer;
		newPlayer.name = *it;
		newPlayer.unoMode = false;
		newPlayer.wildDrawed = false;
		newPlayer.drawAmount = 0;
		_playerInfo.push_back(newPlayer);
	}
	
	// Created cards pool
	_pool = fillPool();

	sendMessage(botData, _channel, "Game start ! you will receive your card by private message...", INFO);

	// Send cards to every player
	for (std::vector<t_player_info>::iterator it = _playerInfo.begin(); it != _playerInfo.end(); ++it) {
		sendCard(botData, *it, 7);
	}	

	// define first card
	int color = std::rand() % 4;
	int rank = std::rand() % 10;
	_lastCard = static_cast<e_card>(color * 13 + rank);
	try {
		_currentColor = colorToString(color);
		_lastColor = _currentColor;
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


// === GETTERS ===

std::string Uno::nextPlayer(bool skip) const{
	std::vector<std::string>::const_iterator it;

	for (it = _playerList.begin(); it != _playerList.end(); ++it) {
		if (*it == _playerTurn) {
			break ;
		}
	}

	if (_reversed) {
		if (it == _playerList.begin())
			it = _playerList.end() - 1;
		else 
			--it;
		if (skip) {
			if (it == _playerList.begin())
				it = _playerList.end() - 1;
			else
				--it;
		}	}
	else {
		if (it == _playerList.end() || it + 1 == _playerList.end())
			it = _playerList.begin();
		else
			++it;
		if (skip) {
			if (it + 1 == _playerList.end())
				it = _playerList.begin();
			else
				++it;
		}
	}
	
	return (*it);
}

std::string Uno::previousPlayer( void ) const {
	std::vector<std::string>::const_iterator it;

	for (it = _playerList.begin(); it != _playerList.end(); ++it) {
		if (*it == _playerTurn)
			break ;
	}

	if (_reversed) {
		if (it == _playerList.end() || it + 1 == _playerList.end())
			it = _playerList.begin();
		else
			++it;
	}
	else {
		if (it == _playerList.begin())
			it = _playerList.end() - 1;
		else
			--it;
	}

	return (*it);
}

void Uno::showCardsAmount(t_bot_data &botData) const {
	std::vector<t_player_info>::const_iterator it;

	sendMessage(botData, _channel, "Here the number of card of everyone : ", CLIENT);
	std::string message;
	for (it = _playerInfo.begin(); it != _playerInfo.end(); ++it) {
		if (message.size() < 400)
		{
			if (!message.empty())
				message.append(" | ");

			// Convert size into string to print it
			std::ostringstream oss;
			oss << it->name << " : " << it->deck.size();
			message.append(oss.str());
		}
		else {
			sendMessage(botData, _channel, message, CLIENT);
			message.clear();
		}
	}
	if (!message.empty())
		sendMessage(botData, _channel, message, CLIENT);
}

std::string Uno::getPlayerTurn( void ) const {
	return (_playerTurn);
}

t_player_info &Uno::getPlayerInfo(std::string userName) {
	std::vector<t_player_info>::iterator it;

	for (it = _playerInfo.begin(); it != _playerInfo.end(); ++it) {
		if (it->name == userName)
			break ;
	}

	return (*it);
}

void Uno::top(t_bot_data &botData) const {
	sendMessage(botData, _channel, "The last card played is " + convertCard(_lastCard), CLIENT);
}
