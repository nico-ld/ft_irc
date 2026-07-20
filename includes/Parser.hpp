/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 15:54:54 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/20 09:56:55 by nile-dai         ###   ########.fr       */
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
		static unsigned int getCommandListId( void ) {return (_commandListId);}
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
		static std::vector<std::string> _commandsChannel;
		static std::vector<std::string> _commandsMessage;
		static std::vector<std::string> _commandsUser;
		static void initCommandList( void );

		static std::string _prefix;
		static std::string _command;
		static unsigned int _commandListId;
		static std::vector<std::string> _parameters;
		static std::vector<std::string> _trailing;

};

std::ostream &operator<<(std::ostream &out, std::vector<std::string> &content);

void dispatch( void );
