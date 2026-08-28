/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DashboardSection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:34:27 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 14:09:42 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

// === METHODS === 
void Dashboard::addSection(t_section &newSection) {
	// Check if section is valid
	if (!isSectionValid(newSection)) {
		log(ERROR_LVL, "Invalid section, aborting addSection");
		return ;
	}
	
	// Check if title is available
	if (getSectionByTitle(newSection.title))
		return ;
	
	_sectionList.push_back(newSection);
}

void Dashboard::removeSection(std::string &title) {
	std::vector<t_section>::iterator it;

	for (it = _sectionList.begin(); it != _sectionList.end(); ++it) {
		if (it->title == title)
			_sectionList.erase(it);
	}
}

void Dashboard::removeSection(size_t index) {
	if (index <= _sectionList.size())
		_sectionList.erase(_sectionList.begin() + index);
}

// === GETTERS ===
std::vector<t_section> Dashboard::getSectionList( void ) const { return (_sectionList); }

t_section *Dashboard::getSectionByTitle(std::string &title) {
	std::vector<t_section>::iterator it;

	for (it = _sectionList.begin(); it != _sectionList.end(); ++it) {
		if (it->title == title)
			return (&(*it)); // Dereference iterator -> get a reference -> take address
	}

	return (NULL);
}

t_section *Dashboard::getSectionByIndex(size_t index) {
	if (index >= _sectionList.size())
		return (NULL);
	else
		return (&(_sectionList[index]));
}

// === SETTERS ===
void Dashboard::setSection(std::string &title, t_section &newSection) {
	if (!isSectionValid(newSection)) {
		log(DEBUG, "Invalid section, aborting setSection for section named " + title);
		return ;
	}

	for (size_t index = 0; index < _sectionList.size(); ++index) {
		if (_sectionList[index].title == title) {
			_sectionList[index] = newSection;
			return ;
		}
	}

	addSection(newSection);
}

void Dashboard::setSection(size_t index, t_section &newSection) {
	if (!isSectionValid(newSection)) {
		log(ERROR_LVL, "Invalid section, aborting setSection for index " + toStr(index));
		return ;
	}
	else if (index >= _sectionList.size())
		addSection(newSection);
	else
		_sectionList[index] = newSection;
}

// === BOOLEAN ===
bool Dashboard::isSectionValid(t_section &section) {
	// Sections must have a title
	if (section.title.empty()){
		log(ERROR_LVL, "Invalid section : missing title");
		return (false);
	}	
	
	// Sections must have somes content
	if (section.leftColumn.infoList.empty() && section.leftColumn.elemList.empty()) {
		if (section.rightColumn.infoList.empty() && section.rightColumn.elemList.empty()) {
			log(ERROR_LVL, "Empty section : need at least 1 content");
		}

		// In case of empty left column, change right column to left
		if (!section.rightColumn.infoList.empty()) {
			section.leftColumn.infoList = section.rightColumn.infoList;
			section.rightColumn.infoList.clear();
		}
		if (!section.rightColumn.elemList.empty()) {
			section.leftColumn.elemList = section.rightColumn.elemList;
			section.rightColumn.elemList.clear();
		}
	}

	// Parse left elemList
	if (!section.leftColumn.elemList.empty()) {
		if (section.leftColumn.elemListTitle.empty()) {
			log(ERROR_LVL, "Invalid elemList: title missing");
			return (false);
		}
		
		size_t defaultSize = section.leftColumn.elemList[0].size();
		for (size_t i = 0; i < section.leftColumn.elemList.size(); ++i) {
			if (section.leftColumn.elemList[i].size() != defaultSize) {
				log(ERROR_LVL, "Invalid elemList : each elements are doesn't get the same size");
				return (false);
			}
		}
	}
		
	// Parse right elemList
	if (!section.rightColumn.elemList.empty()) {
		if (section.rightColumn.elemListTitle.empty()) {
			log(ERROR_LVL, "Invalid elemList : title missing (right column)");
			return (false);
		}
		
		size_t defaultSize = section.rightColumn.elemList[0].size();
		for (size_t i = 0; i < section.rightColumn.elemList.size(); ++i) {
			if (section.rightColumn.elemList[i].size() != defaultSize) {
				log(ERROR_LVL, "Invalid elemList : each elements are doesn't get the same size (right column)");
				return (false);
			}
		}
	}
		
	// If there is content on second column, trigger the flag
	if (!section.rightColumn.infoList.empty() || !section.rightColumn.elemList.empty())
		section.secondColumn = true;
	else
		section.secondColumn = false;

	return (true);
}
