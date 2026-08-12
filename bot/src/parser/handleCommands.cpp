/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:02:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 10:20:37 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Game.hpp"

static void clearEndedGame(t_bot_data &botData) {
	std::time_t currentTime = std::time(NULL);

	for (std::vector<Game *>::iterator it = botData.games.begin(); it != botData.games.end(); ) 
	{
		if ((*it)->getGameState() == ENDED && currentTime - (*it)->getTimeSinceEnd() > 3) {
			// Remove game from dashboard
			for (size_t gameIndex = 0; gameIndex < botData.games.size(); ++gameIndex) {
				
				std::vector<GameChannelInfo> &channel = botData.data.games[gameIndex].channels;
				std::vector<GameChannelInfo>::iterator itDash;
				
				for (itDash = channel.begin(); itDash != channel.end(); ++itDash) {
					if ((*it)->getChannel() == itDash->name) {
						channel.erase(itDash);
						break ;
					}
				}
			}
			
			// Remove game from bot data
			delete *it;
			it = botData.games.erase(it);
		}
		else
			++it;
	}

	applyDashData(botData);
}

void catchCommand(std::string line, t_bot_data &botData)
{
	Parser parser;
	
	parser.parseIRC(line);
	
	std::string command = parser.getCommand();
	std::vector<std::string> parameters = parser.getParameters();

	if (!botData.games.empty())
		clearEndedGame(botData);
	if (command == "INVITE") {
		std::string join = "JOIN " + parameters[0] + "\r\n";
		send(botData.sock, join.c_str(), join.size(), MSG_NOSIGNAL);
		botData.dash->log(CLIENT, join);
	}
	else if (command == "PRIVMSG") {
		std::string trailing = parser.getTrailing();
		parser.parseMessage(trailing);

		command = parser.getGameCmd();
		botData.parser = parser;
		
		if (command == "!help") {
			handleHelp(botData.sock, botData);
		}
		else if (command == "!game") {
			handleGame(botData);
		}
		else if (command == "!uno") {
			botData.dash->log(DEBUG, "command 'uno' is not handled for the moment.");
		}
	}
}
