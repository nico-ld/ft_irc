/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:23:16 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/14 18:57:49 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

void Parser::parseCommand(std::string &input) {
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);

	std::vector<std::string>::iterator it;
	for (it = _commandList.begin(); it != _commandList.end(); it++) {
		if (*it == input) {
			_command = input;
			return ;
		}
	}
}

void Parser::parse(std::string &input) {
	initCommandList();

	_prefix = "";
	_command = "";
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

	for (int i = 0; i < 15; i++) {
		ss >> word;
		if (word[0] == ':')
			break ;
		_parameters.push_back(word);
	}
	while (ss >> word)
		_trailing.push_back(word);
}
