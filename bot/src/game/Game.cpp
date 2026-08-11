/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:23:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 21:59:59 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

// === OVERLOADS ===

std::ostream &operator<<(std::ostream &out, e_state state) {
	out << ((state == STARTED) ? "started" : 
			(state == ENDED) ? "ended" :
			"waiting");
	return (out);
}

// === GETTERS ===

std::time_t Game::getTimeSinceEnd(void) const {
	if (_gameState == ENDED)
		return (_timeSinceEnd);
	else
		return (0);
}

// === HELPERS ===

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

bool isPlayerInGame(std::string playerName, std::vector<std::string> playerList) {
	for (std::vector<std::string>::iterator it = playerList.begin(); it != playerList.end() ; ++it) {
		if (playerName == *it)
			return (true);
	}
	return (false);
}

