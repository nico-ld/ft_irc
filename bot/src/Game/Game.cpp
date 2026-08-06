/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:23:06 by nico              #+#    #+#             */
/*   Updated: 2026/08/05 10:45:19 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

// == Constructor & destructor ==
Game::Game() {}
Game::Game(const Game &other) { *this = other; }
Game::~Game() {}

// === OVERLOADS ===
Game &Game::operator=(const Game &other) {
	if (this != &other) {
		_playerList = other._playerList;
		_gameState = other._gameState;
	}
	return (*this);
}

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
