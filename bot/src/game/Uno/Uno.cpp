/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/15 11:19:02 by nico             ###   ########.fr       */
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
	_pool = fillPool();

	sendMessage(botData, _channel, "Game start ! you will receive your card by private message...", INFO);

	for (std::vector<std::string>::iterator it = _playerList.begin(); it != _playerList.end(); ++it) {
		sendCard(botData, *it, 7);
	}	

	_playerTurn = _playerList[std::rand() % _playerList.size()];
	sendMessage(botData, _channel, "Everyone received their cards ! First person to play is " + _playerTurn, CLIENT);
}

void Uno::sendCard(t_bot_data &botData, std::string userName, int amount)
{
	std::vector<e_card> deck;
	
	// Get current User Deck
	deck = getDeckByUser(userName);
	
	// Fill deck with new cards
	for (int i = 0; i != amount; ++i)
		deck.push_back(_pool[std::rand() % _pool.size()]);

	setUserDeck(userName, deck);

	// Build message to user
	sendMessage(botData, userName, "Here your new deck : ", CLIENT);
	std::string sendingDeck;
	for (size_t index = 0; index < deck.size(); ++index) {
		if (sendingDeck.empty() || sendingDeck.size() < 400) {
			if (!sendingDeck.empty())
				sendingDeck.append(" | ");
			
			std::ostringstream oss;
			oss << index << ". " << convertCard(deck[index]);
			sendingDeck.append(oss.str());
		}
		else {
			sendMessage(botData, userName, sendingDeck, CLIENT);
			sendingDeck.clear();
		}
	}
	if (!sendingDeck.empty())
		sendMessage(botData, userName, sendingDeck, CLIENT);
}

void Uno::whoseTurn(t_bot_data &botData) const {
	sendMessage(botData, _channel, "This is " + _playerTurn + " turn !", CLIENT);
}
