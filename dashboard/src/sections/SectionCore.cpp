/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SectionCore.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:34:27 by nico              #+#    #+#             */
/*   Updated: 2026/08/30 18:12:02 by nico             ###   ########.fr       */
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

void Dashboard::updateMainInfo(t_section *section, std::string newValue) {
	if (sectionIsNull(section, this))
		return ;
	
	section->mainInfo.second = newValue;
	render();
}
