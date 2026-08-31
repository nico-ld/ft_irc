/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 10:49:01 by nico              #+#    #+#             */
/*   Updated: 2026/08/31 17:23:56 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

class User;
class Channel;
class Server;

class Parser {
	private:
		// === COMMANDS LISTS ===
		void _initCommandsList( void );
		std::vector<std::string> _commandsChannel;
		std::vector<std::string> _commandsMessage;
		std::vector<std::string> _commandsUser;


		// === PRIVATE ATTRIBUTS ===
		std::string		_prefix;
		std::string		_command;
		std::string		_trailing;
		std::string		_rawString;
		unsigned int	_commandListId;
		std::vector<std::string> _parameters;

	public:
		// == Constructor & destructor ==
		Parser();
		Parser(const Parser &other);
		~Parser();

		// == Overload ==
		Parser &operator=(const Parser &other);
		
		// === PARSER METHODS ===
		/* > Parse and tokenzie the line received, return an error code if necessary */
		int parse(std::string &line);

		/* > Parse the wanted command */
		int parseCommand(std::string word);


		// === GETTERS ===
		/* > Return line prefix */
		std::string getPrefix( void ) const { return (_prefix); }

		/* > Return the wanted command */
		std::string getCommand( void ) const { return (_command); }

		/* > Return the trailing parameter */
		std::string getTrailing( void ) const { return (_trailing); }

		/* > Return the raw line to echoes input */
		std::string getRawString ( void ) const { return (_rawString); }
		
		/* > Return the command id */
		unsigned int getCommandId( void ) const { return (_commandListId); }

		/* > Return the list of parameters */
		std::vector<std::string> getParameters( void ) const { return (_parameters); }

		// === HELPERS ===
		/* > Return a list of channel from command parameter */
		std::vector<Channel> getChannelList(std::string parameter, Server &server, User &user);

		/* > Return a list of key from command parameter */
		std::vector<std::string> getKeyList(std::string parameter);
		
		/* > Skip the message target and return every other params needed by PRIVMSG or NOTICE */
		std::string getMessage( void );

		/* > Parse the name, return True if this is valid for a channel name, otherwise return False */
		bool checkChannelName(std::string name);
};

std::ostream &operator<<(std::ostream &out, std::vector<std::string> &content);

/* After parsing the user input, this function take the targer user and call the good command */
void dispatchCommand(Server &server, User &user, std::string command);

/* Manage command USER, NICK and PASS */
void userCommandsDispatch(std::string command, User &user, Server &server, Parser &parser);

/* Manage PRIVMSG command */
void messageCommandsDispatch(Server &server, std::string command, User &user, Parser &parser);

/* Manage channel commands (JOIN, LEAVE, KICK, etc...) */
void channelCommandsDispatch(Server &server, std::string command, User &user, Parser &parser);
