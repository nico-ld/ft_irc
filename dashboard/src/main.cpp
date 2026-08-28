/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:18:54 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 14:14:07 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Dashboard.hpp"
#include <unistd.h>

int main(void) {
	// Section SERVER
	t_section section;
	section.title = "SERVER";
	section.mainStatInfo = std::make_pair("mode", "simulation");
	section.leftColumn.infoList.push_back(std::make_pair("Connection", "OK"));
	section.leftColumn.title = "Subtitle Left";
	section.rightColumn.infoList.push_back(std::make_pair("State", "UP"));
	section.rightColumn.title = " ";
	

	// Section USER
	t_section userPart;
	userPart.title = "USER";
	userPart.leftColumn.infoList.push_back(std::make_pair("Users", "42"));
	
	// Information on User
	std::vector<std::pair<std::string, std::string> > userNico;
	userNico.push_back(std::make_pair("name", "nico"));
	userNico.push_back(std::make_pair("fd", "27"));
	userNico.push_back(std::make_pair("Authenticate", "YES"));
	
	// Information on User
	std::vector<std::pair<std::string, std::string> > userAmy;
	userAmy.push_back(std::make_pair("name", "amy"));
	userAmy.push_back(std::make_pair("fd", "42"));
	userAmy.push_back(std::make_pair("Authenticate", "YES"));

	// Add info to section
	userPart.leftColumn.elemList.push_back(userNico);
	userPart.leftColumn.elemList.push_back(userAmy);
	userPart.leftColumn.elemListTitle = "User informations";

	userPart.rightColumn.elemList.push_back(userNico);
	userPart.rightColumn.elemList.push_back(userAmy);
	userPart.rightColumn.elemListTitle = "Second column";

	// Create Dashboard
	Dashboard dash(IRCSERV, "ircserv.log");
	dash.render();

	sleep(1);
	
	dash.addSection(section);
	dash.log(SUCCESS, "section SERVER added");
	dash.render();
	
	sleep(2);

	dash.addSection(userPart);
	dash.log(SUCCESS, "section USER added");

	dash.render();

	// sleep(2);
	
	// t_section userPart2;
	// userPart2.title = "USER";
	// userPart2.leftColumn.infoList.push_back(std::make_pair("Users", "72"));

	// dash.setSection(userPart.title, userPart2);

	// dash.render();
}