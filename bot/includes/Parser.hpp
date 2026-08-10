/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:25:42 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 16:27:39 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

class Parser
{
	private:
		// > IRC parser values
		std::string _userName;
		std::string _command;
		std::string _trailing;
		std::vector<std::string> _parameters;

		// > Message parser value
		std::string _gameCmd;
		std::vector<std::string> _gameCmdParam;

	public:
		Parser() {}
		~Parser() {}

		/* > Parse a message received from the IRC server*/
		int parseIRC(std::string &line);
		
		int parseMessage(std::string &line);

		// === IRC GETTERS ===
		std::string getUserName( void ) const { return (_userName); }
		std::string getCommand( void ) const { return (_command); }
		std::string getTrailing( void ) const { return (_trailing); }
		std::vector<std::string> getParameters( void ) const { return (_parameters);}
};

