/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 17:37:06 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/20 09:47:31 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

std::string Parser::_prefix = "";
std::string Parser::_command = "";
unsigned int Parser::_commandListId = 0;
std::vector<std::string> Parser::_parameters;
std::vector<std::string> Parser::_trailing;
std::vector<std::string> Parser::_commandsChannel;
std::vector<std::string> Parser::_commandsMessage;
std::vector<std::string> Parser::_commandsUser;

void Parser::initCommandList() {
	_commandsChannel.push_back("join");
	_commandsChannel.push_back("kick");
	_commandsChannel.push_back("invite");
	_commandsChannel.push_back("topic");
	_commandsChannel.push_back("mode");
	_commandsChannel.push_back("part");

	_commandsMessage.push_back("privmsg");
	_commandsMessage.push_back("notice");

	_commandsUser.push_back("user");
	_commandsUser.push_back("nick");
	_commandsUser.push_back("pass");
}

std::ostream &operator<<(std::ostream &out, std::vector<std::string> &content) {
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
		out << "[" << *it << "] ";
	return (out);
}
