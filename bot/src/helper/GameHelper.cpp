/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GameHelper.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 10:55:12 by nico              #+#    #+#             */
/*   Updated: 2026/08/15 10:58:35 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Bot.hpp"

Game *getCurrentGame(t_bot_data &botData, std::string channel) {
	for (std::vector<Game *>::iterator it = botData.games.begin(); it != botData.games.end(); ++it) {
		if ((*it)->getChannel() == channel)
			return (*it);
	}
	return (NULL);
}
