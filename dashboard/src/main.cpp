/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:18:54 by nico              #+#    #+#             */
/*   Updated: 2026/08/29 11:09:13 by nico             ###   ########.fr       */
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

	sleep (2);

	t_column userGlobal;
	userGlobal.title = "Global information";
	userGlobal.infoList.push_back(std::make_pair("Users", "2"));
	userGlobal.infoList.push_back(std::make_pair("Channels", "1"));

	std::vector<std::pair<std::string, std::string> > nicoInfo;
	nicoInfo.push_back(std::make_pair("fd", "27"));
	nicoInfo.push_back(std::make_pair("name", "nico"));
	nicoInfo.push_back(std::make_pair("Chan. joined", "1"));
	
	std::vector<std::pair<std::string, std::string> > amyInfo;
	amyInfo.push_back(std::make_pair("fd", "42"));
	amyInfo.push_back(std::make_pair("name", "amy"));
	amyInfo.push_back(std::make_pair("Chan. joined", "1"));

	t_section user;
	user.title = "user";
	user.rightColumn = userGlobal;
	user.leftColumn.elemListTitle = "Users informations";
	user.leftColumn.elemList.push_back(nicoInfo);
	user.leftColumn.elemList.push_back(amyInfo);

	server.rightColumn.infoList[3] = std::make_pair("Client connected", "2");

	dash.addSection(user);
	dash.setSection(0, server);
	dash.render();
}