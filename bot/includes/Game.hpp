/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:11:09 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 16:52:12 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Dashboard.hpp"
#include <iostream>
#include <vector>
#include <algorithm>


enum e_state {
	STARTED,
	ENDED,
	WAITING,
	READY
};

class Game
{
	protected:
		std::vector<std::string> _playerList;
		std::string	_channel;
		std::string _userHost;
		e_state		_gameState;
		int			_sock;
		
	public:
		// == Constructor & destructor ==
		Game();
		// Game(int sock, std::string &channel);
		virtual ~Game();


		// === METHODS ===
		virtual void initGame(std::string &userName, DashData &data, Dashboard &dash) = 0;
		
		
		// === GETTERS / SETTERS ===
		// == Player ==
		/* > Return the number of player in the game */
		int getNbPlayer( void ) const;

		/* > Return the list of player  */
		std::vector<std::string> getPlayerList( void ) const;

		/* > Add a player to the game */
		virtual int addPlayer(std::string playerName, DashData &data, Dashboard &dash) = 0;

		/* > Remove a player from the game */
		virtual int removePlayer(std::string playerName, DashData &data, Dashboard &dash) = 0;
		
		
		// == Game state ==
		/* > Return the current game state */
		e_state getGameState( void ) const;

		/* > Set the game state (STARTED, ENDED or WAITING) */
		virtual int setGameState(e_state state, DashData &data, Dashboard &dash) = 0;
};

std::ostream &operator<<(std::ostream &out, e_state state);

std::string convertState(e_state state);
