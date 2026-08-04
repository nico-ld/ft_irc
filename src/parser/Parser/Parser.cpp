/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:23:16 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/29 15:09:10 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int Parser::parse(std::string &input) {
	std::stringstream ss(input);
	std::string word;

	initCommandList();

	// Wipes out old data in buffer, so last msg doesn't bleed out
	// on the new message
	clearParser();
	
	try {
		// Safe check for empty input
		if (!(ss >> word) || word.empty())
			return (0);
		// Seek for prefix and stores it if found
		// then get rid of it so we only keep the command to parse
		if (word[0] == ':') {
			_prefix = word;
			if (!(ss >> word) || word.empty())
				return (0);
		}

		// Parse command to see if it match an existing IRC command
		parseCommand(word);

		// Get parameters, up to 14 according to the RFC 1459 convention
		// if we find a :, it means it's a trailing parameter,
		// so we save it then break ; if we don't, we keep getting the regular params
		for (int i = 0; i < 14; i++) {
			ss >> std::ws; // Consume leading whitespace
			if (ss.peek())
				break;

			// Check for trailing parameter start (':')
			if (ss.peek() == ':') {
				std::string trailing_str;
				ss.get(); // Consume the ':'
				std::getline(ss, trailing_str);
				_trailing.push_back(trailing_str);
				break;
			}

			if (ss >> word) {
				_parameters.push_back(word);
			} else {
				break;
			}
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::flush << std::endl;
		return (-1);
	}
	return (0);
}
void Parser::parseCommand(std::string &input) {
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);

	std::vector<std::string>::iterator it;
	for (it = _commandsChannel.begin(); it != _commandsChannel.end(); it++) {
		if (*it == input) {
			_command = input;
			_commandListId = 1;
			return ;
		}
	}

	for (it = _commandsMessage.begin(); it != _commandsMessage.end(); it++) {
		if (*it == input) {
			_command = input;
			_commandListId = 2;
			return ;
		}
	}

	for (it = _commandsUser.begin(); it != _commandsUser.end(); it++) {
		if (*it == input) {
			_command = input;
			_commandListId = 3;
			return ;
		}
	}

	throw std::runtime_error("Unknow command");
}

void Parser::clearParser(void) {
	_prefix.clear();
	_command.clear();
	_commandListId = 0;
	_parameters.clear();
	_trailing.clear();
}

void Parser::buildPrefix(User &user) {
	if (!_prefix.empty())
		return ;

	if (user.isAuthenticated())
		_prefix = user.getNickname() + "!" + user.getRealname() + "@" + user.getHostname();
}
