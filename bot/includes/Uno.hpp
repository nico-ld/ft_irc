/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:13 by nico              #+#    #+#             */
/*   Updated: 2026/08/15 10:43:32 by nico             ###   ########.fr       */
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

class Uno: public Game
{
	private:
		std::vector<std::pair<std::string, std::vector<e_card> > > _deckList;
		std::vector<e_card> _pool;

	public:
		// == Constuctor & destructor ==
		Uno(int sock, std::string &channel);
		~Uno();

		void launchGame(t_bot_data &botData);

		/* > Send cards to a specif user by private message */
		void sendCard(t_bot_data &botData, std::string userName, int amount);

		/* > Send the deck of a player from _deckList or init a new deck if player is not in the list */
		std::vector<e_card> getDeckByUser(std::string userName);

		/* > Set the deck of a player into _deckList */
		void setUserDeck(std::string userName, std::vector<e_card> deck);
};

/* > Fill a vector with every card in the game, then sendCard() can random on this vector to choose card */
std::vector<e_card> fillPool( void );

/* > Convert card from enum to string */
std::string convertCard(e_card card);

#endif
