/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:11:09 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 22:02:17 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

struct	t_bot_data;

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <ctime>

enum e_state {
	STARTED,
	ENDED,
	WAITING,
	READY
};

enum e_type {
	UNO,
	WEREWOLF	
};

class Game
{
	protected:
		std::vector<std::string> _playerList;
		std::string	_channel;
		std::string _userHost;
		std::time_t	_timeSinceEnd;
		e_state		_gameState;
		int			_sock;
		
	public:
		// == Constructor & destructor ==
		Game() {}
		virtual ~Game() {}


		// === PURE VIRTUAL METHODS ===
		
		/* > Init basic value and update dashboard */
		virtual void initGame(std::string userName, t_bot_data &botData) = 0;

		/* > Start the game in the current channel if there is enought player */
		virtual void startGame(t_bot_data &botData) = 0;

		/* > Set game state as ENDED */
		virtual void endGame(t_bot_data &botData) = 0;
		
		/* > Set the game state (STARTED, ENDED or WAITING) */
		virtual void setGameState(e_state state, t_bot_data &botData) = 0;
		
		/* > Add a player to the game */
		virtual void addPlayer(std::string playerName, t_bot_data &botData) = 0;
		
		/* > Remove a player from the game */
		virtual void removePlayer(std::string playerName, t_bot_data &botData) = 0;
		
		
		// === GLOBAL METHODS ===
		
		/* > Return the number of player in the game */
		int getNbPlayer( void ) const { return (_playerList.size()); }

		/* > Return the list of player  */
		std::vector<std::string> getPlayerList( void ) const { return (_playerList); }
		
		/* > Return the current game state */
		e_state getGameState( void ) const { return (_gameState); }
		
		/* > Return the channel of the game */
		std::string getChannel( void ) const { return (_channel); }

		/* > Return the time since the game is over */
		std::time_t getTimeSinceEnd( void ) const;
};

std::ostream &operator<<(std::ostream &out, e_state state);

/* > Convert Game state from enum to string */
std::string convertState(e_state state);

/* > Boolean function to check if a player is in the player list */
bool isPlayerInGame(std::string playerName, std::vector<std::string> playerList);
