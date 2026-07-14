/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 15:54:54 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/14 17:39:44 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>

class Parser
{
	public:
		// == Basic methods ==
		static void	parse(std::string &input);

		// == Getters ==
		static std::string getPrefix( void ) {return (_prefix);}
		static std::string getCommand( void ) {return (_command);}
		static std::vector<std::string> getParameters( void ) {return (_parameters);}
		static std::vector<std::string> getTrailing( void ) {return (_trailing);}

		// == Exception ==
		class InvalidCommandException: public std::exception {
			public: virtual const char *what() const throw() {return ("Error: invalid command.");}
		};
		
	private:
		Parser() {}
		~Parser() {}

		// == Private methods ==
		static void parseCommand(std::string &input);

		// Change type when Amy push commands prototype
		static std::vector<std::string> _commandList;
		static void initCommandList( void );

		static std::string _prefix;
		static std::string _command;
		static std::vector<std::string> _parameters;
		static std::vector<std::string> _trailing;

};
