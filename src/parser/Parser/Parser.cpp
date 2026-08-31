/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 10:58:49 by nico              #+#    #+#             */
/*   Updated: 2026/08/31 17:42:21 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp" // absolute path to avoid conflict with bot Parser

Parser::Parser() {
	_initCommandsList();
}

Parser::Parser(const Parser &other) { *this = other; }

Parser::~Parser() {}

// == Overload ==
Parser &Parser::operator=(const Parser &other) {
	if (this != &other) {
		_prefix = other._prefix;
		_command = other._command;
		_trailing = other._trailing;
		_parameters = other._parameters;
		_commandListId = other._commandListId;
		_commandsChannel = other._commandsChannel;
		_commandsMessage = other._commandsMessage;
		_commandsUser = other._commandsUser;
	}
	return (*this);
}


// === PARSER ===

int Parser::parse(std::string &line) {
	_rawString = line;

	std::stringstream ss(line);
	std::string word;

	// Safe check for empty input
	if (!(ss >> word) || word.empty())
		return (0);
		
	// Seek for prefix abd stores it if found
	// Then get rid of it so we only keep the command to parse
	if (word[0] == ':') {
		_prefix = word;
		if (!(ss >> word) || word.empty())
			return (1);
	}

	// Parse command to see if it match an existing IRC command
	if (parseCommand(word) == 1)
		return (2);

	// Get parameters, up to 14 according to the RFC 1419 convention
	// If we find a ':', it means it's a trailing parameter,
	// so we save it then break; if we don't, we keep getting the regular params
	for (int i = 0; i < 14; i++) {
		ss >> std::ws; // Consume leading whitespace
		if (ss.eof())
			break ;
		
		// Check for trailing parameter start (':')
		if (ss.peek() == ':') {
			ss.get(); // Consume the ':'
			std::getline(ss, _trailing);
			return (0);
		}

		// Get param
		if (ss >> word)
			_parameters.push_back(word);
		else
			return (3);
	}

	// Get trailing
	if (!ss.eof()) {
		if (ss.peek() == ':')
			ss.get(); // Consume the ':'
		std::getline(ss, _trailing);
	}
	
	return (0);
}

int Parser::parseCommand(std::string word) {
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);

	std::vector<std::string>::iterator it;
	for (it = _commandsChannel.begin(); it != _commandsChannel.end(); it++) {
		if (*it == word) {
			_command = word;
			_commandListId = 1;
			return (0);
		}
	}

	for (it = _commandsMessage.begin(); it != _commandsMessage.end(); it++) {
		if (*it == word) {
			_command = word;
			_commandListId = 2;
			return (0);
		}
	}

	for (it = _commandsUser.begin(); it != _commandsUser.end(); it++) {
		if (*it == word) {
			_command = word;
			_commandListId = 3;
			return (0);
		}
	}

	return (1);
}
