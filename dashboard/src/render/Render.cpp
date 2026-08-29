/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:20:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/29 10:55:30 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

// 
// === MAIN METHOD ===
// 
void Dashboard::render( void ) {
	if (_lineWritten > 0)
		eraseScreen();
	
	// Dashboard title
	std::cout << _title << CYAN " Dashboard" RESET << std::endl;
	_lineWritten += 1;

	// Top border of dashboard
	std::cout << "┌";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┐\n";
	_lineWritten += 1;

	// Default display in case of no section
	if (_sectionList.empty()) {
		std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";
		std::cout << "│" BOLD << centerText("No section added", INNER_WIDTH) << RESET "│\n";
		std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";
		_lineWritten += 3;
	}

	// Display with content
	else {
		for (size_t index = 0; index < _sectionList.size(); ++index) {
			t_section section = _sectionList[index];
			
			// Print title and main info
			printSectionHeader(section);

			// Print columns titles
			printColumnsTitles(section);

			// If there is only one column
			if (section.hasLeftColumn && !section.hasRightColumn)
				oneColumnCase(section.leftColumn);
			else if (section.hasRightColumn && !section.hasLeftColumn)
				oneColumnCase(section.rightColumn);
			
			// If there is 2 columns
			else
				printSectionContent(section);
			
			// Print separator
			if (index + 1 < _sectionList.size()) {
				std::cout << "│" << std::string(INNER_WIDTH, '-') << "│\n";
				_lineWritten += 1;
			}
		}
	}

	// Print bottom border
	std::cout << "└";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┘\n";
	_lineWritten += 1;

	// Print log File info
	std::cout << std::endl 
			<< "Logs -> " << _logPath << " (tail -f " << _logPath << ")" << std::endl;
	_lineWritten += 2;
}
