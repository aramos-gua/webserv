#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>


// TODO: Replace with the real parsed config
struct ServerConfig
{
	int			port;
	std::string	host;

	ServerConfig(): port(8080), host("0.0.0.0") {}
};

#endif
