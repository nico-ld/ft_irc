/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 14:25:42 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 15:14:53 by nico             ###   ########.fr       */
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
		std::string _userName;
		std::string _command;
		std::string _trailing;
		std::vector<std::string> _parameters;

	public:
		Parser() {}
		~Parser() {}

		int parse(std::string &line);

		std::string getUserName( void ) const { return (_userName); }
		std::string getCommand( void ) const { return (_command); }
		std::string getTrailing( void ) const { return (_trailing); }
		std::vector<std::string> getParameters( void ) const { return (_parameters);}
};
