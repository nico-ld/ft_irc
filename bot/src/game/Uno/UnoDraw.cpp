/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoDraw.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 09:46:28 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 10:27:05 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

int Uno::drawManagement(t_bot_data &botData, t_player_info &player, e_card card) {
	if (card % 13 == 12) // Draw 2
	{
		if (_lastCard == WILD_DRAW_4) {
			sendMessage(botData, _channel, "Nope, you can't do that ! Theses cards are not stackable", WARNING);
			return (1);
		}
	}
	else if (card == WILD_DRAW_4)
	{
		if (_lastCard % 13 == 12) {// Draw 2
			sendMessage(botData, _channel, "Nope, you can't do that ! Theses cards are not stackable", WARNING);
			return (1);
		}
	}
	else {
		sendMessage(botData, _channel, "Nope, you can't play this card ! Please make a choice between counter the draw with another draw card, or draw", WARNING);
		return (1);
	}
	getPlayerInfo(nextPlayer(false)).drawAmount = player.drawAmount;
	player.drawAmount = 0;
	return (0);
}

void Uno::draw(t_bot_data &botData, t_player_info &player) {
	if (player.drawAmount > 0) {
		sendCard(botData, player.name, player.drawAmount);
		_playerTurn = nextPlayer(false);
		player.drawAmount = 0;
		sendMessage(botData, _channel, player.name + " drew their cards. This is at " + _playerTurn + " to play !", CLIENT);
	}
	else
	{
		std::vector<e_card>::iterator it;

		for (it = player.deck.begin(); it != player.deck.end(); ++it) {
			if (cardPlayable(*it, _lastCard, colorToInt(_currentColor)))
				break ;
		}
		
		// Get new card
		e_card newCard = _pool[std::rand() % _pool.size()];
		
		// If no card in deck is playable but the new one
		if (it == player.deck.end() && cardPlayable(newCard, _lastCard, colorToInt(_currentColor)))
		{
			sendMessage(botData, _channel, player.name + " drew a card that is playable !", CLIENT);
			
			if (newCard < WILD) { // NEED TO ASK PLAYER COLOR
				player.deck.push_back(newCard);
				playCard(botData, player.name, convertIntToString(player.deck.size() - 1), "");
			}
		}
		else
		{
			player.deck.push_back(newCard);
			_playerTurn = nextPlayer(false);
			sendMessage(botData, _channel, player.name + " drew a card, this is now at " + _playerTurn + " to play !", CLIENT);
		}
	}
}
