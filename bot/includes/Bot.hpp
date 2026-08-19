/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/19 08:55:51 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Dashboard.hpp"
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
	DashData	data;
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
