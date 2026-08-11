/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGame.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 17:17:07 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 10:01:15 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Uno.hpp"

void handleGame(t_bot_data &botData)
{
	std::vector<std::string> param = botData.parser.getGameCmdParam();

	if (param[0] == "create") {
		if (param.size() == 1)
			botData.dash->log(WARNING, "missing argument for !game create");
		else if (param[1] == "uno") {
			Game *newGame = new Uno(botData.sock, botData.parser.getParameters()[0]);
			newGame->initGame(botData.parser.getUserName(), botData);
			botData.games.push_back(newGame);
		}
		else 
			botData.dash->log(WARNING, "unknow parameter for !game create : " + param[1]);
	}
}
