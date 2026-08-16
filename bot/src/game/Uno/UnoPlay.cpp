/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoPlay.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:12:18 by nico              #+#    #+#             */
/*   Updated: 2026/08/16 15:37:27 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

t_player_info Uno::getPlayerInfo(std::string userName) const {
	std::vector<t_player_info>::const_iterator it;

	for (it = _playerInfo.begin(); it != _playerInfo.end(); ++it) {
		if (it->name == userName)
			break ;
	}

	return (*it);
}

void Uno::playCard(t_bot_data &botData, std::string userName, std::string deckIdx, std::string color)
{
	t_player_info player = getPlayerInfo(userName);

	// Check player turn
	if (player.name != _playerTurn) {
		sendMessage(botData, _channel, "This is not your turn to play " + player.name, WARNING);
		return ;
	}

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

	// Play a wild card
	if (card == WILD || card == WILD_DRAW_4)
	{
		std::transform(color.begin(), color.end(), color.begin(), ::tolower);
		if (!isColorValid(color)) {
			sendMessage(botData, _channel, "Color is invalid, please try again. Valid color are : 'yellow', 'red', 'blue' and 'green'", WARNING);
			return ;
		}
		else if (card == WILD_DRAW_4)
			_drawAmount += 4;
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
				_drawAmount += 2;
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
}