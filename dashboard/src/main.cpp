/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:18:54 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 08:29:40 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Dashboard.hpp"

int main(void) {
	t_section section;
	section.title = "Seeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeerver";
	// section.mainStatInfo = std::make_pair("moooooooooooooooooooooooooooooooooooooode", "siiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiimulation");
	section.leftColumn.infoList.push_back(std::make_pair("Connection", "OK"));
	section.leftColumn.title = "Suuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuubtitle";
	
	Dashboard dash("ircserv", "ircserv.log");
	dash.addSection(section);

	dash.render();
}
