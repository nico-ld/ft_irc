// ---------------------------------------------------------------------
// Demo — remove this main() once you wire the Dashboard into ircbot.cpp
// ---------------------------------------------------------------------

#include "Dashboard.hpp"

int main() {
    Dashboard dash(ROUXBOT, "bot.log");

    ServerInfo server;
    server.mode = "simulation";
    server.host = "127.0.0.1";
    server.serverName = "ircserv";
    server.port = 6667;
    server.connected = true;
    dash.setServerInfo(server);

    BotInfo bot;
    bot.state = "processing";
    bot.channelsJoined = 3;
    bot.gamesAmount = 2;
    bot.playerAmount = 6;
    bot.currentTask.command = "join";
    bot.currentTask.game = "Uno";
    bot.currentTask.channel = "#general";
    dash.setBotInfo(bot);

    GameChannelInfo unoChan;
    unoChan.name = "#general";
    unoChan.playerAmount = 2;
    unoChan.gameState = "READY";
    GameTypeInfo uno;
    uno.name = "UNO";
    uno.gamesAmount = 1;
    uno.playerAmount = 2;
    uno.channels.push_back(unoChan);

    GameChannelInfo wolfChan;
    wolfChan.name = "#werewolfLover";
    wolfChan.playerAmount = 4;
    wolfChan.gameState = "READY";
    GameTypeInfo werewolf;
    werewolf.name = "WEREWOLF";
    werewolf.gamesAmount = 1;
    werewolf.playerAmount = 4;
    werewolf.channels.push_back(wolfChan);

    std::vector<GameTypeInfo> games;
    games.push_back(uno);
    games.push_back(werewolf);
    dash.setGames(games);

    dash.log("INFO", "bot connected to ircserv on 6667");
    dash.log("INFO", "auto-joined #general after INVITE");

    dash.render();
    // In real usage: call dash.render() again every time state changes
    // (new JOIN, new game move, etc.) right after updating BotInfo /
    // GameTypeInfo.

    return 0;
}