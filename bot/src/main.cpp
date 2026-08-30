/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 14:01:38 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 17:42:29 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <unistd.h>
#include <arpa/inet.h>

/* > Init informations for dashboard */
static void initDash(Dashboard *dash, int port) {
	// SERVER
	t_column serverInfo;
	serverInfo.infoList.push_back(std::make_pair("Connection", "KO"));
	serverInfo.infoList.push_back(std::make_pair("Port", toStr(port)));
	serverInfo.infoList.push_back(std::make_pair("Name", "IRCserv"));

	t_section server;
	server.title = "server";
	server.mainInfo = std::make_pair("MODE", "Real");
	server.leftColumn = serverInfo;

	// BOT
	t_column botInfo;
	botInfo.title = "Global informations";
	botInfo.infoList.push_back(std::make_pair("Channel joined", "0"));
	botInfo.infoList.push_back(std::make_pair("Games", "0"));
	botInfo.infoList.push_back(std::make_pair("Player", "0"));

	t_column taskInfo;
	taskInfo.title = "Last command";
	taskInfo.infoList.push_back(std::make_pair("Command", "NONE"));
	taskInfo.infoList.push_back(std::make_pair("Game", "UNO"));
	taskInfo.infoList.push_back(std::make_pair("Channel", "NONE"));

	t_section bot;
	bot.title = "bot";
	bot.mainInfo = std::make_pair("STATE", "Connecting..");
	bot.leftColumn = botInfo;

	// GAME
	t_column uno;
	uno.title = "UNO";
	uno.infoList.push_back(std::make_pair("Games", "0"));
	uno.infoList.push_back(std::make_pair("Player", "0"));

	t_section game;
	game.title = "game";
	game.leftColumn = uno;

	dash->addSection(server);
	dash->addSection(bot);
	dash->addSection(game);
}

int main(int ac, char **av) {
	if (ac != 4) {
		std::cerr << ERROR
				<< ((ac < 4) ? "not enought parameters" : "too many parameters")
				<< std::endl;
		std::cerr << BOLD "Usage: " RESET "<host> <server_port> <server_password>" << std::endl;
		return (1);
	}

	std::string host = av[1];
	std::string password = av[3];
	char	*end;
	int		port = std::strtol(av[2], &end, 10);

	if (*end) {
		std::cerr << ERROR "Invalid port value" << std::endl;
		return (1);
	} else if (port < 6665 || port > 6669) {
		std::cerr << ERROR "port value not in range (must be in interval [6665 - 6669])" << std::endl;
		return (1); 
	}
	
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);
	
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		std::cerr << ERROR "connection impossible to " << host << std::endl;
		close (sock);
		return (errno);
	}
	
	// Set & display Dashboard
	Dashboard dash(ROUXBOT, "bot.log");
	initDash(&dash, port);
	dash.render();

	t_bot_data botData;
	botData.sock = sock;
	botData.dash = &dash;

	if (registerBot(botData, password) == 0) {
		serverLoop(botData);
	}

	close (sock);
}
