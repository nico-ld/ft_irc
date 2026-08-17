/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoPlay.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:12:18 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 09:59:06 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void Uno::playCard(t_bot_data &botData, std::string userName, std::string deckIdx, std::string color)
{
	t_player_info player = getPlayerInfo(userName);

	// Check card index
	char *end;
	size_t index = strtol(deckIdx.c_str(), &end, 10);
	if (*end) {
		sendMessage(botData, _channel, "Invalid char at the end of index : '" + deckIdx + "', please try again", WARNING);
		return ;
	}
	else if (index >= player.deck.size()) {
		sendMessage(botData, _channel, "Index out of range, please try again. You can see your hand with '!uno hand'", WARNING);
		return ;
	}

	// Play the card
	e_card	card = player.deck[index];
	bool	skip = false;

	if (player.drawAmount > 0) {
		if (drawManagement(botData, player, card) == 1)
			return ;
	}

	// Play a wild card
	if (card == WILD || card == WILD_DRAW_4)
	{
		std::transform(color.begin(), color.end(), color.begin(), ::tolower);
		if (!isColorValid(color)) {
			sendMessage(botData, _channel, "Color is invalid, please try again. Valid color are : 'yellow', 'red', 'blue' and 'green'", WARNING);
			return ;
		}
		else if (color == _currentColor) {
			sendMessage(botData, _channel, "This is the current color, that make no sense !", WARNING);
			return ;
		}
		else if (card == WILD_DRAW_4)
			getPlayerInfo(nextPlayer(skip)).drawAmount += 4; // Add draw for the next player 
		_lastColor = _currentColor;
		_currentColor = color;
	}
	
	// Play an action card 
	else 
	{
		if (!cardPlayable(card, _lastCard, colorToInt(color))) {
			sendMessage(botData, _channel, "You can't play this card, please try another card, or take a new one", WARNING);
			return ;
		}

		int cardRank = card % 13;
		if (cardRank >= 10) {
			if (cardRank == 10)
				skip = true;
			else if (cardRank == 11)
				_reversed = !_reversed;
			else
				getPlayerInfo(nextPlayer(skip)).drawAmount += 2; // Add draw for the next player
		}
	}

	// Update info for next round
	_lastCard = card;
	player.deck.erase(player.deck.begin() + index);
	_playerTurn = nextPlayer(skip);

	if (player.deck.size() == 1)
		player.unoMode = true;
	// else if (player.deck.size() == 0)
	// 	endGame();

	std::string cardInfo = ". "; // no info
	if (card >= WILD)
		cardInfo = ", new color is " + color + " ! ";
	
	sendMessage(botData, _channel, player.name + " played " + convertCard(card) + cardInfo + _playerTurn + " this is your turn !", CLIENT);

	if (getPlayerInfo(_playerTurn).drawAmount > 0)
		sendMessage(botData, _channel, _playerTurn + " You have to draw, if you get a card to counter it play it '!uno play <cardIdx>' else draw with '!uno draw'", CLIENT);
}