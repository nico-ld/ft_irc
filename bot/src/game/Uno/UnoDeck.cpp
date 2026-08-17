/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoDeck.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:28:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 15:44:37 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void Uno::sendCard(t_bot_data &botData, t_player_info &player, int amount)
{	
	// Fill deck with new cards
	for (int i = 0; i != amount; ++i) {
		player.deck.push_back(_pool[std::rand() % _pool.size()]);
	}

	// Display new Deck
	showHand(botData, player.name);
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
