#include <cstdlib>
#include <iostream>

#include "Server.hpp"
#include "ServerConfig.hpp"

int main()
{
	ServerConfig	cfg;
	Server			server(cfg);

	if (!server.startServer())
	{
		std::cerr << "Failed to start server." << std::endl;
		return EXIT_FAILURE;
	}
	server.runServer();
	return EXIT_SUCCESS;
}
