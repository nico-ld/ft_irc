/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderColumn.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 16:42:46 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 18:02:19 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

// === ONE COLUMN CASE ===
void Dashboard::oneColumnCase(t_column &column) const {
	// Simple informations
	if (column.hasSimpleInfo) {
		std::vector<std::pair<std::string, std::string> >::const_iterator info;
		
		for (info = column.infoList.begin(); info != column.infoList.end(); ++info) {
			// Get values
			std::string key = info->first;
			std::string value = info->second;
			
			// Trim values
			if (key.size() > COL_WIDTH - 1)
				key = key.substr(0, COL_WIDTH - 2).append(".");
			if (value.size() > COL_WIDTH - 1)
				value = value.substr(0, COL_WIDTH - 2).append(".");

			// Compute remaining space
			size_t spacesRemaining = INNER_WIDTH - 2 - (key.size() + SEP_WIDTH + value.size());

			// Print line
			std::cout << "│ " << key << " : " << value << std::string(spacesRemaining, ' ') << " │\n";
		}
		
		std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";
	}

	// List of elements
	if (column.hasElemList) {
		// Print title
		std::string title = column.elemListTitle;
		std::cout << "│ " 
				<< centerText(((title.size() < INNER_WIDTH - 2) ? title.substr(0, INNER_WIDTH - 3).append(".") : title), INNER_WIDTH - 2)
				<< " │\n";
		
		// Print each elements
		for (size_t index = 0; index < column.elemList.size(); ++index) {
			std::vector<std::pair<std::string, std::string> > element;
			std::vector<std::pair<std::string, std::string> >::const_iterator info;

			for (info = element.begin(); info != element.end(); ++info) {
				// Get values
				std::string key = info->first;
				std::string value = info->second;

				// Trim values
				if (key.size() > COL_WIDTH - 1)
					key = key.substr(0, COL_WIDTH - 2).append(".");
				if (value.size() > COL_WIDTH - 1)
					value = value.substr(0, COL_WIDTH - 2).append(".");
				
				// Compute remaining space
				size_t spaceRemaining = INNER_WIDTH - 2 - (key.size() + SEP_WIDTH + value.size());

				// Print line
				std::cout << "│ " << key << " : " << value << std::string(spaceRemaining, ' ') << " │\n";
			}
			
			// Print an empty line after each element
			std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";
		}
	}
}
