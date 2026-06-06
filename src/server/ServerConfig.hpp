#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>


// TODO: Replace with the real parsed config
struct ServerConfig
{
	ServerConfig();
	ServerConfig(const ServerConfig &copy);
	ServerConfig &operator=(const ServerConfig &copy);
	~ServerConfig();

	int			port;
	std::string	host;

};

#endif
