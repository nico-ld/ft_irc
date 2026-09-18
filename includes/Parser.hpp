/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 10:49:01 by nico              #+#    #+#             */
/*   Updated: 2026/09/18 07:40:42 by jdessoli         ###   ########.fr       */
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
		void _initCommandsList( void );
		std::vector<std::string> _commandsChannel;
		std::vector<std::string> _commandsMessage;
		std::vector<std::string> _commandsUser;

		bool			_isTrailing;
		std::string		_prefix;
		std::string		_command;
		std::string		_trailing;
		std::string		_rawString;
		unsigned int	_commandListId;
		std::vector<std::string> _parameters;

	public:
		Parser();
		Parser(const Parser &other);
		Parser &operator=(const Parser &other);
		~Parser();
		
		// Parse and tokenize the line received, return an error code if necessary
		int parse(std::string &line);
		int parseCommand(std::string word);


		std::string getPrefix( void ) const { return (_prefix); }
		std::string getCommand( void ) const { return (_command); }

		//Return a boolean to know if there is a trailing param
		bool isTrailing( void ) const { return (_isTrailing); }

		std::string getTrailing( void ) const { return (_trailing); }

		//Return the raw line to echoes input
		std::string getRawString ( void ) const { return (_rawString); }
		
		unsigned int getCommandId( void ) const { return (_commandListId); }

		// Return the list of params
		std::vector<std::string> getParameters( void ) const { return (_parameters); }

		std::vector<Channel> getChannelList(std::string parameter, Server &server, User &user);
		std::vector<std::string> getKeyList(std::string parameter);
		
		// Skip the message target and return every other params needed by PRIVMSG or NOTICE
		std::string getMessage( void );

		// Parse the name to see if it's valid for a channel name
		bool checkChannelName(std::string name);
};

std::ostream &operator<<(std::ostream &out, std::vector<std::string> &content);

//Call the good command upon user's input
void dispatchCommand(Server &server, User &user, std::string command);

// Manage command USER, NICK and PASS
void userCommandsDispatch(std::string command, User &user, Server &server, Parser &parser);

// Manage PRIVMSG command
void messageCommandsDispatch(Server &server, std::string command, User &user, Parser &parser);

// Manage channel commands (JOIN, LEAVE, KICK, etc...)
void channelCommandsDispatch(Server &server, std::string command, User &user, Parser &parser);
