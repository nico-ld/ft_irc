/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:23:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 09:43:06 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game() {
	std::srand(std::time(NULL));
}

Game::~Game() {}

// === OVERLOADS ===

std::ostream &operator<<(std::ostream &out, e_state state) {
	out << ((state == STARTED) ? "started" : 
			(state == ENDED) ? "ended" :
			"waiting");
	return (out);
}

// === GETTERS ===

int Game::getNbPlayer( void ) const { return (_playerList.size()); }

std::vector<std::string> Game::getPlayerList( void ) const { return (_playerList); }

std::string Game::getChannel( void ) const { return (_channel); }

std::time_t Game::getTimeSinceEnd( void ) const {
	if (_gameState == ENDED)
		return (_timeSinceEnd);
	else
		return (0);
}

e_state Game::getGameState( void ) const { return (_gameState); }

e_type Game::getGameType( void ) const { return (_gameType); }
