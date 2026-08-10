/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:24:08 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 16:28:45 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parser.hpp"

static bool unknowCommand(std::string command)
{
	std::transform(command.begin(), command.end(), command.begin(), ::tolower);

	if (command == "invite" || command == "privmsg")
		return (false);
	else
		return (true);
}

int Parser::parseIRC(std::string &line)
{
	std::stringstream ss(line);
	std::string word;

	// Safe check for empty input
	if (!(ss >> word) || word.empty())
		return (0);
	// Seek for prefix and stores it if found
	// then get rid of it so we only keep the command to parse
	if (word[0] == ':')
	{
		_userName = word.substr(1, word.find('!') - 1);
		if (!(ss >> word) || word.empty())
			return (0);
	}

	if (unknowCommand(word))
		return (1);
	_command = word;
	
	// Get parameters, up to 14 according to the RFC 1459 convention
	// if we find a :, it means it's a trailing parameter,
	// so we save it then break ; if we don't, we keep getting the regular params
	for (int i = 0; i < 14; i++) {
		ss >> std::ws; // Consume leading whitespace
		if (ss.eof())
			break;

		// Check for trailing parameter start (':')
		if (ss.peek() == ':') {
			ss.get(); // Consume the ':'
			std::getline(ss, _trailing);
			break ;
		}

		if (ss >> word)
			_parameters.push_back(word);
		else
			break;
	}
	return (0);
}

int Parser::parseMessage(std::string &line) {
	if (line.empty())
		return (1);
	
	std::stringstream ss(line);
	std::string word;

	if (!(ss >> word) || word[0] != '!')
		return (1);

	if (word == "!help" || word == "!game" || word == "!uno")
		_gameCmd = word;
	else
		return (1);
		
	while (ss >> word) {
		_gameCmdParam.push_back(word);
	}
	return (0);
}
