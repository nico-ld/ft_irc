/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserInit.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 17:37:06 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/14 17:39:08 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

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

