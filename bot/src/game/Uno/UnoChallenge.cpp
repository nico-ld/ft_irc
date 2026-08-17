/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoChallenge.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 16:17:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 09:35:44 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void Uno::uno(t_bot_data &botData, std::string userName) {
	t_player_info player = getPlayerInfo(userName);

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
				sendCard(botData, it->name, 2);
				sendMessage(botData, _channel, "UNO ! " + player.name + " sends 2 cards to " + it->name + ", don't forget to yell 'UNO' next time !", CLIENT);
				it->unoMode = false;
				return ;
			}
		}
	}
}

void Uno::challenge(t_bot_data &botData, std::string userName) {
	if (_lastCard != WILD_DRAW_4) {
		sendMessage(botData, _channel, "You can challenge only " + convertCard(WILD_DRAW_4), WARNING);
		return ;
	}

	t_player_info challenged = getPlayerInfo(previousPlayer());
	t_player_info drawTarget = getPlayerInfo(userName);

	std::vector<e_card>::iterator it;
	for (it = challenged.deck.begin(); it != challenged.deck.end(); ++it) {
		if (*it / 13 == colorToInt(_lastColor)) {
			sendMessage(botData, _channel, "Well done ! " + challenged.name + " you've been caught, get every cards !", CLIENT);
			sendCard(botData, challenged.name, drawTarget.drawAmount);
			drawTarget.drawAmount = 0;
			return ;
		}
	}

	sendMessage(botData, _channel, "Ouch.. Sorry " + drawTarget.name + ", you're wrong. Get a penalty of 2 cards.", CLIENT);
	sendCard(botData, drawTarget.name, drawTarget.drawAmount + 2);
	drawTarget.drawAmount = 0;
	_playerTurn = nextPlayer(false);
	sendMessage(botData, _channel, "This is now at " + _playerTurn + " to play", CLIENT);
	return ;
}
