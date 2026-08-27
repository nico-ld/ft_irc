/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:18:54 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 17:19:01 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Dashboard.hpp"

int main(void) {
	t_section section;
	section.title = "Server";
	section.mainStatInfo = std::make_pair("mode", "simulation");
	section.leftColumn.infoList.push_back(std::make_pair("Connection", "OK"));
	
	Dashboard dash("ircserv", "ircserv.log");
	dash.addSection(section);

	dash.render();
}
