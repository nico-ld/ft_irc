/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoChallenge.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 16:17:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 15:44:12 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void Uno::uno(t_bot_data &botData, t_player_info &player) {
	if (player.unoMode == true) {
		player.unoMode = false;
		sendMessage(botData, _channel, "UNO ! " + player.name + " get only 1 card !", CLIENT);
		return ;
	}
	else
	{
		std::vector<t_player_info>::iterator it;

		for (it = _playerInfo.begin(); it != _playerInfo.end(); ++it) {
			if (it->unoMode == true) {
				sendCard(botData, *it, 2);
				sendMessage(botData, _channel, "UNO ! " + player.name + " sends 2 cards to " + it->name + ", don't forget to yell 'UNO' next time !", CLIENT);
				it->unoMode = false;
				return ;
			}
		}
		if (player.deck.size() > 1)
			sendMessage(botData, _channel, "There is nobody in 'UNO' situation, not even you", CLIENT);
		else
			sendMessage(botData, _channel, "There is nobody in 'UNO' situation, you already yell UNO", CLIENT);
	}
}

void Uno::challenge(t_bot_data &botData, t_player_info &player) {
	if (_lastCard != WILD_DRAW_4) {
		sendMessage(botData, _channel, "You can challenge only " + convertCard(WILD_DRAW_4), WARNING);
		return ;
	}
	else if (_challenged) {
		sendMessage(botData, _channel, "You already challenged this " + convertCard(WILD_DRAW_4), WARNING);
		return ;
	}

	t_player_info challenged = getPlayerInfo(previousPlayer());

	std::vector<e_card>::iterator it;
	for (it = challenged.deck.begin(); it != challenged.deck.end(); ++it) {
		if (*it / 13 == colorToInt(_lastColor)) {
			sendMessage(botData, _channel, "Well done " + player.name + " ! " + challenged.name + " you've been caught, get " + convertIntToString(player.drawAmount) + " cards !", CLIENT);
			sendCard(botData, getPlayerInfo(previousPlayer()), player.drawAmount);
			player.drawAmount = 0;
			return ;
		}
	}

	sendMessage(botData, _channel, "Ouch.. Sorry " + player.name + ", you're wrong. Get a penalty of 2 cards.", CLIENT);
	sendCard(botData, player, player.drawAmount + 2);
	player.drawAmount = 0;
	_playerTurn = nextPlayer(false);
	sendMessage(botData, _channel, "This is now at " + _playerTurn + " to play", CLIENT);
	return ;
}
