/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:23:16 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/20 10:32:39 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

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
}

void Parser::parse(std::string &input) {
	initCommandList();

	_prefix = "";
	_command = "";
	_commandListId = 0;
	_parameters.clear();
	_trailing.clear();

	std::string word;
	std::stringstream ss(input);
	ss >> word;
	if (word[0] == ':') {
		_prefix = word;
		ss >> word;
	}
	parseCommand(word);
	if (word.empty())
		throw InvalidCommandException();

	for (int i = 0; i < 14; i++) {
		ss >> word;
		if (word[0] == ':') {
			_trailing.push_back(word);
			break ;
		}
		_parameters.push_back(word);
	}
	while (ss >> word)
		_trailing.push_back(word);
}
