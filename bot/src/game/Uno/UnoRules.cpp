/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnoRules.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 16:47:10 by nico              #+#    #+#             */
/*   Updated: 2026/08/17 17:02:08 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "Uno.hpp"

void Uno::rules(t_bot_data &botData)
{
	// Rules
	sendMessage(botData, _channel,
		"The rules that I follow are theses ones : https://unorules.net/wp-content/uploads/2024/08/Official-Uno-rules-PDF-new-design-white.pdf. Also, you can stack +2 with +2 and +4 with +4 : not +2 and +4 together !", 
		CLIENT);

	// How to play part I
	sendMessage(botData, _channel,
		"How to play ? This is simple, You already get it ! Start your message with '!uno' and then the wanted command. : 'turn', show player who have to play; 'hand', show your hand by private message; 'uno', yell UNO if you get only 1 card or catch a player who didn't yell it; 'cards', show cards amount of every player; 'top' show the last card played; 'play <cardIndex> [color]', to play a card from your hand (color arg is for wild card)",
		CLIENT);

	// How to play part II 
	sendMessage(botData, _channel, "'draw', draw a new card; 'challenge', challenge the last player if he play a wild draw 4", CLIENT);
}
