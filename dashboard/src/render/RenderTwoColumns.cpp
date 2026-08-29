/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderTwoColumns.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 18:05:02 by nico              #+#    #+#             */
/*   Updated: 2026/08/29 11:11:29 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

static int printSimpleInfo(t_column &column, size_t *index) {
	std::pair<std::string, std::string> content = column.infoList[*index];
	
	// Print Content
	std::cout << createInfo(content.first, content.second);
	
	// Increase index
	*index += 1;
	if (*index >= column.infoList.size()) {
		*index = 0;
		column.hasSimpleInfo = 0;

		// Check if column still have content
		if (!column.hasElemList)
			column.hasContent = false;

		// Return 1 for new line
		return (1);
	}
	return (0);
}

static int printElemInfo(t_column &column, size_t *eleIdx, size_t *infoIdx, bool *titlePrinted) {
	if (!*titlePrinted) {
		std::string title = column.elemListTitle;
		
		if (title.size() > COL_WIDTH - 1)
			title = title.substr(0, COL_WIDTH - 2).append(".");
		std::cout << title << std::string(COL_WIDTH - 1 - title.size(), ' ');

		*titlePrinted = true;
		return (0);
	}
	
	std::pair<std::string, std::string> content = column.elemList[*eleIdx][*infoIdx];

	// print Content
	std::cout << createInfo(content.first, content.second);

	// Increase index
	*infoIdx += 1;
	if (*infoIdx >= column.elemList[*eleIdx].size()) {
		*infoIdx = 0;
		*eleIdx += 1;

		// Check if we ended the elem list
		if (*eleIdx >= column.elemList.size()) {
			column.hasElemList = false;
			column.hasContent = false;
		}
	
		// Return 1 for the empty line
		return (1);
	}
	return (0);
}

void Dashboard::printSectionContent(t_section &section) {
	t_column	leftColumn = section.leftColumn;
	t_column	rightColumn = section.rightColumn;
	
	// Index to go trough content
	size_t		leftCntIdx = 0;
	size_t		leftEleIdx = 0;
	size_t		rightCntIdx = 0;
	size_t		rightEleIdx = 0;

	// For new lines in element list
	bool		emptyLeftLine = false;
	bool		emptyRightLine = false;

	// For element list titles
	bool		leftTitlePrinted = false;
	bool		rightTitlePrinted = false;

	// Print Content
	while (leftColumn.hasContent || rightColumn.hasContent) {
		// Print left border
		std::cout << "│ ";

		// Print left content
		if (leftColumn.hasContent && !emptyLeftLine) {
			// Simple info case
			if (leftColumn.hasSimpleInfo) {
				if (printSimpleInfo(leftColumn, &leftCntIdx) == 1)
					emptyLeftLine = true;
			}
			// Element list case
			else if (leftColumn.hasElemList) {
				if (printElemInfo(leftColumn, &leftCntIdx, &leftEleIdx, &leftTitlePrinted) == 1)
					emptyLeftLine = true;
			}
		}
		// Empty left line
		else {
			std::cout << std::string(COL_WIDTH - 1, ' ');
			emptyLeftLine = false;
		}

		// Print separator
		std::cout << " | ";

		// Print right content
		if (rightColumn.hasContent && !emptyRightLine) {
			// Simple info case
			if (rightColumn.hasSimpleInfo) {
				if (printSimpleInfo(rightColumn, &rightCntIdx) == 1)
					emptyRightLine = true;
			}
			// Element list case
			else if (rightColumn.hasElemList) {
				if (printElemInfo(rightColumn, &rightCntIdx, &rightEleIdx, &rightTitlePrinted) == 1)
					emptyRightLine = true;
			}
		}
		// Empty right line
		else {
			std::cout << std::string(COL_WIDTH - 1, ' ');
			emptyRightLine = false;
		}

		// Print right border
		std::cout << " │\n";
		_lineWritten += 1;
	}
	
	// Print last line
	std::cout << "│ " << std::string (COL_WIDTH, ' ') << "|" << std::string(COL_WIDTH, ' ') << " │\n";
	_lineWritten += 1;
}
