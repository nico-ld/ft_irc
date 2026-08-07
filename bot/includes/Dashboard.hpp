#include "../../../includes/ansi.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm> // std::fill_n, for repeating multi-byte UTF-8 border chars
#include <iterator>  // std::ostream_iterator

// ---------------------------------------------------------------------
// Data model — fill these from your bot's real state before calling
// Dashboard::render()
// ---------------------------------------------------------------------

struct ServerInfo
{
	std::string mode; // "simulation", "live", ...
	std::string host;
	std::string serverName;
	int port;
	bool connected;
};

struct BotTask
{
	std::string command; // "join", "privmsg", "idle", ...
	std::string game;	 // "Uno", "Werewolf", "-"
	std::string channel; // "#general"
};

struct BotInfo
{
	std::string state; // "processing", "idle", ...
	int channelsJoined;
	int gamesAmount;
	int playerAmount;
	BotTask currentTask;
};

struct GameChannelInfo
{
	std::string name; // "#general"
	int playerAmount;
	std::string gameState; // "READY", "WAITING", "IN_PROGRESS", ...
};

struct GameTypeInfo
{
	std::string name; // "UNO", "WEREWOLF"
	int gamesAmount;
	int playerAmount;
	std::vector<GameChannelInfo> channels; // usually one, but supports more
};

struct DashData
{
	ServerInfo	server;
	BotInfo		bot;
	std::vector<GameTypeInfo> games;
};

// ---------------------------------------------------------------------
// Dashboard
// ---------------------------------------------------------------------

class Dashboard
{
public:
	Dashboard(const std::string &botName, const std::string &logPath);
	~Dashboard();

	void setServerInfo(const ServerInfo &s) { _server = s; }
	void setBotInfo(const BotInfo &b) { _bot = b; }
	void setGames(const std::vector<GameTypeInfo> &g) { _games = g; }

	// Writes a timestamped line to bot.log. Never touches stdout, so it
	// can never corrupt the redrawing dashboard below.
	void log(const std::string &level, const std::string &message);

	// Clears the terminal and redraws the whole box from current state.
	void render();

private:
	static const int INNER_WIDTH = 53; // between the two vertical borders
	static const int COL_WIDTH = 25;   // width of each half in two-column rows

	std::string _botName;
	std::string _logPath;
	std::ofstream _logFile;
	ServerInfo _server;
	BotInfo _bot;
	std::vector<GameTypeInfo> _games;

	static std::string toStr(int v);

	static std::string timestamp();

	// No color codes anywhere now, so plain text width == byte length —
	// std::setw/std::left can pad directly, no hand-written padRight
	// needed. centerText still has no manipulator equivalent (there's
	// no std::center), so it stays as a small helper.
	static std::string centerText(const std::string &s, size_t width);

	// Box corners/edges: UTF-8 bytes for ┌ ┐ └ ┘ │ ─ (no u8"" prefix —
	// that requires C++11, and ft_irc targets C++98).
	//
	// std::fill_n + std::ostream_iterator<std::string> repeats the 3-byte
	// "─" glyph N times — std::setfill() can't do this, it only accepts
	// a single char, and '─' isn't representable as one.
	void printTopBorder() const
	{
		std::cout << "┌";
		std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
		std::cout << "┐\n";
	}
	void printBottom() const
	{
		std::cout << "└";
		std::fill_n(std::ostream_iterator<std::string>(std::cout), INNER_WIDTH, "─");
		std::cout << "┘\n";
	}
	void printThinSeparator() const
	{
		std::cout << "│" << std::string(INNER_WIDTH, '-')
				  << "│\n";
	}
	void printRow(const std::string &text) {
		std::cout << "│ " << std::left << std::setw(INNER_WIDTH - 2) << text
				  << " │\n";
	}

	void printSectionHeader(const std::string &title,
							 const std::string &rightLabel) const;

	void printTwoCol(const std::string &left, const std::string &right) const;
};

void initData(DashData &data);
