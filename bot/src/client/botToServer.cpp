/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botToServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 14:01:43 by nico              #+#    #+#             */
/*   Updated: 2026/08/07 14:41:46 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <sys/socket.h>
#include <cstring>

int	registerBot(int sock, std::string password, DashData &data, Dashboard &dash)
{
	send(sock, ("PASS " + password + "\r\n").c_str(), password.size() + 2, 0);
	send(sock, "NICK RouxBot\r\n", 14, 0);
	send(sock, "USER rouxbot . . rouxbot\r\n", 26, 0);
	
	bool registered = false;
	std::string buf;

	while (!registered) {
		char tmp[BUFFER_SIZE];
		memset(tmp, 0, BUFFER_SIZE);
		
		int n = recv(sock, tmp, BUFFER_SIZE - 1, 0);
		if (n <= 0) {
			dash.log(SYSTEM, "Server connection is closed");
			return (1);
		}
		
		buf.append(tmp, n);
		size_t pos;
		while ((pos = buf.find("\r\n")) != std::string::npos) {
			std::string line = buf.substr(0, pos);
			buf.erase(0, pos + 2);

			dash.log(SERVER, line);

			if (line.find(" 001 ") != std::string::npos) {
				registered = true;
				dash.log(SUCCESS, ROUXBOT "is connected to IRCSERV");
			} else if (line.find(" 464 ") != std::string::npos) {
				dash.log(ERROR_LVL, "Invalid password");
				return (1);
			} else if (line.find(" 433 ") != std::string::npos) {
				dash.log(ERROR_LVL, "Nickname 'RouxBot' already used");
				return (1);
			}
		}
	}
	
	return (0);
}

void serverLoop(int sock, DashData &data, Dashboard &dash)
{
	std::string buf;
	Bot rouxbot;
	
	while (true) {
		char tmp[BUFFER_SIZE];
		memset(tmp, 0, BUFFER_SIZE);

		int n = recv(sock, tmp, BUFFER_SIZE - 1, 0);
		if (n <= 0) {
			dash.log(SYSTEM, "Server connection is closed");
			return ;
		}

		buf.append(tmp, n);
		size_t pos;
		while ((pos = buf.find("\r\n")) != std::string::npos) {
			std::string line = buf.substr(0, pos);
			buf.erase(0, pos + 2);

			dash.log(SERVER, line);

			Parser::parse(line);
			rouxbot.setCommand(Parser::getCommand());
			rouxbot.setParameters(Parser::getParameters());
		}
	}
}
