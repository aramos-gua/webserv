#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : port(8080), host ("0.0.0.0")
{
}

ServerConfig::ServerConfig(const ServerConfig &copy): port(copy.port), host(copy.host)
{
}

ServerConfig &ServerConfig::operator=(const ServerConfig &copy)
{
	if (this != &copy)
	{
		this->port = copy.port;
		this->host = copy.host;
	}
	return (*this);
}

ServerConfig::~ServerConfig()
{

}
