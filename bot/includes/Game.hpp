/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:11:09 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 10:31:50 by nico             ###   ########.fr       */
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
	WEREWOLF,
	UNKNOW // Delimiter in case of loop
};

class Game
{
	protected:
		std::vector<std::string> _playerList;
		std::string	_channel;
		std::time_t	_timeSinceEnd;
		e_state		_gameState;
		e_type		_gameType;
		int			_sock;
		
	public:
		// == Constructor & destructor ==
		Game();
		virtual ~Game();


		// === PURE VIRTUAL METHODS ===
		
		/* > Init basic value and update dashboard */
		void initGame(std::string userName, t_bot_data &botData);

		/* > Start the game in the current channel if there is enought player */
		void startGame(t_bot_data &botData);

		/* > Set game state as ENDED */
		void endGame(t_bot_data &botData);
		
		/* > Set the game state (STARTED, ENDED or WAITING) */
		void setGameState(e_state state, t_bot_data &botData);
		
		/* > Add a player to the game */
		void addPlayer(std::string playerName, t_bot_data &botData);
		
		/* > Remove a player from the game */
		void removePlayer(std::string playerName, t_bot_data &botData);
		
		
		// === GETTERS ===
		
		/* > Return the number of player in the game */
		int getNbPlayer( void ) const;

		/* > Return the list of player  */
		std::vector<std::string> getPlayerList( void ) const;
		
		/* > Return the channel of the game */
		std::string getChannel( void ) const;
		
		/* > Return the time since the game is over */
		std::time_t getTimeSinceEnd( void ) const;
		
		/* > Return the current game state */
		e_state getGameState( void ) const;

		/* > Return the type of game (Uno or Werewolf) */
		e_type getGameType( void ) const;
};

std::ostream &operator<<(std::ostream &out, e_state state);

/* > Convert game state from enum to string */
std::string convertState(e_state state);

/* > Convert game type from enum to string */
std::string convertType(e_type type);

/* > Boolean function to check if a player is in the player list */
bool isPlayerInGame(std::string playerName, std::vector<std::string> playerList);
