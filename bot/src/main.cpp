#include "Bot.hpp"

int main(int ac, char **av) {
	if (ac != 3) {
		std::cout << RED BOLD "ERROR: " RESET
				<< ((ac < 3) ? "not enought parameters" : "too many parameters")
				<< std::endl;
		std::cout << BOLD "Usage: " RESET "<server_port> <server_password>" << std::endl;
		return (1);
	}
	(void)av;

	Bot rouxbot;

	rouxbot.setUser("nico");
	std::string line = "";
	std::cout << "Type 'EXIT' to end communication." << std::endl;
	while(line != "EXIT") {
		std::cout << "> ";
		std::getline(std::cin, line);
		if (line != "EXIT" && rouxbot.processMessage(line) == 1 && !line.empty())
			std::cout << line << std::endl;
	}

	// User rouxbotProfil(0);
	// rouxbotProfil.setUsername("rouxbot");
	// rouxbotProfil.setNickname("RouxBot");
	// rouxbotProfil.setPrefix("rouxbot!RouxBot@localhost");
	// rouxbotProfil.setProvidedNick(true);
	// rouxbotProfil.setProvidedUser(true);
	// rouxbotProfil.setProvidedPassword(true);
	// rouxbotProfil.setAuthenticated(true);
}