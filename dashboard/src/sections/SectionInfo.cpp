/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SectionInfo.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 18:39:38 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 12:22:41 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

static bool sectionIsNull(t_section *section, Dashboard *dash) {
	if (!section) {
		dash->log(ERROR_LVL, "Section unknow : cannot modify information");
		return (true);
	}
	return (false);
}

static bool indexOutOfRange(t_section *section, Dashboard *dash, size_t index, size_t size) {
	if (index >= size) {
		dash->log(ERROR_LVL, "Index out of range : cannot modify information");
		return ;
	}
}

void Dashboard::updateInfo(t_section *section, e_side colSide, size_t index, std::string newValue) {
	if (sectionIsNull(section, this))
		return ;

	if (colSide == LEFT && !indexOutOfRange(section, this, index, section->leftColumn.infoList.size())) {
		section->leftColumn.infoList[index].second = newValue;
	}
	else if (colSide == RIGHT && indexOutOfRange(section, this, index, section->rightColumn.infoList.size())) {
		section->rightColumn.infoList[index].second = newValue;
	}
	else
		return ;

	render();
}

void Dashboard::increaseInfo(t_section *section, e_side colSide, size_t index) {
	if (sectionIsNull(section, this))
		return ;

	if (colSide == LEFT && !indexOutOfRange(section, this, index, section->leftColumn.infoList.size())) {
		int value = toInt(section->leftColumn.infoList[index].second) + 1;
		section->leftColumn.infoList[index].second = toStr(value);
	}
	else if (colSide == RIGHT && !indexOutOfRange(section, this, index, section->rightColumn.infoList.size())) {
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

		
	if (colSide == LEFT && !indexOutOfRange(section, this, index, section->leftColumn.infoList.size())) {
		int value = toInt(section->leftColumn.infoList[index].second) + 1;
		section->leftColumn.infoList[index].second = toStr(value);
	}
	else if (colSide == RIGHT && !indexOutOfRange(section, this, index, section->rightColumn.infoList.size())) {
		int value = toInt(section->rightColumn.infoList[index].second) + 1;
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
		
	if (colSide == LEFT)
		section->leftColumn.infoList.push_back(std::make_pair(key, value));
	else
		section->rightColumn.infoList.push_back(std::make_pair(key, value));

	render();
}

void Dashboard::removeSimpleInfo(t_section *section, e_side colSide, size_t index) {
	if (sectionIsNull(section, this))
		return ;

	if (colSide == LEFT && !indexOutOfRange(section, this, index, section->leftColumn.infoList.size())) {
		section->leftColumn.infoList.erase(section->leftColumn.infoList.begin() + index);
	}
	else if (colSide == RIGHT && !indexOutOfRange(section, this, index, section->rightColumn.infoList.size())) {
		section->rightColumn.infoList.erase(section->rightColumn.infoList.begin() + index);
	}
	else
		return ;

	render();
}

void Dashboard::addElem(t_section *section, e_side colSide,
						std::vector<std::pair<std::string, std::string> > newElem)
{
	if (sectionIsNull(section, this))
		return ;
	
	if (colSide == LEFT) {
		if (!section->leftColumn.elemList.empty() && section->leftColumn.elemList[0].size() != newElem.size()) {
			log(ERROR_LVL, "Invalid element : new element has a different size than first element");
			return ;
		}
		else if (section->leftColumn.elemList.empty() && section->leftColumn.elemListTitle.empty())
			section->leftColumn.elemListTitle = "Info by element";
		section->leftColumn.elemList.push_back(newElem);
	}
	else {
		if (!section->rightColumn.elemList.empty() && section->rightColumn.elemList[0].size() != newElem.size()) {
			log(ERROR_LVL, "Invalid element : new element has a different size than first element");
			return ;
		}
		else if (section->rightColumn.elemList.empty() && section->rightColumn.elemListTitle.empty())
			section->rightColumn.elemListTitle = "Info by element";
		section->rightColumn.elemList.push_back(newElem);
	}

	render();
}

void Dashboard::removeElem(t_section *section, e_side colSide, size_t index) {
	if (sectionIsNull(section, this))
		return ;
	
	if (colSide == LEFT && !indexOutOfRange(section, this, index, section->leftColumn.elemList.size())) {
		section->leftColumn.elemList.erase(section->leftColumn.elemList.begin() + index);
	}
	else if (colSide == RIGHT && !indexOutOfRange(section, this, index, section->rightColumn.elemList.size())) {
		section->rightColumn.elemList.erase(section->rightColumn.elemList.begin() + index);
	}
	else
		return ;

	render ();
}
