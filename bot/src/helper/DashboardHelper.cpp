/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DashboardHelper.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:14:20 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 11:11:35 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Game.hpp"

std::vector<std::pair<std::string, std::string> > 
addGame(std::string name, std::string playerAmount, std::string gameState) {
	std::vector<std::pair<std::string, std::string> > newGame;

	newGame.push_back(std::make_pair("Channel", name));
	newGame.push_back(std::make_pair("Player", playerAmount));
	newGame.push_back(std::make_pair("Game state", gameState));

	return (newGame);
}

void sendMessage(t_bot_data &botData, std::string target, std::string content, std::string level) {
	if (content.size() + target.size() > 450) {
		botData.dash->log(ERROR_LVL, "Message too long");
		return ;
	}

	std::string message = "PRIVMSG " + target + " :" + content + "\r\n";
	send(botData.sock, message.c_str(), message.size(), MSG_NOSIGNAL);
	botData.dash->log(level, message);
}
