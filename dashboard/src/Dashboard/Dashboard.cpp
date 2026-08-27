/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dashboard.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 14:07:04 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 17:19:36 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Dashboard.hpp"

// === Constructor & Destructor ===
Dashboard::Dashboard(std::string title, std::string logPath) : _title(title), _logPath(logPath)
{
	// Open _logFile
	_logFile.open(_logPath.c_str(), std::ios::app);
	if (!_logFile.is_open())
		throw std::runtime_error(ERROR "cannot open " + _logPath);

	// Log the begining
	_logFile << std::endl;
	log(SYSTEM, "Starting log for " + title + "\n");
	
	// Close _logFile to avoid fd leaks
	if (_logFile.is_open())
		_logFile.close();
}

Dashboard::~Dashboard() {
	_logFile.open(_logPath.c_str(), std::ios::app);
	if (!_logFile.is_open())
		return ;

	_logFile << std::endl;
	log(SYSTEM, "End of log for that session");
	
	_logFile.open(_logPath.c_str(), std::ios::app);
	if (!_logFile.is_open())
		return ;

	_logFile << std::endl << "------------------------------------------" << std::endl;
	if (_logFile.is_open())
		_logFile.close();
}

// === METHODS ===
int Dashboard::log(const std::string &level, const std::string &message)
{
	if (!_logFile.is_open()) {
		_logFile.open(_logPath.c_str(), std::ios::app);
		if (!_logFile.is_open())
			return (-1);
	}

	_logFile << DIM "[" << timestamp() << "]" RESET BOLD " [" << level << BOLD "] " RESET
			 << message << std::endl;

	_logFile.close();
	return (0);
}