/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGame.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 17:17:07 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 11:12:36 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Uno.hpp"

void handleGame(t_bot_data &botData)
{
	std::string channel = botData.parser.getParameters()[0];
	std::vector<std::string> param = botData.parser.getGameCmdParam();
	std::string command = param[0];

	if (command == "create") {
		if (param.size() == 1)
			botData.dash->log(WARNING, "missing argument for !game create");
		else if (param[1] == "uno") {
			Game *newGame = new Uno(botData.sock, channel);
			newGame->initGame(botData.parser.getUserName(), botData);
			botData.games.push_back(newGame);
		}
		else 
			botData.dash->log(WARNING, "unknow parameter for !game create : " + param[1]);
	}
	else if (command == "join" || command == "leave" || command == "start" || command == "turn") {
		for (std::vector<Game *>::iterator it = botData.games.begin(); it != botData.games.end(); ++it) {
			if (channel == (*it)->getChannel()) {
				if (command == "join")
					(*it)->addPlayer(botData.parser.getUserName(), botData);
				else if (command == "leave")
					(*it)->removePlayer(botData.parser.getUserName(), botData);
				// else if (command == "start")
				// 	(*it)->startGame(botData);
				// else
				// 	(*it)->whoseTurn(botData);
				return ;
			}
		}
		botData.dash->log(WARNING, "the channel " + channel + "doesn't exist");
	}
	else
		botData.dash->log(WARNING, "unknow command for !game : " + command);
}
