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

	t_bot_data botData;
	botData.sock = 27;
	botData.data = data;
	botData.dash = &dash;

	catchCommand(":LeRoux!User@host PRIVMSG #general :!help", botData);
	sleep(1);

	catchCommand(":LeRoux!User@host PRIVMSG #general :!game create uno", botData);
	sleep(1);

	catchCommand(":ElJulien!User@host PRIVMSG #general :!game join", botData);
	sleep(1);

	catchCommand(":AmyUnMax!User@host PRIVMSG #general :!game join", botData);
	sleep(1);

	catchCommand(":ElJulien!User@host PRIVMSG #general :!game leave", botData);
	sleep(1);

	catchCommand(":ElJulien!User@host PRIVMSG #general :!game join", botData);
	sleep(1);

	catchCommand(":ElJulien!User@host PRIVMSG #general :!game start", botData);
	sleep(1);

	dash.log(SYSTEM, "Program ended");

}
