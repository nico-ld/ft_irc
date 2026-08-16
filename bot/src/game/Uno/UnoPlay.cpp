/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoPlay.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 16:12:18 by nico              #+#    #+#             */
/*   Updated: 2026/08/15 17:04:36 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Uno.hpp"
#include "Bot.hpp"

/* > Manage wild card, return 0 on success, 1 on error */
static int checkWild(t_bot_data &botData, std::string channel, e_card card, std::string color) {	
	if (color.empty()) {
		sendMessage(botData, channel, "You need to give a color to play this card", WARNING);
		return (1);
	}
	
	// Case-insensitive
	std::transform(color.begin(), color.end(), color.begin(), ::tolower);

	if (!isColorValid(color)) {
		sendMessage(botData, channel, "Invalid color. Valid color are : yellow, red, blue, green", WARNING);
		return (1);
	}
	return (0);
}

void Uno::playCard(t_bot_data &botData, std::string userName, std::string cardIdx, std::string color) {
	char *end;

	// Invalid char protection
	size_t index = strtol(cardIdx.c_str(), &end, 10);
	if (end) {
		sendMessage(botData, _channel, "Index is an invalid value", WARNING);
		return ;
	}

	std::vector<e_card> deck = getDeckByUser(userName);

	// Index out of range protection
	if (index > deck.size()) {
		sendMessage(botData, _channel, "Index is out of range", WARNING);
		return ;
	}

	
	if (deck[index] == WILD || deck[index] == WILD_DRAW_4) {
		if (checkWild(botData, _channel, deck[index], color) == 1)
			return ;
		else if (deck[index] == WILD_DRAW_4)
			_currentDraw += 4;
		_currentColor = color;
	}
}
