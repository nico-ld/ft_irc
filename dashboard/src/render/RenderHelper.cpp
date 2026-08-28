/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderHelper.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:35:13 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 16:44:33 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

int Dashboard::getElemLines(t_section section) const {
	t_column leftColumn = section.leftColumn;
	t_column rightColumn = section.rightColumn;

	// Get the number of element in the list => number of new line (+ 1)
	int leftLines = leftColumn.elemList.size();
	int rightLines = rightColumn.elemList.size();

	// Get number of line with content writtent (minus the +1 line in the previous step)
	if (leftLines != 0)
		leftLines += (leftColumn.elemList.size() * leftColumn.elemList[0].size()) - 1;
	if (rightLines != 0)
		rightLines += (rightColumn.elemList.size() * rightColumn.elemList[0].size()) - 1;
	
	// Return the biggest number
	return (std::max(leftLines, rightLines));
}

int Dashboard::getNbLines( void ) const {
	int nbLines = 4; // Defaults lines

	if (_sectionList.empty()) {
		return (6); // Default dashboard without any section
	}
	
	// Add Section separator
	nbLines += _sectionList.size() - 1;

	// For each sections
	std::vector<t_section>::const_iterator it;
	for (it = _sectionList.begin(); it != _sectionList.end(); ++it) {
		// Add section title and second line
		nbLines += 2;
		
		// Check for a column title
		if (!it->leftColumn.title.empty() || !it->rightColumn.title.empty())
			nbLines += 1;
		
		// Get number of lines on each columns
		int leftLines = it->leftColumn.infoList.size();
		int rightLines = it->rightColumn.infoList.size();

		// Add the max line
		nbLines += std::max(leftLines, rightLines);

		// Add the number of lines in the elemList
		nbLines += getElemLines(*it);
	}
	
	// Return number of lines
	return (nbLines);
}

