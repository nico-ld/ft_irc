/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DashboardRender.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 15:20:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 09:54:26 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

// 
// === MAIN METHOD ===
// 
void Dashboard::render( void ) const {
	printDashTitle();

	for (size_t index = 0; index < _sectionList.size(); ++index) {
		t_section section = _sectionList[index];
		printSectionHeader(index);
		
		// Check if there is a second column
		if (section.secondColumn)
			printTwoColumn(index);
		else
			printColumn(index);
			
		if (index + 1 < _sectionList.size())
			printSeparator();
	}

	printDashBottom();
	std::cout << std::endl << "Logs -> " << _logPath << " (tail -f " << _logPath << ")" << std::endl;
}



// 
// === AUXILIARY METHODS ===
// 

// === BORDER ===
void Dashboard::printDashTitle( void ) const {
	// Dashboard title
	std::cout << _title << CYAN " Dashboard" RESET << std::endl;

	// Top border of dashboard
	std::cout << "┌";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┐\n";
}

void Dashboard::printSectionHeader(size_t index) const {
	std::string title = _sectionList[index].title;
	std::string key = _sectionList[index].mainStatInfo.first;
	std::string value = _sectionList[index].mainStatInfo.second;

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
	std::cout << "│";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, " ");
	std::cout << "│\n";
}

void Dashboard::printDashBottom( void ) const {
	// Print last line
	std::cout << "│";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, " ");
	std::cout << "│\n";

	// Print bottom border
	std::cout << "└";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
	std::cout << "┘\n";
}

void Dashboard::printSeparator( void ) const {
	std::cout << "│";
	std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "-");
	std::cout << "│\n";
}


// === SECTION CONTENT ===

static void printTitle(std::string leftTitle, std::string rightTitle) {
	if (leftTitle.empty() && rightTitle.empty())
		return ;

	size_t maxWidth = (rightTitle.empty()) ? INNER_WIDTH - 2 : COL_WIDTH - 1;
	std::cout << "│ ";

	// Left title
	if (!leftTitle.empty()) {
		if (leftTitle.size() > maxWidth)
			std::cout << centerText(leftTitle.substr(0, maxWidth - 1).append("."), maxWidth);
		else
			std::cout << centerText(leftTitle, maxWidth);
	}
	else
		std::cout << std::string(maxWidth, ' ');
	
	// Right title
	if (!rightTitle.empty()) {
		std::cout << " | ";
		if (rightTitle.size() > maxWidth)
			std::cout << centerText(rightTitle.substr(0, maxWidth - 1).append("."), maxWidth);
		else
			std::cout << centerText(rightTitle, maxWidth);
	}

	std::cout << " │\n";
}

void Dashboard::printColumn(size_t index) const {
	t_column column = _sectionList[index].leftColumn;

	// Print column title at the center of line
	printTitle(column.title, "");

	// Print each informations
	std::vector<std::pair<std::string, std::string> >::const_iterator info;
	for (info = column.infoList.begin(); info != column.infoList.end(); ++info) {		
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

static std::string createInfo(std::string key, std::string value) {
	const size_t maxWidth = (COL_WIDTH - 2) / 2;

	// Get size
	size_t leftWidth = (maxWidth < key.size()) ? maxWidth : key.size();
	size_t rightWidth = (maxWidth < value.size()) ? maxWidth : value.size();
	size_t totalWidth = leftWidth + rightWidth + SEP_WIDTH;

	// Trim element
	if (key.size() > maxWidth)
		key = key.substr(0, maxWidth - 1).append(".");
	if (value.size() > maxWidth)
		value = value.substr(0, maxWidth - 1).append(".");

	// Return element
	return (key + " : " + value + std::string(COL_WIDTH - 1 - totalWidth, ' '));
}

void Dashboard::printTwoColumn(size_t index) const {
	t_column leftColumn = _sectionList[index].leftColumn;
	t_column rightColumn = _sectionList[index].rightColumn;

	// Print titles
	printTitle(leftColumn.title, rightColumn.title);

	// print each informations
	size_t longestColumn = std::max(leftColumn.infoList.size(), rightColumn.infoList.size());
	for (size_t i = 0; i < longestColumn; ++i) {
		std::cout << "│ ";
		std::cout << ((i < leftColumn.infoList.size()) ? createInfo(leftColumn.infoList[i].first, leftColumn.infoList[i].second) : std::string(COL_WIDTH - 1, ' '));
		std::cout << " | ";
		std::cout << ((i < rightColumn.infoList.size()) ? createInfo(rightColumn.infoList[i].first, rightColumn.infoList[i].second) : std::string(COL_WIDTH - 1, ' '));
		std::cout << " │\n";
	}
	
	std::cout << "│ " << std::string(COL_WIDTH, ' ') << "|" << std::string(COL_WIDTH, ' ') << " │\n";
}
