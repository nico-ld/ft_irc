/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderTitles.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 16:41:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 18:11:18 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

void Dashboard::printSectionHeader(t_section &section) {
	std::string title = section.title;
	std::string key = section.mainInfo.first;
	std::string value = section.mainInfo.second;

	// Get size of all elements
	size_t titleWidth = (COL_WIDTH < title.size()) ? COL_WIDTH : title.size();
	size_t keyWidth = ((COL_WIDTH - SEP_WIDTH) / 2 < key.size()) ? (COL_WIDTH - SEP_WIDTH) / 2 : key.size();
	size_t valueWidth = ((COL_WIDTH - SEP_WIDTH) / 2 < value.size()) ? (COL_WIDTH - SEP_WIDTH) / 2 : value.size();
	size_t totalWidth = titleWidth + keyWidth + valueWidth + SEP_WIDTH;

	// Trim elements
	if (title.size() > COL_WIDTH)
		title = title.substr(0, COL_WIDTH - 1).append(".");
	if (key.size() > (COL_WIDTH - SEP_WIDTH) / 2)
		key = key.substr(0, ((COL_WIDTH - SEP_WIDTH) / 2) - 1).append(".");
	if (value.size() > (COL_WIDTH - SEP_WIDTH) / 2)
		value = value.substr(0, ((COL_WIDTH - SEP_WIDTH) / 2) - 1).append(".");

	// Print first line
	std::cout << "│ " << title << std::string((INNER_WIDTH - 2) - totalWidth, ' ');
	if (!key.empty())
		std::cout << key << " : " << value;
	else
		std::cout << "   "; // Replace sep emplacement by spaces
	std::cout << " │\n";
	
	// Print second line
	std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";
	_lineWritten += 2;
}

void Dashboard::printColumnsTitles(t_section &section) {
	t_column leftColumn = section.leftColumn;
	t_column rightColumn = section.rightColumn;

	// If no title
	if (!leftColumn.hasTitle && !rightColumn.hasTitle)
		return ;
		
	// If only one title
	if (leftColumn.hasTitle != rightColumn.hasTitle) {
		std::string title;
		
		if (leftColumn.hasTitle)
			title = leftColumn.title;
		else
			title = rightColumn.title;

		// If only one column
		if (section.hasLeftColumn != section.hasRightColumn) {
			if (title.size() > INNER_WIDTH - 2)
				title = title.substr(0, INNER_WIDTH - 3).append(".");
				
			std::cout << "│ " << centerText(title, INNER_WIDTH - 2) << "│ \n";
		}
		
		// If left title
		else if (leftColumn.hasTitle) {
			if (title.size() > COL_WIDTH - 1)
				title = title.substr(0, COL_WIDTH - 2).append(".");

			std::cout << "│ " << centerText(title, COL_WIDTH - 1) << " | " << std::string(COL_WIDTH, ' ') << "│\n";
		}

		// If right title
		else {
			if (title.size() > COL_WIDTH - 1)
				title = title.substr(0, COL_WIDTH - 2).append(".");

			std::cout << "│ " << std::string(COL_WIDTH - 1, ' ') << " | " << centerText(title, COL_WIDTH - 1) << " │\n";
		}
	}

	// If two title
	else {
		std::string leftTitle = leftColumn.title;
		std::string rightTitle = rightColumn.title;

		// Trim titles if there too large
		if (leftTitle.size() > COL_WIDTH - 1)
			leftTitle = leftTitle.substr(0, COL_WIDTH - 2).append(".");
		if (rightTitle.size() > COL_WIDTH - 1)
			rightTitle = rightTitle.substr(0, COL_WIDTH - 2).append(".");

		std::cout << "│ " << centerText(leftTitle, COL_WIDTH - 1) << " | " << centerText(rightTitle, COL_WIDTH - 1) << " │\n";
	}
	_lineWritten += 1;
}
