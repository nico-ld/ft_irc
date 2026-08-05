/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:11:09 by nico              #+#    #+#             */
/*   Updated: 2026/08/05 10:38:23 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/colors.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

enum e_state {
	STARTED,
	ENDED,
	WAITING	
};

class Game
{
	protected:
		std::vector<std::string> _playerList;
		e_state	_gameState;
		
	public:
		// == Constructor & destructor ==
		Game();
		Game(const Game &other);
		~Game();

		
		// === OVERLOADS ===
		Game &operator=(const Game &other);

		
		// === GETTERS / SETTERS ===
		// == Player ==
		/* > Return the number of player in the game */
		int getNbPlayer( void ) const;

		/* > Return the list of player  */
		std::vector<std::string> getPlayerList( void ) const;

		/* > Add a player to the game */
		void addPlayer(std::string playerName);

		/* > Remove a player from the game */
		void removePlayer(std::string playerName);
		
		// == Game state ==
		/* > Return the current game state */
		e_state getGameState( void ) const;

		/* > Set the game state (STARTED, ENDED or WAITING) */
		void setGameState(e_state state);
};

std::ostream &operator<<(std::ostream &out, e_state state);
