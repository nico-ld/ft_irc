#include "Bot.hpp"
#include "Game.hpp"
#include "Uno.hpp"
#include <unistd.h>

int main(void) {
	// Init dashboard
	DashData data;
	initData(data, "127.0.0.1", 6667);
	data.server.mode = "Simulation";
	data.server.connected = true;
	data.bot.channelsJoined = 1;

	Dashboard dash(ROUXBOT, "bot.log");
	dash.setServerInfo(data.server);
	dash.setBotInfo(data.bot);
	dash.setGames(data.games);
	dash.render();

	// Create games
	std::string channel1 = "#general";
	std::string channel2 = "#UnoExperts";
	Game *game1 = new Uno(27, channel1);
	Game *game2 = new Uno(27, channel2);

	// Create player
	std::string player = "leRoux";
	std::string secondPlayer = "Julien";
	std::string thirdPlayer = "Amy";

	try {
		// init game
		game1->initGame(player, data, dash);
		game2->initGame(player, data, dash);
		sleep(1);
		
		// add player 
		game1->addPlayer(secondPlayer, data, dash);
		game2->addPlayer(secondPlayer, data, dash);
		game2->addPlayer(secondPlayer, data, dash);
		sleep(1);
		
		game1->addPlayer(thirdPlayer, data, dash);
		sleep(1);

		// remove player
		game1->removePlayer(secondPlayer, data, dash);
		game1->removePlayer("Bob", data, dash);
		sleep(1);
		game1->removePlayer(player, data, dash);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	dash.log(SYSTEM, "Program ended");

	delete game1;
	delete game2;
}
