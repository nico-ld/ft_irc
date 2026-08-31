#include "Bot.hpp"
#include "Game.hpp"
#include "Uno.hpp"

#include <iostream>
#include <unistd.h>

int main(void) {
	std::srand(std::time(NULL));
	Dashboard dash(ROUXBOT, "bot.log");

	// Init dashboard
	// Server
	t_column serverInfo;
	serverInfo.infoList.push_back(std::make_pair("Connection", "OK"));
	serverInfo.infoList.push_back(std::make_pair("Port", "6667"));
	serverInfo.infoList.push_back(std::make_pair("Server name", "ircserv"));

	t_section server;
	server.title = "server";
	server.mainInfo = std::make_pair("MODE", "Simulation");
	server.leftColumn = serverInfo;

	// Bot
	t_column botGlobal;
	botGlobal.title = "Global info";
	botGlobal.infoList.push_back(std::make_pair("Chan. joined", "0"));
	botGlobal.infoList.push_back(std::make_pair("Games", "0"));
	botGlobal.infoList.push_back(std::make_pair("Player amount", "0"));

	t_column lastTask;
	lastTask.title = "Last command";
	lastTask.infoList.push_back(std::make_pair("Command", "None"));
	lastTask.infoList.push_back(std::make_pair("Game", "Uno"));
	lastTask.infoList.push_back(std::make_pair("Channel", "None"));

	t_section bot;
	bot.title = "bot";
	bot.mainInfo = std::make_pair("STATE", "Up");
	bot.leftColumn = botGlobal;
	bot.rightColumn = lastTask;

	// Games
	t_column uno;
	uno.title = "UNO";
	uno.infoList.push_back(std::make_pair("Games", "0"));
	uno.infoList.push_back(std::make_pair("Player", "0"));
	uno.elemListTitle = "Info by channel";

	t_section game;
	game.title = "game";
	game.leftColumn = uno;

	dash.addSection(server);
	dash.addSection(bot);
	dash.addSection(game);
	dash.render();

	// Init botData
	t_bot_data botData;
	botData.sock = 27;
	botData.dash = &dash;

	// init game
	catchCommand(":leroux! PRIVMSG #channel :!game create uno", botData);
	catchCommand(":amy! PRIVMSG #channel :!game join", botData);
	catchCommand(":leroux! PRIVMSG #channel :!game start", botData);

	std::string line;
	std::cout << " > ";
	while (std::getline(std::cin, line)) {
		if (line == "EXIT")
			break ;
		catchCommand(line, botData);
		std::cout << " > ";
	}

	// free not ended games
	std::vector<Game *>::iterator it;
	for (it = botData.games.begin(); it != botData.games.end(); ++it) {
		delete *it;
	}
}
