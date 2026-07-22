/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afons <afons@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 15:54:54 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/21 14:30:51 by afons            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>

class User;
class Server;
class Parser
{
	public:
		// === PUBLIC METHODS ===
		/* Parse and tokenize user input */
		static void	parse(std::string &input);

		/* Reset private attribut */
		static void clearParser( void );
		
		/* Init every commands list */
		static void initCommandList( void );

		/* Build origin prefix, if there is an existing prefix, it overwrite the function */
		static void buildPrefix(User &user);

		
		// === GETTERS ===
		/* Return the prefix of user input as string */
		static std::string getPrefix( void ) {return (_prefix);}
		
		/* Return the command of user input */
		static std::string getCommand( void ) {return (_command);}

		/* Return the Id of command list {1: Channel commands; 2: Message commands; 3: User commands} */
		static unsigned int getCommandListId( void ) {return (_commandListId);}

		/* Return the list of paramaters (no trailing) as vector */
		static std::vector<std::string> getParameters( void ) {return (_parameters);}

		/* Return the trailing parameter as vector */
		static std::vector<std::string> getTrailing( void ) {return (_trailing);}

		
		// === EXCEPTION ===
		class InvalidCommandException: public std::exception {
			public: virtual const char *what() const throw() {return ("Error: invalid command.");}
		};
		
	private:
		Parser() {}
		~Parser() {}

		// === PRIVATE METHODS ===
		/* Search command, if command is unknow _command  stay empty and an error is throwed */
		static void parseCommand(std::string &input);


		// === COMMANDS LISTS ===
		static bool	_listInit;
		static std::vector<std::string> _commandsChannel;
		static std::vector<std::string> _commandsMessage;
		static std::vector<std::string> _commandsUser;


		// === PRIVATE ATTRIBUTS ===
		static std::string _prefix;
		static std::string _command;
		static unsigned int _commandListId;
		static std::vector<std::string> _parameters;
		static std::vector<std::string> _trailing;

};

std::ostream &operator<<(std::ostream &out, std::vector<std::string> &content);

/* After parsing the user input, this function take the targer user and call the good command */
void dispatchCommand(Server &server, User &user);