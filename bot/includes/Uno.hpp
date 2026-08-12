/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Uno.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:44:13 by nico              #+#    #+#             */
/*   Updated: 2026/08/12 10:54:43 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

struct	t_bot_data;

#include "Game.hpp"
#include <sys/socket.h>
#include <algorithm>
#include <stdexcept>

enum e_card {
	Y1, Y2, Y3, Y4, Y5, Y6, Y7, Y8, Y9, Y0, // Yellow
	R1, R2, R3, R4, R5, R6, R7, R8, R9, R0, // Red
	B1, B2, B3, B4, B5, B6, B7, B8, B9, B0, // Blue
	G1, G2, G3, G4, G5, G6, G7, G8, G9, G0, // Green
};

enum e_action_card {
	Y_SKIP, R_SKIP, B_SKIP, G_SKIP,
	Y_REVERSE, R_REVERSE, B_REVERSE, G_REVERSE,
	Y_DRAW_2, R_DRAW_2, B_DRAW_2, G_DRAW_2,
	WILD, WILD_DRAW_4
};

class Uno: public Game
{
	public:
		// == Constuctor & destructor ==
		Uno(int sock, std::string &channel);
		~Uno();
};
