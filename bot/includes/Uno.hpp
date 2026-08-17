/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:13 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 11:03:46 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UNO_HPP
# define UNO_HPP

struct	t_bot_data;

#include "Game.hpp"
#include <sys/socket.h>
#include <algorithm>
#include <stdexcept>

// 108 cards total: 4 colors x (10 numbers + skip + reverse + draw2) = 52, plus 8 wild + 4 wild_draw4
enum e_card {
	Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8, Y9, Y_SKIP, Y_REVERSE, Y_DRAW_2, // Yellow (13)
	R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R_SKIP, R_REVERSE, R_DRAW_2, // Red (13)
	B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B_SKIP, B_REVERSE, B_DRAW_2, // Blue (13)
	G0, G1, G2, G3, G4, G5, G6, G7, G8, G9, G_SKIP, G_REVERSE, G_DRAW_2, // Green (13)
	WILD,        // 8 cards
	WILD_DRAW_4  // 4 cards
};

struct t_player_info {
	std::vector<e_card> deck;
	std::string	name;
	bool		unoMode;
	bool		wildDrawed;
	int			drawAmount;
};

class Uno: public Game
{
	private:
		// Pool of every cards in the uno game
		std::vector<e_card> _pool;

		// Player informations
		std::vector<t_player_info> _playerInfo;

		// Round informations
		std::string	_playerTurn;
		std::string	_currentColor;
		std::string	_lastColor;
		e_card		_lastCard;
		bool		_reversed;

	public:
		// == Constuctor & destructor ==
		Uno(int sock, std::string &channel);
		~Uno();

		// Pure virtual method
		void launchGame(t_bot_data &botData);


		// === GETTERS ===

		/* > Send the deck of a player from _deckList or init a new deck if player is not in the list */
		std::vector<e_card> getDeckByUser(std::string userName) const;
		
		/* > Return information about a player in the game */
		t_player_info &getPlayerInfo(std::string userName);
		
		/* > Return the next player */
		std::string nextPlayer(bool skip) const;

		/* > Return the previous player */
		std::string previousPlayer( void ) const;

		/* > Show how many card everyone gets */
		void showCardsAmount(t_bot_data &botData) const;

		/* > Return wich player have to play */
		std::string getPlayerTurn( void ) const;

		
		// === DECK ===

		/* > Send cards to a specif user by private message */
		void sendCard(t_bot_data &botData, std::string userName, int amount);

		/* > Set the deck of a player into _deckList */
		void setUserDeck(std::string userName, std::vector<e_card> deck);

		/* > Send, by private message, player deck */
		void showHand(t_bot_data &botData, std::string userName);


		// === PLAYER COMMANDS ===
		
		/* > Tell wich player have to play in the channel */
		void whoseTurn(t_bot_data &botData) const;

		/* > Tell the last card played */
		void top(t_bot_data &botData) const;

		/* > Play a card */
		void playCard(t_bot_data &botData, std::string userName, std::string deckIdx, std::string color);
		
		/* > When a player get only one card */
		void uno(t_bot_data &botData, std::string userName);

		/* > Challenge a player that play a WILD_DRAW_4 */
		void challenge(t_bot_data &botData, std::string userName);

		/* > Draw cards */
		void draw(t_bot_data &botData, t_player_info &player);

		/* > In case of wild card drawed, player have to set manually the color */
		void color(t_bot_data &botData, t_player_info &player, std::string color);
		
		// === SPECIAL CASES ===
		
		/* > handle case of a draw, return 1 on error */
		int drawManagement(t_bot_data &botData, t_player_info &player, e_card card);
};

/* > Fill a vector with every card in the game, then sendCard() can random on this vector to choose card */
std::vector<e_card> fillPool( void );

/* > Convert card from enum to string */
std::string convertCard(e_card card);

/* > Parse the color */
bool isColorValid(std::string color);

/* > Convert a color into an index (int), return -1 on invalid color */
int colorToInt(std::string color);

/* > Convert a color index into a string, throw an error on invalid index */
std::string colorToString(int color);

/* > Check if player can play this card */
bool cardPlayable(e_card card, e_card lastCard, int gameColor);

#endif
