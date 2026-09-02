/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeHelper.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:36:04 by nico              #+#    #+#             */
/*   Updated: 2026/09/02 11:50:02 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//Handle MODE command without argument neither option
std::string Server::displayChannelStatus(Channel &channel) {
	std::string status;
	std::string parameter;

	if (channel.isInviteOnly())
		status += "i";
	if (channel.isTopicRestricted())
		status += "t";
	if (channel.getKey().size()) {
		status += "k";
		parameter += channel.getKey() + " ";
	}
	if (channel.getUserLimit() != -1) {
		status += "l";
		std::stringstream ss;
		ss << channel.getUserLimit();
		parameter += ss.str()  + " ";
	}
	return ("+" + status + " " + parameter);
}

// Breaks down a raw string of settings (like "+i-t+k") into individual action groups
// (like ["+i", "-t", "+k"]) so they can be processed one by one.
std::vector<std::string> split_mode(std::string listMode) {
	std::vector<std::string> listString;
	std::string::iterator it = listMode.begin();

	while(it != listMode.end()) {
	    // Skip + and - to find the being pos of the letters
		while (it != listMode.end() && (*it == '+' || *it == '-')) {
			++it;
		}
		
		std::string::iterator pos_tmp = it;
		
		// Skip until another + or - to find the end pos of the letters
		while (it != listMode.end() && *it != '+' && *it != '-') {
			++it;
		}
		
		// Extract the letters part, to then happen it as a string to the list
		std::string string = listMode.substr(pos_tmp - listMode.begin() - 1, it - pos_tmp + 1);
		listString.push_back(string);
	}
	return (listString);
}

t_mode_reply addNode(char mode, bool adding, std::string param) {
	t_mode_reply node;
	node.mode = mode;
	node.adding = adding;
	node.param = param;
	return (node);
}

std::string createReplyMessage(std::vector<t_mode_reply> messageContent) {
	int	adding = 0;
	std::string message;
	std::string parameters;

	for (std::vector<t_mode_reply>::iterator it = messageContent.begin(); it != messageContent.end(); ++it) {
		// Adding or remove mode
		if (adding < 1 && it->adding == true)
			message.append("+");
		else if (adding > -1 && it->adding == false)
			message.append("-");
			
		// MODE flag
		message += it->mode;
	
		// flag parameter
		if (!it->param.empty()) {
			if (!parameters.empty())
				parameters.append(" ");
			parameters.append(it->param);
		}
	}

	if (!parameters.empty())
		message += " " + parameters;
	return (message);
}
