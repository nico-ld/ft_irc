/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleUno.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 10:52:26 by nico              #+#    #+#             */
/*   Updated: 2026/08/15 11:18:25 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

void handleUno(t_bot_data &botData) {
	// Get value from parser
	std::string channel = botData.parser.getParameters()[0];
	std::vector<std::string> param = botData.parser.getGameCmdParam();
	std::string command = param[0];

	Uno *currentGame = static_cast<Uno *>(getCurrentGame(botData, channel));
	
	if (!currentGame || currentGame->getGameState() != STARTED) {
		if (!currentGame)
			sendMessage(botData, channel, "There is no Uno in this channel.", WARNING);
		else
			sendMessage(botData, channel, "The Uno not start yet.", WARNING);
		return ;
	}

	if (command == "turn")
		currentGame->whoseTurn(botData);
}
