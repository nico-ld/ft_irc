/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoDeck.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:28:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/16 17:19:37 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void Uno::sendCard(t_bot_data &botData, std::string userName, int amount)
{
	std::vector<e_card> deck;
	
	// Get current user deck
	deck = getDeckByUser(userName);
	
	// Fill deck with new cards
	for (int i = 0; i != amount; ++i)
		deck.push_back(_pool[std::rand() % _pool.size()]);

	// Update deck
	setUserDeck(userName, deck);

	// Display new Deck
	showHand(botData, userName);
}

void Uno::showHand(t_bot_data &botData, std::string userName) {
	sendMessage(botData, userName, "Here your deck : ", CLIENT);
	
	std::vector<e_card> deck = getDeckByUser(userName);
	std::string sendingDeck;
	
	for (size_t index = 0; index < deck.size(); ++index) {
		if (sendingDeck.size() < 400) {
			if (!sendingDeck.empty())
				sendingDeck.append(" | ");
			
			// Convert index into string to print it
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

std::vector<e_card> Uno::getDeckByUser(std::string userName) const {
	std::vector<t_player_info>::const_iterator it;

	for (it = _playerInfo.begin(); it != _playerInfo.end(); ++it)
	{
		if (it->name == userName)
			break ;
	}
	return (it->deck);
}

void Uno::setUserDeck(std::string userName, std::vector<e_card> deck) {
	std::vector<t_player_info>::iterator it;

	for (it = _playerInfo.begin(); it != _playerInfo.end(); ++it) {
		if (it->name == userName) {
			it->deck = deck;
			return ;
		}
	}
}
