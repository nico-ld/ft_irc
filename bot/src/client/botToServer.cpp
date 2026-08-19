/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botToServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 14:01:43 by nico              #+#    #+#             */
/*   Updated: 2026/08/19 08:55:10 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <cstring>

int	registerBot(t_bot_data &botData, std::string password)
{
	send(botData.sock, ("PASS " + password + "\r\n").c_str(), password.size() + 2, 0);
	botData.dash->log(CLIENT, "PASS " + password);
	send(botData.sock, "NICK RouxBot\r\n", 14, 0);
	botData.dash->log(CLIENT, "NICK RouxBot");
	send(botData.sock, "USER rouxbot . . rouxbot\r\n", 26, 0);
	botData.dash->log(CLIENT, "USER rouxbot . . rouxbot");
	
	bool registered = false;
	std::string buf;

	while (!registered) {
		char tmp[BUFFER_SIZE];
		memset(tmp, 0, BUFFER_SIZE);
		
		int n = recv(botData.sock, tmp, BUFFER_SIZE - 1, 0);
		if (n <= 0) {
			botData.dash->log(SYSTEM, "Server connection is closed");
			return (1);
		}
		
		buf.append(tmp, n);
		size_t pos;
		while ((pos = buf.find("\r\n")) != std::string::npos) {
			std::string line = buf.substr(0, pos);
			buf.erase(0, pos + 2);

			botData.dash->log(SERVER, line);

			if (line.find(" 001 ") != std::string::npos) {
				registered = true;
				botData.dash->log(SUCCESS, ROUXBOT "is connected to IRCSERV");
			} else if (line.find(" 464 ") != std::string::npos) {
				botData.dash->log(ERROR_LVL, "Invalid password");
				return (1);
			} else if (line.find(" 433 ") != std::string::npos) {
				botData.dash->log(ERROR_LVL, "Nickname 'RouxBot' already used");
				return (1);
			}
		}
	}
	
	botData.data.server.connected = true;
	applyDashData(botData);
	return (0);
}

void serverLoop(t_bot_data &botData)
{
	std::string buf;
	
	while (true) {
		char tmp[BUFFER_SIZE];
		memset(tmp, 0, BUFFER_SIZE);

		int n = recv(botData.sock, tmp, BUFFER_SIZE - 1, 0);
		if (n <= 0) {
			botData.dash->log(SYSTEM, "Server connection is closed");
			return ;
		}

		buf.append(tmp, n);
		size_t pos;
		while ((pos = buf.find("\r\n")) != std::string::npos) {
			std::string line = buf.substr(0, pos);
			buf.erase(0, pos + 2);

			botData.dash->log(SERVER, line);

			catchCommand(line, botData);
		}
	}
}
