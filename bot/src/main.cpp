/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 14:01:38 by nico              #+#    #+#             */
/*   Updated: 2026/08/07 14:36:58 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <unistd.h>
#include <arpa/inet.h>

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
	
	// Set value for dashboard
	DashData data;
	initData(data, host, port);

	// Set & display Dashboard
	Dashboard dash(ROUXBOT, "bot.log");
	dash.setServerInfo(data.server);
	dash.setBotInfo(data.bot);
	dash.setGames(data.games);
	dash.render();

	if (registerBot(sock, password, data, dash) == 0) {
		serverLoop(sock, data, dash);
	}

	close (sock);
}
