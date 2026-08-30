/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoDraw.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 09:46:28 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 17:29:21 by nico             ###   ########.fr       */
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
		sendCard(botData, player, player.drawAmount);
		_playerTurn = nextPlayer(false);
		sendMessage(botData, _channel, player.name + " drew " + toStr(player.drawAmount) + " cards. This is at " + _playerTurn + " to play !", CLIENT);
		player.drawAmount = 0;
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

		if (player.unoMode)
			player.unoMode = false;
		
		// If no card in deck is playable but the new one
		if (it == player.deck.end() && cardPlayable(newCard, _lastCard, colorToInt(_currentColor)))
		{
			sendMessage(botData, _channel, player.name + " drew a card that is playable !", CLIENT);
			player.deck.push_back(newCard);
			
			if (newCard < WILD) {
				playCard(botData, player, toStr(player.deck.size() - 1), "");
			}
			else {
				player.wildDrawed = true;
				_lastCard = newCard;
				if (newCard == WILD_DRAW_4)
					getPlayerInfo(nextPlayer(false)).drawAmount += 4;
				sendMessage(botData, _channel, player.name + " you just drew a " + convertCard(newCard) + ", please choose a color with '!uno color <color>'", CLIENT);
			}
		}
		else
		{
			player.deck.push_back(newCard);
			showHand(botData, player.name);
			_playerTurn = nextPlayer(false);
			sendMessage(botData, _channel, player.name + " drew a card, this is now at " + _playerTurn + " to play !", CLIENT);
		}
	}
}

void Uno::color(t_bot_data &botData, t_player_info &player, std::string color) {
	if (player.wildDrawed == false) {
		sendMessage(botData, _channel, "You can't use this command, if you want to see the top card use '!uno top'", WARNING);
		return ;
	}
	else if (!isColorValid(color)) {
		sendMessage(botData, _channel, "Color is invalid, please try again. Valid color are : 'yellow', 'red', 'blue' and 'green'", WARNING);
		return ;
	}

	player.wildDrawed = false;
	_playerTurn = nextPlayer(false);
	player.deck.pop_back();
	if (player.deck.size() == 1)
		player.unoMode = true;
	_lastColor = _currentColor;
	_currentColor = color;
	sendMessage(botData, _channel, player.name + " just set game color to " + color + ". This is now at " + _playerTurn + " to play !", CLIENT);
}
