/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:02:10 by nico              #+#    #+#             */
/*   Updated: 2026/09/01 22:19:16 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Game.hpp"

static void clearEndedGame(t_bot_data &botData) {
	std::time_t currentTime = std::time(NULL);

	// Check every games states
	for (std::vector<Game *>::iterator it = botData.games.begin(); it != botData.games.end(); ) 
	{
		// If a game is ended ended, it is removed from the dashboard after 3 seconds
		if ((*it)->getGameState() == ENDED && currentTime - (*it)->getTimeSinceEnd() > 3) {
			
			// Get the section of dashboard
			t_section *section = botData.dash->getSectionByIndex(2);

			// Search for UNO game (for LEFT or RIGHT in dashboard)
			if ((*it)->getGameType() == UNO) {
				for (size_t index = 0; index < section->leftColumn.elemList.size(); ++index) {
					
					// If game get the same channel name, and the Game is ended
					if (section->leftColumn.elemList[index][0].second == (*it)->getChannel()
						&& section->leftColumn.elemList[index][2].second == convertState(ENDED))
					{
						// Remove game from dashboard
						botData.dash->removeElem(section, LEFT, index);
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
		std::string join = "JOIN " + parameters[1] + "\r\n";
		send(botData.sock, join.c_str(), join.size(), MSG_NOSIGNAL);
		botData.dash->log(CLIENT, join);

		// Update dashboard
		botData.dash->increaseInfo(botData.dash->getSectionByIndex(1), LEFT, 0);
	}
	else if (command == "PRIVMSG") {
		std::string trailing = parser.getTrailing();
		parser.parseMessage(trailing);

		// Get command info
		std::string channel = parser.getParameters()[0];
		std::string commandGrp = parser.getGameCmd();
		if (!parser.getGameCmdParam().empty())
			command = parser.getGameCmdParam()[0];
		botData.parser = parser;
		
		
		if (commandGrp == "!help") {
			handleHelp(botData);
		}
		else if (commandGrp == "!game") {
			handleGame(botData, channel, command);
		}
		else if (commandGrp == "!uno") {
			handleUno(botData, channel, command);
		}
		else if (commandGrp == "!ww" || commandGrp == "!werewolf") {
			sendMessage(botData, channel, "Sorry about that, this game is not handled for the moment. But you can play Uno !", CLIENT);
			return ;
		}
		
		botData.dash->updateInfo(botData.dash->getSectionByIndex(1), LEFT, 0, channel);
		botData.dash->updateInfo(botData.dash->getSectionByIndex(1), LEFT, 2, commandGrp + " " + command);
	}
}
