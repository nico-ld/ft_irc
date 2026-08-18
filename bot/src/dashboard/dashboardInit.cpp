/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dashboardInit.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:58:29 by nico              #+#    #+#             */
/*   Updated: 2026/08/07 15:58:30 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void initData(DashData &data, std::string host, int port) {
    ServerInfo server;
    server.mode = "real" ;
    server.host = host;
    server.serverName = "ircserv";
    server.port = port;
    server.connected = false;
    data.server = server;

    BotInfo bot;
    bot.state = "down";
    bot.channelsJoined = 0;
    bot.gamesAmount = 0;
    bot.playerAmount = 0;
    bot.currentTask.command = "";
    bot.currentTask.game = "";
    bot.currentTask.channel = "";
    data.bot = bot;

    GameTypeInfo uno;
    uno.name = "UNO";
    uno.gamesAmount = 0;
    uno.playerAmount = 0;

    GameTypeInfo werewolf;
    werewolf.name = "WEREWOLF";
    werewolf.gamesAmount = 0;
    werewolf.playerAmount = 0;

    std::vector<GameTypeInfo> games;
    games.push_back(uno);
    games.push_back(werewolf);
    data.games = games;
}