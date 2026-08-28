/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:20:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 18:01:49 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

// 
// === MAIN METHOD ===
// 
void Dashboard::render( void ) const {
	eraseScreen();
	
	// Dashboard title
	std::cout << _title << CYAN " Dashboard" RESET << std::endl;

	// Top border of dashboard
	std::cout << "┌";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┐\n";

	// Default display in case of no section
	if (_sectionList.empty()) {
		std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";
		std::cout << "│" << centerText("No section added", INNER_WIDTH) << "│\n";
	}

	// Display with content
	else {
		for (size_t index = 0; index < _sectionList.size(); ++index) {
			t_section section = _sectionList[index];
			
			// Print title and main info
			printSectionHeader(section);

			// Print columns titles
			printColumnsTitles(section);

			// Print section informations line by line
			// If there is only one column
			if (section.hasLeftColumn && !section.hasRightColumn) {
				oneColumnCase(section.leftColumn);
				return ;
			}
			else if (section.hasRightColumn && !section.hasLeftColumn) {
				oneColumnCase(section.rightColumn);
				return ;
			}
			
			
			// Print separator
			if (index + 1 < _sectionList.size())
				std::cout << "│" << std::string(INNER_WIDTH, '-') << "│\n";
		}
	}

	// Print dashboard bottom
	std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";

	// Print bottom border
	std::cout << "└";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┘\n";
	
	// Print log File info
	std::cout << std::endl << "Logs -> " << _logPath << " (tail -f " << _logPath << ")" << std::endl;
}
