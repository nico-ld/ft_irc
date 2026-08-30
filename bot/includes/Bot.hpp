/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 11:12:46 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../dashboard/includes/Dashboard.hpp"
#include "./Parser.hpp"
#include <sys/socket.h>

struct	DashData;
class	Dashboard;
class	Game;

// === Buffer ===
#define BUFFER_SIZE 512

struct t_bot_data
{
	int			sock;
	Dashboard	*dash;
	Parser		parser;
	std::vector<Game *> games;
};

/* > Create a user for the bot on the server, return 1 on error */
int	registerBot(t_bot_data &botData, std::string password);

/* > Start the loop that read server output */
void serverLoop(t_bot_data &botData);

/* > Read incoming message to find a knowed command */
void catchCommand(std::string line, t_bot_data &botData);

/* > Send a message to the server */
void sendMessage(t_bot_data &botData, std::string target, std::string content, std::string level);

/* > Create a new vector to add a game in the dashboard */
std::vector<std::pair<std::string, std::string> > 
addGame(std::string name, std::string playerAmount, std::string gameState);

