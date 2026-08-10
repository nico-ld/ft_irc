/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 15:19:02 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/ansi.hpp"
#include "Dashboard.hpp"

// === Buffer ===
#define BUFFER_SIZE 512

/* > Create a user for the bot on the server, return 1 on error */
int	registerBot(int sock, std::string password, DashData &data, Dashboard &dash);

/* > Start the loop that read server output */
void serverLoop(int sock, DashData &data, Dashboard &Dashboard);

/* > Read incoming message to find a knowed command */
void catchCommand(int sock, std::string line, DashData &data, Dashboard &dash);
