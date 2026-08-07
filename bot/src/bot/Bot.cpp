/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:28:20 by nico              #+#    #+#             */
/*   Updated: 2026/08/05 10:06:43 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

// == Construtor & destructor ==
Bot::Bot() {}
Bot::Bot(const Bot &other) { *this = other; }
Bot::~Bot() {}


// === OVERLOADS ===
Bot &Bot::operator=(const Bot &other) {
	if (this != &other) {
		_command = other._command;
		_parameters = other._parameters;
	}
	return (*this);
}


// === GETTERS / SETTERS ===
const std::string &Bot::getCommand(void) const { return (_command); }
void Bot::setCommand(std::string command) { _command = command; }

const std::vector<std::string> &Bot::getParameters(void) const { return (_parameters); }
void Bot::setParameters(std::vector<std::string> parameters) { _parameters = parameters; }