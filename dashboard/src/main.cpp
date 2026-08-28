/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:18:54 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 18:27:55 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Dashboard.hpp"
#include <unistd.h>

int main(void) {
	Dashboard dash(IRCSERV, "ircserv.log");
	dash.render();
	sleep(1);
	
	t_column serverInfo;
	serverInfo.infoList.push_back(std::make_pair("state", "UP"));
	serverInfo.infoList.push_back(std::make_pair("host", "127.0.0.1"));
	serverInfo.infoList.push_back(std::make_pair("name", "IRCserv"));
	serverInfo.infoList.push_back(std::make_pair("Client connected", ""));
	
	t_section server;
	server.title = "server";
	server.mainInfo = std::make_pair("mode", "simulation");
	server.rightColumn = serverInfo;

	dash.addSection(server);
	dash.render();
}