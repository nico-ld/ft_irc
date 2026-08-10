/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:23:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 09:15:31 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include <stdexcept>

// == Constructor & destructor ==
Game::Game() {
	throw std::runtime_error("Error: no socket and no channel given");
}

Game::Game(int sock, std::string &channel) {
	_sock = sock;
	_channel = channel;
}

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

void Game::addPlayer(std::string playerName) { _playerList.push_back(playerName); }

void Game::removePlayer(std::string playerName) {
	std::vector<std::string>::iterator it;
	it = std::find(_playerList.begin(), _playerList.end(), playerName);
	
	if (it != _playerList.end())
		_playerList.erase(it);
	else
		std::cerr << ERROR "Player not found" << std::endl;
}

// == Game state ==
e_state Game::getGameState(void) const { return (_gameState); }

void Game::setGameState(e_state state) { _gameState = state; }
