/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 17:37:06 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/16 09:22:08 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

std::string Parser::_prefix = "";
std::string Parser::_command = "";
std::vector<std::string> Parser::_parameters;
std::vector<std::string> Parser::_trailing;
std::vector<std::string> Parser::_commandList;

void Parser::initCommandList() {
	_commandList.push_back("join");
	_commandList.push_back("kick");
	_commandList.push_back("invite");
	_commandList.push_back("topic");
	_commandList.push_back("mode");
	_commandList.push_back("part");
	_commandList.push_back("privmsg");
	_commandList.push_back("notice");
}

std::ostream &operator<<(std::ostream &out, std::vector<std::string> &content) {
	for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); it++)
		out << "[" << *it << "] ";
	return (out);
}
