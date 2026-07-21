/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:23:16 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/21 12:53:04 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void Parser::parse(std::string &input) {
	std::stringstream ss(input);
	std::string word;

	initCommandList();
	
	try {
		ss >> word;
		if (word[0] == ':') {
			_prefix = word;
			ss >> word;
		}

		parseCommand(word);

		for (int i = 0; i < 14; i++) {
			if (!(ss >> word))
				break ;
			if (word[0] == ':') {
				_trailing.push_back(word);
				break ;
			}
			_parameters.push_back(word);
		}

		while (ss >> word) {
			_trailing.push_back(word);
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::flush << std::endl;
	}
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

	throw InvalidCommandException();
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
