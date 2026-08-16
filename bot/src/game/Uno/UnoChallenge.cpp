/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoChallenge.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 16:17:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/16 17:14:00 by nico             ###   ########.fr       */
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
