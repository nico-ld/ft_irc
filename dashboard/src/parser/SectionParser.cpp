/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SectionParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 15:45:41 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 16:32:58 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

bool Dashboard::isSectionValid(t_section &section) {
	// Parse section
	if (!sectionInfoValid(section))
		return (false);
	
	// Parse left column
	if (!columnInfoValid(section.leftColumn, section.title))
		return (false);

	// Parse right column
	if (!columnInfoValid(section.rightColumn, section.title))
		return (false);

	// Check if section get somes content
	if (!section.leftColumn.hasContent && !section.rightColumn.hasContent) {
		log(ERROR_LVL, "Invalid section [" + section.title + "] : Section doesn't get any content");
		return (false);
	}

	// Trigger column flag
	section.hasLeftColumn = section.leftColumn.hasContent;
	section.hasRightColumn = section.rightColumn.hasContent;

	return (true);
}

bool Dashboard::sectionInfoValid(t_section &section) {
	// Check title
	if (section.title.empty()) {
		log(ERROR_LVL, "Invalid section: Missing title");
		return (false);
	}
	
	// Check if only one of two is empty
	if (section.mainInfo.first.empty() != section.mainInfo.second.empty()) {
		log(ERROR_LVL, "Invalid section [" + section.title + "] : There is an empty string in main info");
		return (false);
	}
	// If first string isn't empty the second too
	else if (!section.mainInfo.first.empty())
		section.hasMainInfo = true;

	return (true);
}

bool Dashboard::columnInfoValid(t_column &column, std::string sectionTitle) {
	// Check if there is a title
	column.hasTitle = !column.title.empty();

	// Check if there is simple informations
	column.hasSimpleInfo = !column.title.empty();

	// Check if there is list of elements
	column.hasElemList = !column.elemList.empty();

	// Trigger content flag
	column.hasContent = (column.hasSimpleInfo || column.hasElemList);

	// Check if every simple informations get a key
	if (column.hasSimpleInfo) {
		for (size_t index = 0; index < column.infoList.size(); ++index) {
			if (column.infoList[index].first.empty()) {
				log(ERROR_LVL, "Invalid element [" + sectionTitle + "] : Missing key " + toStr(index) + " in simple informations");
				return (false);
			}
		}
	}

	// Check if element list get element of same size everywhere
	if (column.hasElemList) {
		// Take first size as reference
		size_t refSize = column.elemList[0].size();
		
		for (size_t index = 0; index < column.elemList.size(); ++index) {
			if (column.elemList[index].size() != refSize) {
				log(ERROR_LVL, "Invalid element [" + sectionTitle + "] : The element " + toStr(index) + " get a different size from the first one");
				return (false);

				// Check prescence of keys
				for (size_t elemIndex = 0; elemIndex < column.elemList[index].size(); ++elemIndex) {
					if (column.elemList[index][elemIndex].first.empty()) {
						log(ERROR_LVL, "Invalid element [" + sectionTitle + "] : Missing key " + toStr(elemIndex) + " in element " + toStr(index));
						return (false);
					}
				}
			}
		}
	}

	return (true);
}
