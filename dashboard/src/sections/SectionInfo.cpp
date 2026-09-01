/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SectionInfo.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 18:39:38 by nico              #+#    #+#             */
/*   Updated: 2026/09/01 22:18:56 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

INFO_LIST *Dashboard::getSimpleInfo(t_section *section, e_side colSide) {
	if (sectionIsNull(section, this))
		return (NULL);

	if (colSide == LEFT)
		return (&(section->leftColumn.infoList));
	else
		return (&(section->rightColumn.infoList));
}

void Dashboard::updateInfo(t_section *section, e_side colSide, size_t index, std::string newValue) {
	if (sectionIsNull(section, this))
		return ;

	if (colSide == LEFT && !indexOutOfRange(this, section, index, section->leftColumn.infoList.size())) {
		section->leftColumn.infoList[index].second = newValue;
	}
	else if (colSide == RIGHT && !indexOutOfRange(this, section, index, section->rightColumn.infoList.size())) {
		section->rightColumn.infoList[index].second = newValue;
	}
	else
		return ;

	render();
}

void Dashboard::increaseInfo(t_section *section, e_side colSide, size_t index) {
	if (sectionIsNull(section, this))
		return ;

	if (colSide == LEFT && !indexOutOfRange(this, section, index, section->leftColumn.infoList.size())) {
		int value = toInt(section->leftColumn.infoList[index].second) + 1;
		section->leftColumn.infoList[index].second = toStr(value);
	}
	else if (colSide == RIGHT && !indexOutOfRange(this, section, index, section->rightColumn.infoList.size())) {
		int value = toInt(section->rightColumn.infoList[index].second) + 1;
		section->rightColumn.infoList[index].second = toStr(value);
	}
	else
		return ;

	render();
}

void Dashboard::decreaseInfo(t_section *section, e_side colSide, size_t index) {
	if (sectionIsNull(section, this))
		return ;

		
	if (colSide == LEFT && !indexOutOfRange(this, section, index, section->leftColumn.infoList.size())) {
		int value = toInt(section->leftColumn.infoList[index].second) - 1;
		section->leftColumn.infoList[index].second = toStr(value);
	}
	else if (colSide == RIGHT && !indexOutOfRange(this, section, index, section->rightColumn.infoList.size())) {
		int value = toInt(section->rightColumn.infoList[index].second) - 1;
		section->rightColumn.infoList[index].second = toStr(value);
	}
	else
		return ;

	render();
}

void Dashboard::addSimpleInfo(t_section *section, e_side colSide, std::string key, std::string value) {
	if (sectionIsNull(section, this))
		return ;

	if (key.empty()) {
		log(ERROR_LVL, "Invalid key : cannot add new information, the key is empty");
		return ;
	}
		
	if (colSide == LEFT) {
		section->leftColumn.infoList.push_back(std::make_pair(key, value));
		section->leftColumn.hasSimpleInfo = true;
	}
	else {
		section->rightColumn.infoList.push_back(std::make_pair(key, value));
		section->leftColumn.hasSimpleInfo = true;
	}
	
	render();
}

void Dashboard::removeSimpleInfo(t_section *section, e_side colSide, size_t index) {
	if (sectionIsNull(section, this))
		return ;

	if (colSide == LEFT && !indexOutOfRange(this, section, index, section->leftColumn.infoList.size())) {
		section->leftColumn.infoList.erase(section->leftColumn.infoList.begin() + index); 
		if (section->leftColumn.infoList.empty())
			section->leftColumn.hasSimpleInfo = false;
	}
	else if (colSide == RIGHT && !indexOutOfRange(this, section, index, section->rightColumn.infoList.size())) {
		section->rightColumn.infoList.erase(section->rightColumn.infoList.begin() + index);
		if (section->rightColumn.infoList.empty())
			section->rightColumn.hasSimpleInfo = false;
	}
	else
		return ;

	render();
}
