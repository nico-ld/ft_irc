/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dashboard.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:59:07 by nico              #+#    #+#             */
/*   Updated: 2026/08/10 10:32:39 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dashboard.hpp"

Dashboard::Dashboard(const std::string &botName, const std::string &logPath)
	: _botName(botName), _logPath(logPath)
{
	_logFile.open(_logPath.c_str(), std::ios::app);
	if (!_logFile.is_open())
		std::cerr << "[ERROR] could not open log file " << _logPath << "\n";

	_logFile << std::endl;
	log(SYSTEM, "Starting log");
	_logFile << std::endl;
}

Dashboard::~Dashboard()
{
	if (_logFile.is_open())
		_logFile.close();
}

void Dashboard::log(const std::string &level, const std::string &message)
{
	if (!_logFile.is_open())
		return;
	_logFile << DIM "[" << timestamp() << "]" RESET BOLD " [" << level << BOLD "] " RESET
			 << message << std::endl;
}

void Dashboard::render()
{
	std::cout << SCREEN_CLEAR << setCursor(1, 1);

	std::cout << _botName << CYAN " Dashboard" RESET << std::endl;
	printTopBorder();

	// === SECTION SERVER ===
	printSectionHeader("SERVER", "Mode : " + _server.mode);
	printRow("");
	printRow(std::string("Connection : ") + (_server.connected ? "OK" : "DOWN"));
	printRow("Port : " + toStr(_server.port));
	printRow("Host : " + _server.host);
	printRow("Server name : " + _server.serverName);

	printThinSeparator();

	// === SECTION BOT ===
	printSectionHeader("BOT", "State : " + _bot.state);
	printTwoCol("", "");
	printTwoCol("Global :", "Last task :");
	printTwoCol("Channel joined : " + toStr(_bot.channelsJoined),
				"Command : " + _bot.currentTask.command);
	printTwoCol("Games : " + toStr(_bot.gamesAmount),
				"Game : " + _bot.currentTask.game);
	printTwoCol("Player amount : " + toStr(_bot.playerAmount),
				"Channel : " + _bot.currentTask.channel);

	printThinSeparator();

	// === SECTION GAME ===
	printSectionHeader("GAME", "");
	printTwoCol("", "");

	// Render game types two per row, same layout as the mockup.
	for (size_t i = 0; i < _games.size(); i += 2)
	{
		const GameTypeInfo &left = _games[i];
		bool hasRight = (i + 1) < _games.size();
		const GameTypeInfo *right = hasRight ? &_games[i + 1] : NULL;

		printTwoCol(centerText(left.name, COL_WIDTH),
					hasRight ? centerText(right->name, COL_WIDTH) : "");
		printTwoCol("Games amount : " + toStr(left.gamesAmount),
					hasRight ? "Games amount : " + toStr(right->gamesAmount) : "");
		printTwoCol("Player amount : " + toStr(left.playerAmount),
					hasRight ? "Player amount : " + toStr(right->playerAmount) : "");
		printTwoCol("", "");
		printTwoCol("Info by channel :", hasRight ? "Info by channel :" : "");

		size_t chanRows = left.channels.size();
		if (hasRight && right->channels.size() > chanRows)
			chanRows = right->channels.size();

		for (size_t c = 0; c < chanRows; ++c)
		{
			std::string lname, lplayers, lstate;
			if (c < left.channels.size())
			{
				lname = "  Name : " + left.channels[c].name;
				lplayers = "  Player amount : " + toStr(left.channels[c].playerAmount);
				lstate = "  Game state : " + left.channels[c].gameState;
			}
			std::string rname, rplayers, rstate;
			if (hasRight && c < right->channels.size())
			{
				rname = "  Name : " + right->channels[c].name;
				rplayers = "  Player amount : " + toStr(right->channels[c].playerAmount);
				rstate = "  Game state : " + right->channels[c].gameState;
			}
			printTwoCol(lname, rname);
			printTwoCol(lplayers, rplayers);
			printTwoCol(lstate, rstate);
		}
		printTwoCol("", "");
	}

	printBottom();
	std::cout << "\nLogs -> " << _logPath
			  << " (tail -f " << _logPath << ")\n";
	std::cout.flush();
}

std::string Dashboard::toStr(int v)
{
	std::ostringstream oss;
	oss << v;
	return oss.str();
}

std::string Dashboard::timestamp()
{
	std::time_t t = std::time(NULL);
	char buf[32];
	std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&t));
	return std::string(buf);
}

std::string Dashboard::centerText(const std::string &s, size_t width)
{
	if (s.size() >= width)
		return s;
	size_t total = width - s.size();
	size_t left = total / 2;
	size_t right = total - left;
	return std::string(left, ' ') + s + std::string(right, ' ');
}

void Dashboard::printSectionHeader(const std::string &title,
								   const std::string &rightLabel) const
{
	size_t used = title.size() + rightLabel.size();
	size_t space = (INNER_WIDTH - 2 > used) ? (INNER_WIDTH - 2 - used) : 1;
	std::cout << "│ " << title << std::string(space, ' ') << rightLabel << " │\n";
}

void Dashboard::printTwoCol(const std::string &left, const std::string &right) const
{
	std::cout << "│ " << std::left << std::setw(COL_WIDTH) << left << "| "
			  << std::left << std::setw(INNER_WIDTH - 1 - COL_WIDTH - 2) << right
			  << "│\n";
}
