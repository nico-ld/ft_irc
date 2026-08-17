/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:02:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 16:36:26 by nico             ###   ########.fr       */
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
		
		botData.data.bot.currentTask.channel = channel;
		botData.data.bot.currentTask.command = commandGrp + " " + command;
		botData.data.bot.currentTask.game = "Uno";
		applyDashData(botData);
	}
}
