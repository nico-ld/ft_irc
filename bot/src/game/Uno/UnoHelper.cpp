/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoHelper.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 16:18:25 by nico              #+#    #+#             */
/*   Updated: 2026/08/15 17:04:00 by nico             ###   ########.fr       */
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
