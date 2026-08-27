/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 17:37:06 by nile-dai          #+#    #+#             */
/*   Updated: 2026/08/27 10:01:40 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/Parser.hpp"  // absolute path to avoid conflict with bot Parser

void Parser::_initCommandsList( void ) {
	// Init values to avoid conditional jump on uninitialized values
	_prefix.clear();
	_command.clear();
	_trailing.clear();
	_parameters.clear();

	// Channel management command
	_commandsChannel.push_back("join");
	_commandsChannel.push_back("kick");
	_commandsChannel.push_back("invite");
	_commandsChannel.push_back("topic");
	_commandsChannel.push_back("mode");
	_commandsChannel.push_back("part");
	_commandsChannel.push_back("quit");

	// Messaging command
	_commandsMessage.push_back("privmsg");
	_commandsMessage.push_back("notice");

	// User and connection commands
	_commandsUser.push_back("user");
	_commandsUser.push_back("nick");
	_commandsUser.push_back("pass");
}

std::ostream &operator<<(std::ostream &out, std::vector<std::string> &content) {
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
		out << "[" << *it << "] ";
	return (out);
}
