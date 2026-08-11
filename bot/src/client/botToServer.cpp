/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botToServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 14:01:43 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 10:24:58 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <cstring>

int	registerBot(int sock, std::string password, DashData &data, Dashboard &dash)
{
	send(sock, ("PASS " + password + "\r\n").c_str(), password.size() + 2, 0);
	dash.log(CLIENT, ":RouxBot PASS " + password);
	send(sock, "NICK RouxBot\r\n", 14, 0);
	dash.log(CLIENT, ":RouxBot NICK RouxBot");
	send(sock, "USER rouxbot . . rouxbot\r\n", 26, 0);
	dash.log(CLIENT, ":RouxBot USER rouxbot . . rouxbot");
	
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
	
	data.server.connected = true;
	dash.setServerInfo(data.server);
	dash.render();
	return (0);
}

void serverLoop(int sock, DashData &data, Dashboard &dash)
{
	std::string buf;
	(void)data;
	
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

		}
	}
}
