/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DashboardRender.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:20:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 16:51:30 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

// 
// === MAIN METHOD ===
// 
void Dashboard::render( void ) const {
	printDashTitle();

	for (size_t index = 0; index < _sectionList.size(); ++index) {
		printSectionHeader(index);
	}

	printDashBottom();
}



// 
// === AUXILIARY METHODS ===
// 

void Dashboard::printDashTitle( void ) const {
	// Dashboard title
	std::cout << _title << CYAN " Dashboard" RESET << std::endl;

	// Top border of dashboard
	std::cout << "┌";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┐\n";
}

void Dashboard::printSectionHeader(size_t index) const {
	t_section section = _sectionList[index];
	std::string key = section.mainStatInfo.first;
	std::string value = section.mainStatInfo.second;
	
	// Compute size needed on line, std::min is used to define a size max
	size_t used = std::min((int)section.title.size(), COL_WIDTH - 2);
	if (!key.empty())
		used += std::min((int)key.size(), 10) + std::min((int)value.size(), 10) + 3;
	
	// Compute space remaining
	size_t space = (INNER_WIDTH - 2 > used) ? (INNER_WIDTH - 2 - used) : 1;
	
	// Print first line
	std::cout << "│ ";
	if (section.title.size() > COL_WIDTH - 2)
		std::cout << section.title.substr(0, COL_WIDTH - 2) << ".";
	else
		std::cout << section.title;
	std::cout << std::string(space, ' ');
	
	// Print main stat info
	if (!key.empty()) {
		if (key.size() > 10)
			std::cout << key.substr(0, 10) << ".";
		else
			std::cout << key;
		std::cout << " : ";
		if (value.size() > 10)
			std::cout << value.substr(0, 10) << ".";
		else
			std::cout << value;
	}
	std::cout << " │\n";

	// Print second line
	std::cout << "│ ";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), COL_WIDTH, " ");
	std::cout << "|";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), COL_WIDTH, " ");
	std::cout << " │\n";
}

void Dashboard::printDashBottom( void ) const {
	std::cout << "└";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┘\n";
}
