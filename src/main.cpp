#include <iostream>
#include <string>
#include "core/Server.hpp"
#include <sstream>

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
	return 0;
}
