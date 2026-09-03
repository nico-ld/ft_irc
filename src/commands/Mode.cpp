/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:46:21 by afons             #+#    #+#             */
/*   Updated: 2026/09/03 21:09:41 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"
#include "Parser.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

// Applies each requested change in settings (mode) to a specific chat room (channel).
// Modes enable or disable room features like invite-only, passwords, or member limits.
void Server::launchMode(Channel &channel, std::vector<std::string> modestring, std::vector<std::string> params, User *user) {
	std::vector<std::string>::iterator it_modestring = modestring.begin();
	std::vector<std::string>::iterator it_params;
	std::vector<t_mode_reply> replyMessageContent;
	if (params.size() > 0) it_params = params.begin();

	// Process letter setting for ADDING then REMOVING
	for(; it_modestring != modestring.end(); ++it_modestring) {
		size_t i = 0;

		
		/*=================*\
		|			 	    |
		|    ADDING FLAG    |
		|				    |
		\*=================*/
		
		if ((*it_modestring)[i] == '+') {
			i++;
			while((*it_modestring)[i]) {
			    
				
				// i : Make the room invite-only //
				if ((*it_modestring)[i] == 'i') {
					channel.setInviteOnly(true);
					replyMessageContent.push_back(addNode('i', true, ""));
				}


				// t : Restrict room topics to administrators choice //
				else if ((*it_modestring)[i] == 't') {
					channel.setTopicRestricted(true);
					replyMessageContent.push_back(addNode('t', true, ""));
				}


				// k : Set a room password //
				else if ((*it_modestring)[i] == 'k') {
					if (params.size() <= 0 || it_params == params.end()) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Missing parameter for +k MODE flag");
						sendReply(*user, ERR_NEEDMOREPARAMS, "Missing parameter for +k MODE flag");
						i++;
						continue ;
					}

					channel.setKey(*it_params);
					replyMessageContent.push_back(addNode('k', true, *it_params));
					it_params++;
				}

				
				// l : Set a maximum user limit for the room //
				else if ((*it_modestring)[i] == 'l') {
					if (params.size() <= 0 || it_params == params.end()) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Missing parameter for +l MODE flag");
						sendReply(*user, ERR_NEEDMOREPARAMS, "Missing parameter for +l MODE flag");
						i++;
						continue;
					}

					std::stringstream ss(*it_params);
					int limit;
					if (!(ss >> limit) || limit <= 0) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Invalid parameter for +l MODE flag");
						sendReply(*user, ERR_UNKNOWNMODE, "Invalid parameter for +l flag. Valid parameter is positive and non-null integer");
						i++;
						continue ;
					}

					channel.setUserLimit(limit);
					replyMessageContent.push_back(addNode('l', true, *it_params));
					it_params++;
				}

				
				// o : Grant administrator/operator privileges to another user (requires user's name as parameter) //
				else if ((*it_modestring)[i] == 'o') {
					if (params.size() <= 0 || it_params == params.end()) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Missing parameter for +o MODE flag");
						sendReply(*user, ERR_NEEDMOREPARAMS, "Missing parameter for +o MODE flag");
						++i;
						continue ;
					}
					
					User *target = getUserByNickname(*it_params);
					if (!target) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to promote a user who doesn't exist");
						sendReply(*user, ERR_NOSUCHNICK, "User '" + *it_params + "' doesn't exist");
						++i;
						continue ;
					}
					else if (!channel.isMember(target->getFd())) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to promote a user who is not on the channel");
						sendReply(*user, ERR_USERNOTINCHANNEL, "User '" + *it_params + "' is not on the channel");
						++i;
						continue ;
					}
					
					channel.addOperator(target);
					replyMessageContent.push_back(addNode('o', true, *it_params));
					it_params++;
				}
				
				
				// Unknow flag //
				else {
					std::string message = "Unknow flag : ";
					message += (*it_modestring)[i];
					dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": " + message);
					sendReply(*user, ERR_UNKNOWNMODE, message);
				}
				++i;
			}
		}

		
		/*=================*\
		|			 	    |
		|    DELETE FLAG    |
		|				    |
		\*=================*/
		
		else if ((*it_modestring)[i] == '-') {
			i++;
			while((*it_modestring)[i]) {

				
				// i: Remove invite-only mode //
				if ((*it_modestring)[i] == 'i') {
					channel.setInviteOnly(false);
					replyMessageContent.push_back(addNode('i', false, ""));
				}

				
				// t: Remove restricted topic access (everyone can change topic) //
				else if ((*it_modestring)[i] == 't') {
					channel.setTopicRestricted(false);
					replyMessageContent.push_back(addNode('t', false, ""));
				}


				// k: Remove key to join channel (no more password is needeed) //
				else if ((*it_modestring)[i] == 'k') {
					channel.setKey("");
					replyMessageContent.push_back(addNode('k', false, ""));
				}


				// l: Remove member limit // 
				else if ((*it_modestring)[i] == 'l') {
					channel.setUserLimit(-1);
					replyMessageContent.push_back(addNode('l', false, ""));
				}


				// o: Remove channel operator privilege to the target
				else if ((*it_modestring)[i] == 'o') {
					if (params.size() <= 0 || it_params == params.end()) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Missing parameter for -o MODE flag");
						sendReply(*user, ERR_NEEDMOREPARAMS, "Missing parameter for -o MODE flag");
						++i;
						continue ;
					}

					User *target = getUserByNickname(*it_params);
					if (!target) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to promote a user who doesn't exist");
						sendReply(*user, ERR_NOSUCHNICK, "User '" + *it_params + "' doesn't exist");
						++i;
						continue ;
					}
					else if (!channel.isMember(target->getFd())) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to promote a user who is not on the channel");
						sendReply(*user, ERR_USERNOTINCHANNEL, "User '" + *it_params + "' is not on the channel");
						++i;
						continue ;
					}
					else if (!channel.isOperator(target->getFd())) {
						dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Trying to remove channel operator privilege to an user who is not operator");
						sendReply(*user, ERR_CHANOPRIVSNEEDED, "User '" + target->getNickname() + "' isn't operator");
						++i;
						continue ;
					}
					
					channel.removeOperator(target);
					replyMessageContent.push_back(addNode('o', false, *it_params));
					it_params++;
				}


				// Unknow flag //
				else {
					std::string message = "Unknow flag : ";
					message += (*it_modestring)[i];
					dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": " + message);
					sendReply(*user, ERR_UNKNOWNMODE, message);
				}
				i++;
			}
		}
	}
	
	
	// Send reply message
	if (!replyMessageContent.empty()) {
		std::string message = user->getPrefix() + " MODE " + channel.getName() + " ";
		broadcast(channel, message + createReplyMessage(replyMessageContent));
	}
}

// Entry point for handling a user's request to change room settings.
// It verifies that the room exists, validates the user has permission, and triggers the settings update.
void Server::mode(Channel &channel, std::string listMode, User *user, std::vector<std::string> params) {
	// Ensure the user requesting changes is a room administrator
	if (!channel.isOperator(user->getFd())) {
		dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": User need channel operator privilege to use MODE");
		sendReply(*user, ERR_CHANOPRIVSNEEDED, "You need channel operator privilege to use MODE");
		return ;
	}

	if (listMode[0] != '+' && listMode[0] != '-') {
		dash->log(WARNING, "Fd : " + toStr(user->getFd()) + ": Missing operator at start of flag list ('+' or '-')");
		sendReply(*user, ERR_UNKNOWNMODE, "Missing operator at start of flag list ('+' or '-')");
		return ;
	}

	// Break down the settings string into manageable pieces
	std::vector<std::string> modestring = split_mode(listMode);

	// Apply the parsed settings to the room
	launchMode(channel, modestring, params, user);
}
