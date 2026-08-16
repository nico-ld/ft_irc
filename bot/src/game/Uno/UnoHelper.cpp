/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoHelper.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 16:18:25 by nico              #+#    #+#             */
/*   Updated: 2026/08/16 15:25:50 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"

std::vector<e_card> fillPool(void)
{
	std::vector<e_card> pool;

	// Number cards: 4 colors, values 1-9 twice each, '0' once each
	for (int c = 0; c != 52; ++c)
	{
		int weight = (c % 13 == 0) ? 1 : 2; // the '0' of each color is unique
		for (int w = 0; w != weight; ++w)
			pool.push_back(static_cast<e_card>(c));
	}
	for (int i = 0; i != 8; ++i)
		pool.push_back(WILD);
	for (int i = 0; i != 4; ++i)
		pool.push_back(WILD_DRAW_4);

	return (pool);
}

std::string convertCard(e_card card)
{
	static const char *colors[4]   = { "🟡", "🔴", "🔵", "🟢" };
	static const char *ranks[13]  = {
		" 0", " 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9",
		" Skip", " Reverse", " Draw 2"
	};

	if (card == WILD)
		return "🌈 Wild";
	if (card == WILD_DRAW_4)
		return "🌈 Wild Draw 4";

	std::string ret = colors[card / 13];
	ret.append(ranks[card % 13]);

	return (ret);
}

bool isColorValid(std::string color) {	
	if (color == "yellow" || color == "red" || color == "blue" || color == "green")
		return (true);
	else
		return (false);
}

int colorToInt(std::string color) {
	if (color == "yellow")
		return (0);
	else if (color == "red")
		return (1);
	else if (color == "blue")
		return (2);
	else if (color == "green")
		return (3);
	else
		return (-1);
}

std::string colorToString(int color) {
	if (color == 0)
		return ("yellow");
	else if (color == 1)
		return ("red");
	else if (color == 2)
		return ("blue");
	else if (color == 3)
		return ("green");
	else
		throw std::invalid_argument("Invalid color index");
}

bool cardPlayable(e_card card, e_card lastCard, int gameColor) {
	int cardColor = card / 13;
	int cardRank = card % 13;
	
	int lastCardColor = lastCard / 13;
	int lastCardRank = lastCard % 13;

	if (lastCard >= WILD)
	{
		if (card < WILD && cardColor != gameColor)
			return (false);
	}
	else if (cardColor != lastCardColor && cardRank != lastCardRank)
		return (false);
	
	return (true);
}
