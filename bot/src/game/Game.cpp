/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:23:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 10:40:00 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

// == Constructor & destructor ==
Game::Game() {}
Game::~Game() {}

std::ostream &operator<<(std::ostream &out, e_state state) {
	out << ((state == STARTED) ? "started" : 
			(state == ENDED) ? "ended" :
			"waiting");
	return (out);
}

// === GETTERS / SETTERS ===
// == Player ==
int Game::getNbPlayer(void) const { return (_playerList.size()); }

std::vector<std::string> Game::getPlayerList(void) const { return (_playerList); }

// == Game state ==
e_state Game::getGameState(void) const { return (_gameState); }

e_type Game::getGameType(void) const { return (_gameType); }

// == Other ==
std::string Game::getChannel(void) const { return (_channel); }


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
