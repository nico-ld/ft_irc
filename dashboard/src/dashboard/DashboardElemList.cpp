/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DashboardElemList.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 14:34:50 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 15:23:04 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

void Dashboard::printElemList(size_t index) const {
	t_column column = _sectionList[index].leftColumn;

	// Print empty line
	if (!column.infoList.empty())
		std::cout << "│" << std::string(INNER_WIDTH, ' ') << "│\n";
		
	// Print title
	std::cout << "│ " << column.elemListTitle << " :" << std::string(INNER_WIDTH - 3 - column.elemListTitle.size(), ' ') << "│\n";

	for (size_t i = 0; i < column.elemList.size(); ++i) {
		std::vector<std::pair<std::string, std::string> >::const_iterator info;
		for (info = column.elemList[i].begin(); info != column.elemList[i].end(); ++info) {
			// Get size of content
			size_t leftWidth = (COL_WIDTH - 1 < info->first.size()) ? COL_WIDTH - 1 : info->first.size();
			size_t rightWidth = (COL_WIDTH - 1 < info->first.size()) ? COL_WIDTH - 1 : info->second.size();
			size_t totalWidth = leftWidth + rightWidth + SEP_WIDTH;

			// Trim content in case of too large value
			std::string leftContent = info->first;
			std::string rightContent = info->second;
			if (info->first.size() > COL_WIDTH - 1)
				leftContent = info->first.substr(0, COL_WIDTH - 2).append(".");
			if (info->second.size() > COL_WIDTH - 1)
				rightContent = info->second.substr(0, COL_WIDTH - 2).append(".");

			// Print content
			std::cout << "│ " << leftContent << " : " << rightContent 
					<< std::string((INNER_WIDTH - 2) - totalWidth, ' ') << " │\n";
		}
	}
}

void Dashboard::printTwoElemList(size_t index) const {
	t_column leftColumn = _sectionList[index].leftColumn;
	t_column rightColumn = _sectionList[index].rightColumn;

	// Print left title
	std::string title;
	if (!leftColumn.elemList.empty()) {
		title = (leftColumn.elemListTitle.size() > COL_WIDTH - 3) ? leftColumn.elemListTitle.substr(0, COL_WIDTH - 4).append(".") : leftColumn.elemListTitle;
		std::cout << "│ " << title << " :" << std::string(COL_WIDTH - 3 - title.size(), ' ') << " | ";
	}

	// Print right title
	if (!rightColumn.elemList.empty()) {
		title = (rightColumn.elemListTitle.size() > COL_WIDTH - 3) ? rightColumn.elemListTitle.substr(0, COL_WIDTH - 4).append(".") : rightColumn.elemListTitle;
		std::cout << title << " :" << std::string(COL_WIDTH - 3 - title.size(), ' ') << " │\n";
	}

	size_t leftListIndex = 0;
	size_t leftElemIndex = 0;
	size_t rightListIndex = 0;
	size_t rightElemIndex = 0;
	bool newLeftLine = false;
	bool newRightLine = false;

	// Print each element
	while (leftListIndex < leftColumn.elemList.size() || rightListIndex < rightColumn.elemList.size()) {
		std::cout << "│ ";

		// Print left
		if (!newLeftLine && leftListIndex < leftColumn.elemList.size()) {
			// Print content;
			std::cout << createInfo(leftColumn.elemList[leftListIndex][leftElemIndex].first, leftColumn.elemList[leftListIndex][leftElemIndex].second);

			// Increase then check if we are at the end of the element
			++leftElemIndex;
			if (leftElemIndex >= leftColumn.elemList[leftListIndex].size()) {
				newLeftLine = true;
				++leftListIndex;
				leftElemIndex = 0;
			}
		}
		else {
			newLeftLine = false;
			std::cout << std::string(COL_WIDTH - 1, ' ');
		}

		// Print separator
		std::cout << " | ";

		// Print right
		if (!newRightLine && rightListIndex < rightColumn.elemList.size()) {
			// Print content
			std::cout << createInfo(rightColumn.elemList[rightListIndex][rightElemIndex].first, rightColumn.elemList[rightListIndex][rightElemIndex].second);
			
			// Increase then check if we are at the end of the element
			++rightElemIndex;
			if (rightElemIndex >= rightColumn.elemList[rightListIndex].size()) {
				newRightLine = true;
				++rightListIndex;
				rightElemIndex = 0;
			}
		}
		else {
			newRightLine = false;
			std::cout << std::string(COL_WIDTH - 1, ' ');
		}
		
		std::cout << " │\n";
	}

	std::cout << "│ " << std::string(COL_WIDTH, ' ') << "|" << std::string(COL_WIDTH, ' ') << " │\n";
}

