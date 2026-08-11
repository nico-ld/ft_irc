/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameDataUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:14:20 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 22:27:01 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Game.hpp"

size_t findGameByChannel(std::vector<GameChannelInfo> &channels, std::string &target)
{
	size_t index = 0;
	for ( ; index < channels.size(); ++index) {
		if (channels[index].name == target)
			return (index);
	}
	return (std::string::npos);
}

void applyDashData(t_bot_data &botData) {
	// Refresh data
	botData.dash->setBotInfo(botData.data.bot);
	botData.dash->setGames(botData.data.games);
	botData.dash->setServerInfo(botData.data.server);

	// Refresh dashboard
	botData.dash->render();
}
