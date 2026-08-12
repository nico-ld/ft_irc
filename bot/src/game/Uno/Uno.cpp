/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoGame.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:11 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 09:58:27 by nico             ###   ########.fr       */
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

/*
	// Warn User The Game just start
	std::string message = "PRIVMSG " + _channel + " :The game has started you get your cards in private message ! Good luck !";
	send(_sock, message.c_str(), message.size(), 0);
	botData.dash->log(CLIENT, message);
	botData.dash->log(INFO, "The Uno in " + _channel + " has started");

	// Send card to user by private message
	std::vector<std::string>::iterator it;
	for (it = _playerList.begin(); it != _playerList.end(); ++it) {
		std::string message = "PRIVMSG " + *it + " :This is your cards : not defined";
		send(_sock, message.c_str(), message.size(), 0);
		botData.dash->log(CLIENT, message);
	}

	message = "PRIVMSG " + _channel + " :First person to play is : " + _playerList[std::rand() % _playerList.size()];
	send(_sock, message.c_str(), message.size(), MSG_NOSIGNAL);
	botData.dash->log(CLIENT, message);
*/
