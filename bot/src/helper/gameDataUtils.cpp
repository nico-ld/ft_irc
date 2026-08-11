/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameDataUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 17:14:20 by nico              #+#    #+#             */
/*   Updated: 2026/08/11 17:17:23 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

bool isPlayerInGame(std::string playerName, std::vector<std::string> playerList) {
	for (std::vector<std::string>::iterator it = playerList.begin(); it != playerList.end() ; ++it) {
		if (playerName == *it)
			return (true);
	}
	return (false);
}
