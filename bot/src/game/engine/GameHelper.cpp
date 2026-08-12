/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameHelper.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 09:11:07 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 11:26:57 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

std::string convertState(e_state state)
{
	switch (state)
	{
		case WAITING:
			return ("WAITING");
		case READY:
			return ("READY");
		case STARTED:
			return ("STARTED");
					
		default:
			return ("ENDED");
	}
}

std::string convertType(e_type type)
{
	switch (type)
	{
		case UNO:
			return ("UNO");
		case WEREWOLF:
			return ("WEREWOLF");
					
		default:
			return ("UNKNOW");
	}
}

bool isPlayerInGame(std::string playerName, std::vector<std::string> playerList) {
	if (playerList.empty())
		return (false);
	
	for (std::vector<std::string>::iterator it = playerList.begin(); it != playerList.end() ; ++it) {
		if (playerName == *it)
			return (true);
	}
	return (false);
}

