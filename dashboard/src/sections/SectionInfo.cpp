/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SectionInfo.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 18:39:38 by nico              #+#    #+#             */
/*   Updated: 2026/08/29 18:49:07 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

void Dashboard::updateInfo(t_section *section, e_side colSide, std::string title, std::string newValue) {
	if (!section) {
		log(ERROR_LVL, "Section unknow : cannot modify information");
		return ;
	}
	
	if (colSide == LEFT) {
		for (size_t index; index < section->leftColumn.infoList.size(); ++index) {
			if (section->leftColumn.infoList[index].first == title) {
				section->leftColumn.infoList[index].second = newValue;
				break ;
			}
		}
	}
	else {
		for (size_t index; index < section->rightColumn.infoList.size(); ++index) {
			if (section->rightColumn.infoList[index].first == title) {
				section->rightColumn.infoList[index].second = newValue;
				break ;
			}
		}
	}

	render();
}

void Dashboard::updateInfo(t_section *section, e_side colSide, size_t index, std::string newValue) {
	if (!section) {
		log(ERROR_LVL, "Section unknow : cannot modify information");
		return ;
	}

	if (index >= section->leftColumn.infoList.size()) {
		log(ERROR_LVL, "Index out of range : connot modify information");
		return ;
	}
	
	if (colSide == LEFT)
		section->leftColumn.infoList[index].second = newValue;
	else
		section->rightColumn.infoList[index].second = newValue;

	render();
}

void Dashboard::increaseInfo(t_section *section, e_side colSide, size_t index) {
	if (!section) {
		log(ERROR_LVL, "Section unknow : cannot modify information");
		return ;
	}

	if (index >= section->leftColumn.infoList.size()) {
		log(ERROR_LVL, "Index out of range : connot modify information");
		return ;
	}
	
	if (colSide == LEFT) {
		int value = toInt(section->leftColumn.infoList[index].second) + 1;
		section->leftColumn.infoList[index].second = toStr(value);
	}
	else {
		int value = toInt(section->rightColumn.infoList[index].second) + 1;
		section->rightColumn.infoList[index].second = toStr(value);
	}

	render();
}

void Dashboard::decreaseInfo(t_section *section, e_side colSide, size_t index) {
	if (!section) {
		log(ERROR_LVL, "Section unknow : cannot modify information");
		return ;
	}

	if (index >= section->leftColumn.infoList.size()) {
		log(ERROR_LVL, "Index out of range : connot modify information");
		return ;
	}
	
	if (colSide == LEFT) {
		int value = toInt(section->leftColumn.infoList[index].second) + 1;
		section->leftColumn.infoList[index].second = toStr(value);
	}
	else {
		int value = toInt(section->rightColumn.infoList[index].second) + 1;
		section->rightColumn.infoList[index].second = toStr(value);
	}

	render();
}
