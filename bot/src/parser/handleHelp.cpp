/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleHelp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 16:24:03 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 17:15:00 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void handleHelp(t_bot_data &botData) {
	std::string channel = botData.parser.getParameters()[0];

	// General informations
	sendMessage(botData, channel,
		"RouxBot is an IRC bot that manage Uno and Werewolf (if dev get enought time and motivation). To use it you get 3 command groups : '!game',  '!uno' and '!ww' or '!werewolf'. '!help' is a  independant command. To get special information about Uno or Werewolf each one get 'rules' command. So the correct syntax for rouxbot is '!<command group> <command> [arg]'",
		CLIENT
	);

	// '!game' command description
	sendMessage(botData, channel,
		"Here the description of commands in '!game'. 'create <game>', init a new game of Uno or Werewolf; 'join', to join  a game not started yet; 'leave', to leave a game, even if game has started; 'kick <user>', to kick someone from the game (not from the channel); 'start', to start a game when there is enought player; 'end', to end a game in case of problem.",
		CLIENT
	);
}
