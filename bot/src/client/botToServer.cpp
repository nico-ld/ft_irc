/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botToServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 14:01:43 by nico              #+#    #+#             */
/*   Updated: 2026/09/01 09:19:20 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <cstring>
#include <signal.h>

volatile sig_atomic_t g_stop = 0;

void handleSignal(int sig) {
	g_stop = sig;
}

void setupSignalHandler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSignal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}

int	registerBot(t_bot_data &botData, std::string password)
{
	// Send password
	std::string message = "PASS " + password + "\r\n";
	send(botData.sock, message.c_str(), message.size(), MSG_NOSIGNAL);
	botData.dash->log(CLIENT, message);

	// Send Nickname
	message = "NICK RouxBot\r\n";
	send(botData.sock, message.c_str(), message.size(), MSG_NOSIGNAL);
	botData.dash->log(CLIENT, message);

	// Send User
	message = "USER rouxbot . . rouxbot\r\n";
	send(botData.sock, message.c_str(), message.size(), MSG_NOSIGNAL);
	botData.dash->log(CLIENT, message);
	
	bool registered = false;
	std::string buf;

	// Handle signal;
	setupSignalHandler();

	while (!registered) {
		char tmp[BUFFER_SIZE];
		memset(tmp, 0, BUFFER_SIZE);
		
		int n = recv(botData.sock, tmp, BUFFER_SIZE - 1, 0);
		if (n <= 0 || g_stop == SIGINT) {
			if (g_stop == SIGINT) {
				botData.dash->log(SYSTEM, "Shutdown signal received, stopping " ROUXBOT);
				
				std::string message = "QUIT :RouxBot goes to infinity and beyond\r\n";
				send(botData.sock, message.c_str(), message.size(), MSG_NOSIGNAL);
				botData.dash->log(CLIENT, message);
			}
			else
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
				botData.dash->log(SUCCESS, ROUXBOT " is connected to IRCSERV");
			} else if (line.find(" 464 ") != std::string::npos) {
				botData.dash->log(ERROR_LVL, "Invalid password");
				return (1);
			} else if (line.find(" 433 ") != std::string::npos) {
				botData.dash->log(ERROR_LVL, "Nickname 'RouxBot' already used");
				return (1);
			}
		}
	}
	
	botData.dash->updateInfo(botData.dash->getSectionByIndex(0), LEFT, 0, "OK");
	botData.dash->updateMainInfo(botData.dash->getSectionByIndex(1), "Up");
	return (0);
}

void serverLoop(t_bot_data &botData)
{
	std::string buf;
	
	while (true) {
		char tmp[BUFFER_SIZE];
		memset(tmp, 0, BUFFER_SIZE);

		// Read bytes
		int n = recv(botData.sock, tmp, BUFFER_SIZE - 1, 0);
		
		// Handle evenements 
		if (n <= 0 || g_stop == SIGINT) {
			if (g_stop == SIGINT) {
				botData.dash->log(SYSTEM, "Shutdown signal received, stopping " ROUXBOT);
				
				std::string message = "QUIT :RouxBot goes to infinity and beyond\r\n";
				send(botData.sock, message.c_str(), message.size(), MSG_NOSIGNAL);
				botData.dash->log(CLIENT, message);
				return ;
			}
			else
				botData.dash->log(SYSTEM, "Server connection is closed");
			return ;
		}

		// Read line
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
