/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 11:53:34 by nico             ###   ########.fr       */
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
	sendMessage(botData, _channel, "Game has started, you get your cards in private message ! Good luck !", INFO);
	
	// Send cards

	std::string firstPlayer = _playerList[std::rand() % _playerList.size()];
	sendMessage(botData, _channel, "First person to play is " + firstPlayer, CLIENT);
}
