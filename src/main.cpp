#include "ft_irc.hpp"
#include "Replies.hpp"
#include <cstdlib>

int main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << "Error: " 
					<< ((ac < 3) ? "not enought" : "too many") 
					<< " arguments." 
					<< std::endl;
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	char	*ptrEnd;
	int		port = (int)strtol(av[1], &ptrEnd, 10);
	if (*ptrEnd) {
		std::cerr << "Error: invalid value for server port." << std::endl;
		return (1);
	}

	if (port < 6665 || port > 6669) {
		std::cerr << "Error: invalid port, please port in a valid range [6665 - 6669]" << std::endl;
		return (1);
	}

	try {
		Server server(port, av[2]);
		server.init();
		server.startLoop();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}