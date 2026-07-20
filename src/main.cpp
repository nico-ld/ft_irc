#include "ft_irc.hpp"

int main(int ac, char **av) {
	if (ac != 3)
		return 1;

	std::stringstream ss(av[1]);
	int port;
	ss >> port;
	std::string password = av[2];
	Server server(port, password);

	server.initServer();
	server.startLoop();
	server.stopServer();

	Parser::initCommandList();

	std::string line;
	std::cout << "input > ";
	std::getline(std::cin, line);

	Parser::parse(line);
	std::cout << "Line parsed : " << std::endl;
	std::cout << "\t- Prefix : " << Parser::getPrefix() << std::endl;
	std::cout << "\t- Command : " << Parser::getCommand() << std::endl;
	
	std::vector<std::string> content = Parser::getParameters();
	std::cout << "\t- Parameters : " << content << std::endl;
	content = Parser::getTrailing();
	std::cout << "\t- Trailing : " << content << std::endl;
}
