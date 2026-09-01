/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 13:53:18 by nico              #+#    #+#             */
/*   Updated: 2026/09/01 22:15:31 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

std::string toStr(int num) {
	std::ostringstream oss;
	oss << num;
	return (oss.str());
}

int toInt(std::string str) {
	std::stringstream ss(str);
	int num;
	ss >> num;
	return (num);
}

std::string timestamp() {
	std::time_t t = std::time(NULL);
	char buf[32];
	std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&t));
	return (std::string(buf));
}

std::string centerText(std::string text, size_t width) {
	if (text.size() >= width)
		return (text);

	size_t total = width - text.size();
	size_t left = total / 2;
	size_t right = total - left;
	return (std::string(left, ' ') + text + std::string(right, ' '));
}

std::string createInfo(std::string key, std::string value) {
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

bool sectionIsNull(t_section *section, Dashboard *dash) {
	if (!section) {
		dash->log(ERROR_LVL, "Section unknow : cannot modify information");
		return (true);
	}
	return (false);
}

bool indexOutOfRange(Dashboard *dash, t_section *section, size_t index, size_t size) {
	if (index >= size) {
		dash->log(ERROR_LVL, "[" + section->title + "]: Index out of range {" + toStr(index) + "}: cannot modify information");
		return (true);
	}
	return (false);
}

