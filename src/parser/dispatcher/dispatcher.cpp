/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 09:32:14 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/27 19:13:21 by jdessoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

/* Commands to manage channel and user */
static void channelCommandsDispatch(std::string command, User &user) {
    std::vector<std::string> parameters = Parser::getParameters();

    if (command == "join") {
        if (parameters.empty())
            throw std::runtime_error("Error: JOIN requires at least one channel name.");

        // The user enters a chat room. If they provided a password (key), 
        // the server unlocks the room and lets them in; otherwise, they enter normally.
        std::string channel = parameters[0];
        std::string key = (parameters.size() > 1) ? parameters[1] : "";

        if (!key.empty())
            Server::join(channel, key, user);
        else
            Server::join(channel, user);
    }
    else if (command == "part") {
        if (parameters.empty())
            throw std::runtime_error("Error: PART requires a channel name.");

        // The user voluntarily leaves a chat room, optionally leaving behind 
        // a goodbye message for the other people in that room.
        std::string channel = parameters[0];
        std::string reason = (parameters.size() > 1) ? parameters[1] : "";
        Server::part(channel, reason, user);
    }
    else if (command == "topic") {
        if (parameters.empty())
            throw std::runtime_error("Error: TOPIC requires a channel name.");

        // If a new description is provided, update the chat room's main topic notice.
        // If no new description is provided, simply show the current topic to the user.
        std::string channel = parameters[0];
        if (parameters.size() > 1) {
            Server::setTopic(channel, parameters[1], user);
        } else {
            Server::getTopic(channel, user);
        }
    }
    else if (command == "kick") {
        if (parameters.size() < 2)
            throw std::runtime_error("Error: KICK requires a channel and a user nickname.");

        // A moderator forcibly removes a specific person from the chat room,
        // optionally displaying a reason for why they were removed.
        std::string channel = parameters[0];
        std::string targetUser = parameters[1];
        std::string reason = (parameters.size() > 2) ? parameters[2] : "";
        Server::kick(channel, targetUser, reason, user);
    }
    else if (command == "mode") {
        if (parameters.empty())
            throw std::runtime_error("Error: MODE requires a channel or nickname target.");

        // Changes the rules or settings of a chat room (like making it invite-only 
        // or setting a user password) or views current room permissions.
        std::string target = parameters[0];
        std::string modes = (parameters.size() > 1) ? parameters[1] : "";
        std::string params = (parameters.size() > 2) ? parameters[2] : "";
        Server::mode(target, modes, params, user);
    }
    else {
        std::cout << "Channel command '" << command << "' is not handled." << std::endl;
    }
}

/* Commands to send a message */
static void messageCommandsDispatch(std::string command, User &user) {
    std::vector<std::string> parameters = Parser::getParameters();

    if (command == "privmsg") {
        if (parameters.empty())
            throw std::runtime_error("Error: PRIVMSG requires a target recipient.");
        if (parameters.size() < 2)
            throw std::runtime_error("Error: PRIVMSG requires a message body.");
        // Sends a direct message to either a specific person or an entire chat room.
        // If the message cannot be delivered, the server lets the sender know.

        std::string target = parameters[0];
        std::string message = parameters[1];
        Server::privmsg(target, message, user);
    }
    else if (command == "notice") {
        if (parameters.empty())
            throw std::runtime_error("Error: NOTICE requires a target recipient.");
        if (parameters.size() < 2)
            throw std::runtime_error("Error: NOTICE requires a message body.");
        // Sends an automated notification or notice to a person or chat room.
        // Unlike regular messages, this never triggers automated error replies back 
        // to prevent infinite messaging loops between automated bots.

        std::string target = parameters[0];
        std::string message = parameters[1];
        Server::notice(target, message, user);
    }
    else {
        std::cout << "Message command '" << command << "' is not handled." << std::endl;
    }
}

/* Commands to register a user */
static void userCommandsDispatch(std::string command, User &user) {
	std::vector<std::string> parameters = Parser::getParameters();
	std::vector<std::string> trailing = Parser::getTrailing();

	if (command == "user") {
		if (parameters.size() < 3 && trailing.empty())
			throw std::runtime_error("Error: USER command requires username and realname parameters.");

		// Registers the user's identity profile with the server. It stores their 
		// account details, using the last parameter provided as their actual full name.
		// Set realname using the trailing string parameter if available, 
		// otherwise fall back to the last parameter provided.
		std::string realname = !trailing.empty() ? trailing[0] : parameters.back();
		user.setRealname(realname);
		user.setProvidedUser(true);
	}
	else if (command == "nick") {
		if (parameters.empty())
			throw std::runtime_error("Error: NICK command requires a nickname.");
		
		// Assigns or changes the display name (nickname) that other users 
		// in the chat server will see them by.
		user.setNickname(parameters[0]);
		user.setProvidedNick(true);
	}
	else if (command == "pass") {
		if (parameters.empty())
			throw std::runtime_error("Error: PASS command requires a password.");

		// Verifies the connection password given by the user to make sure 
		// they are authorized to access this server.
		if (parameters[0] != Server::getPassword())
			throw std::runtime_error("Error: Invalid password.");
		user.setProvidedPassword(true);
	}

	if (user.hasProvidedNick() && user.hasProvidedUser() && user.hasProvidedPassword())
		user.setAuthenticated(true);
}

void dispatchCommand(User &user) {
	std::string command = Parser::getCommand();
	Parser::buildPrefix(user);

	// Case 3 corresponds to user registration commands (PASS, NICK, USER).
	// For all other commands, verify the user has fully registered before proceeding.
	if (Parser::getCommandListId() != 3 && !user.isAuthenticated()) {
		throw std::runtime_error("Error: You must authenticate (PASS, NICK, USER) before sending commands.");
	}

	switch (Parser::getCommandListId())
	{
		case 1:
			// Routes incoming actions related to managing chat rooms and channel membership.
			channelCommandsDispatch(command, user);
			break ;

		case 2:
			// Routes requests to send messages or notices to individuals or channels.
			messageCommandsDispatch(command, user);
			break ;

		case 3:
			// Routes user account setup commands needed to log in and register with the server.
			userCommandsDispatch(command, user);
			break ;

		default:
			// The server received an unknown command category or unhandled input.
			throw Parser::InvalidCommandException();
			break ;
	}
}
