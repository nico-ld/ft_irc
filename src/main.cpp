#include "ft_irc.hpp"
#include <sstream>

int main(int ac, char **av) {
	if (ac != 3)
		return 1;

	std::stringstream ss(av[1]);
	int port;
	ss >> port;
	std::string password = av[2];
	Server server(port, password);

	std::string line;
	std::cout << "input > ";
	std::getline(std::cin, line);

	User user(5);

	Parser::parse(line);
	std::cout << "Line parsed : " << std::endl;
	std::cout << "\t- Prefix : " << Parser::getPrefix() << std::endl;
	std::cout << "\t- Command : " << Parser::getCommand() << std::endl;

	std::vector<std::string> content = Parser::getParameters();
	std::cout << "\t- Parameters : " << content << std::endl;
	content = Parser::getTrailing();
	std::cout << "\t- Trailing : " << content << std::endl;

	dispatchCommand(server, user);

	// TEST JOIN
	std::cout << server.getChannelByName("#general") << std::endl;

	// TEST KICK
	std::cout << server.getUserById(user.getFd());
}
