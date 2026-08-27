/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DashboardRenderHelper.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:35:13 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 15:58:56 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"



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
	}
	
	// Return number of lines
	return (nbLines);
}
