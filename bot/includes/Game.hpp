/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:11:09 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 11:19:53 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

struct	t_bot_data;

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

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
		e_state		_gameState;
		e_type		_gameType;
		int			_sock;
		
	public:
		// == Constructor & destructor ==
		Game();
		// Game(int sock, std::string &channel);
		virtual ~Game();


		// === METHODS ===
		/* > Init basic value and update dashboard */
		virtual void initGame(std::string userName, t_bot_data &botData) = 0;

		/* > Start the game in the current channel if there is enought player */
		virtual void startGame(t_bot_data &botData) = 0;
		
		
		// === GETTERS / SETTERS ===
		// == Player ==
		/* > Return the number of player in the game */
		int getNbPlayer( void ) const;

		/* > Return the list of player  */
		std::vector<std::string> getPlayerList( void ) const;

		/* > Add a player to the game */
		virtual int addPlayer(std::string playerName, t_bot_data &botData) = 0;

		/* > Remove a player from the game */
		virtual int removePlayer(std::string playerName, t_bot_data &botData) = 0;
		
		
		// == Game state ==
		/* > Return the current game state */
		e_state getGameState( void ) const;

		/* > Return the game type */
		e_type getGameType( void ) const;

		/* > Set the game state (STARTED, ENDED or WAITING) */
		virtual int setGameState(e_state state, t_bot_data &botData) = 0;

		
		// == Other ==
		/* > Return the channel of the game */
		std::string getChannel( void ) const;
};

std::ostream &operator<<(std::ostream &out, e_state state);

std::string convertState(e_state state);
