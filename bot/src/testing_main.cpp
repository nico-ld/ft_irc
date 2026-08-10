#include "Bot.hpp"
#include "Game.hpp"
#include "Uno.hpp"
#include <unistd.h>

int main(void) {
	DashData data;
	initData(data, "127.0.0.1", 6667);
	data.server.mode = "Simulation";
	data.server.connected = true;

	Dashboard dash(ROUXBOT, "bot.log");
	dash.setServerInfo(data.server);
	dash.setBotInfo(data.bot);
	dash.setGames(data.games);
	dash.render();

	std::string channel = "#general";
	Game *uno = new Uno(27, channel);

	std::string player = "leRoux";
	std::string secondPlayer = "Julien";
	std::string thirdPlayer = "Amy";

	try {
		uno->initGame(player, data, dash);
		sleep(1);
		uno->addPlayer(secondPlayer, data, dash);
		sleep(1);
		uno->addPlayer(thirdPlayer, data, dash);
		sleep(1);

		uno->removePlayer(secondPlayer, data, dash);
		sleep(1);
		uno->removePlayer(player, data, dash);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	dash.log(SYSTEM, "Program ended");

	delete uno;
}
