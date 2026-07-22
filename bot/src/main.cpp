#include "Bot.hpp"

int main(int ac, char **av) {
	if (ac != 2) {
		std::cerr << "ERROR : wrong amount of argument, please enter only USER name." << std::endl;
		return (1);
	}
	try {
		// Start Bot
		Bot RouxBot;

		// Set USER for personnal behavior
		RouxBot.setUser(av[1]);
		
		std::string line = "";
		std::cout << "Type 'EXIT' to end communication." << std::endl;
		while(line != "EXIT") {
			std::cout << "> ";
			std::getline(std::cin, line);
			if (line != "EXIT" && RouxBot.processMessage(line) == 1 && !line.empty())
				std::cout << line << std::endl;
		}
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
}