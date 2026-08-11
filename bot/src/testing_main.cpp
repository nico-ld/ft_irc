#include "Bot.hpp"
#include "Game.hpp"
#include "Uno.hpp"

#include <iostream>

int main(void) {
	// Init dashboard
	DashData data;
	initData(data, "127.0.0.1", 6667);
	data.server.mode = "Simulation";
	data.server.connected = true;
	data.bot.channelsJoined = 1;
	data.bot.state = "Up";

	Dashboard dash(ROUXBOT, "bot.log");
	dash.setServerInfo(data.server);
	dash.setBotInfo(data.bot);
	dash.setGames(data.games);
	dash.render();

	t_bot_data botData;
	botData.sock = 27;
	botData.data = data;
	botData.dash = &dash;

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
