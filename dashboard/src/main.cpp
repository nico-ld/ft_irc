/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:18:54 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 09:53:45 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Dashboard.hpp"

int main(void) {
	t_section section;
	section.title = "SERVER";
	section.mainStatInfo = std::make_pair("mode", "simulation");
	section.leftColumn.infoList.push_back(std::make_pair("Connection", "OK"));
	section.leftColumn.title = "Subtitle Left";
	section.rightColumn.infoList.push_back(std::make_pair("State", "UP"));
	section.rightColumn.title = " ";
	

	t_section userPart;
	userPart.title = "USER";
	userPart.leftColumn.infoList.push_back(std::make_pair("Users", "42"));
	
	Dashboard dash("ircserv", "ircserv.log");
	dash.addSection(section);
	dash.addSection(userPart);

	dash.render();
}
