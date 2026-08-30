/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SectionElem.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 14:59:18 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 17:49:11 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

std::vector<INFO_LIST> *Dashboard::getElemList(t_section *section, e_side colSide) {
	if (sectionIsNull(section, this))
		return (NULL);

	if (colSide == LEFT)
		return (&(section->leftColumn.elemList));
	else
		return (&(section->rightColumn.elemList));
}

INFO_LIST *Dashboard::getElem(t_section *section, e_side colSide, size_t index) {
	if (sectionIsNull(section, this))
		return (NULL);

	if (colSide == LEFT) {
		if (indexOutOfRange(this, index, section->leftColumn.elemList.size()))
			return (NULL);
		return (&(section->leftColumn.elemList[index]));
	}
	else {
		if (indexOutOfRange(this, index, section->rightColumn.elemList.size()))
			return (NULL);
		return (&(section->rightColumn.elemList[index]));
	}
}

void Dashboard::addElem(t_section *section, e_side colSide, INFO_LIST newElem) {
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
	
	if (colSide == LEFT && !indexOutOfRange(this, index, section->leftColumn.elemList.size())) {
		section->leftColumn.elemList.erase(section->leftColumn.elemList.begin() + index);
	}
	else if (colSide == RIGHT && !indexOutOfRange(this, index, section->rightColumn.elemList.size())) {
		section->rightColumn.elemList.erase(section->rightColumn.elemList.begin() + index);
	}
	else
		return ;

	render ();
}

void Dashboard::setElem(t_section *section, e_side colSide, size_t index, INFO_LIST newElem) {
	if (sectionIsNull(section, this))
		return ;

	if (colSide == LEFT) {
		if (indexOutOfRange(this, index, section->leftColumn.elemList.size()))
			return ;
		section->leftColumn.elemList[index] = newElem;
	}
	else {
		if (indexOutOfRange(this, index, section->rightColumn.elemList.size()))
			return ;
		section->rightColumn.elemList[index] = newElem;
	}

	render();
}
